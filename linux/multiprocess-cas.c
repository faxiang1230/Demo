#include <sys/mman.h>   /*  `shm_open`, `shm_unlink`, `mmap`                */
#include <sys/stat.h>   /*  `S_IRUSR`, `S_IWUSR`                            */
#include <fcntl.h>      /*  `O_RDWR`, `O_CREAT`                             */
#include <string.h>     /*  `strerror`                                      */
#include <unistd.h>     /*  `ftruncate`, `getpid`, `pid_t`, `fork`, `close` */
#include <stdlib.h>     /*  `rand_r`                                        */
#include <errno.h>      /*  `errno`                                         */
#include <stdlib.h>     /*  `abort`                                         */
#include <time.h>       /*  `struct timespec`, `nanosleep`                  */
#include <stdio.h>      /*  `printf`                                        */

#define false 0
#define true ~false

int shm_init(void **shm, int *shm_fd, char *shm_handle);
int child_task(int a_child, void *shm);
int random_sleep(void);

int shm_init(void **shm, int *shm_fd, char *shm_handle) {
    /*  Open a shared memory space  */
    if (!(*shm_fd = shm_open(shm_handle, (O_RDWR | O_CREAT), (S_IRUSR | S_IWUSR)))) {
        printf("%s\n", strerror(errno));
        abort();
    }

    /*  Truncate shared memory to the size of an `int`. */
    if (ftruncate(*shm_fd, sizeof(int)) == -1) {
        printf("ftruncate: %s\n", strerror(errno));
        abort();
    }

    /*  Map the shared space    */
    if (!(*shm = mmap(NULL, sizeof(int), (PROT_READ | PROT_WRITE), (MAP_SHARED | MAP_LOCKED), *shm_fd, 0))) {
        printf("%s\n", strerror(errno));
        abort();
    }

    /*  Unlink so file goes away on last close  */
    if (shm_unlink(shm_handle) == -1) {
        printf("%s\n", strerror(errno));
        abort();
    }

    return true;
}

int child_task(int a_child, void *shm) {
    int per_child       = 50000;    /*  Number of increments each worker contributes                    */
    int an_iteration    = 0;    /*  Current increment to contribute                                 */
    int val             = 0;    /*  The next proposed increment to the shared counter               */
    int val_old         = 0;    /*  The last known value of the shared counter                      */
    int val_last        = 0;    /*  The shared counter value when attemping to compare-and-swap     */

    printf("child %d PID: %d\n", a_child, getpid());

    /*  Do `per_child` iterations (increments to the shared counter)    */
    for (; an_iteration < per_child; an_iteration++) {
        while (1) {
            val     = *((int *) shm);           /*  Read shared counter     */
            val_old = val;                      /*  Save old counter        */
            val++;                              /*  Increment local counter */

            /*  Create swap failures for demonstration purposes */
            //random_sleep();

            /*  Swap counter if old counter == shared counter   */
            val_last = __sync_val_compare_and_swap((int *) shm, val_old, val);

            /*  If `val_old` != `val_last`, another child incremented,  */
            if (val_old != val_last) {                                          
                printf("child %d: no swap (%d), val_old: %d, val_last: %d\n", a_child, an_iteration, val_old, val_last);
                continue;
            /*  Otherwise, this child's increment was successful.   */
            } else {                                                            
                printf("child %d: swap (%d), val_old: %d, val_last: %d\n", a_child, an_iteration, val_old, val_last);
            }

            /*  Print the shared counter value contributed by the child.    */
            printf("child %d: %d\n", a_child, val);
            break;
        }
    }

    return true;
}

int random_sleep(void) {
    static unsigned int seed    = 0;    /*  Re-entrant seed for rand_r()                                    */
    struct timespec sleepspec   = {     /*  Time to sleep between reading shared counter and incrementing   */
        .tv_sec                 = 0,
        .tv_nsec                = 0
    };

    sleepspec.tv_nsec = rand_r(&seed);
    nanosleep(&sleepspec, NULL);

    return true;
}

int main(int argc, char *argv[]) {
    char *shm_handle    = "/cas_test_shm";
    int shm_fd          = 0;
    void *shm           = NULL;
    int children        = 4;
    pid_t child         = 0;                /*  Worker PID     */
    int a_child         = 0;                /*  Current worker */

    printf("parent PID: %d\n", getpid());

    shm_init(&shm, &shm_fd, shm_handle);

    for (; a_child < children; a_child++) {
        if ((child = fork()) < 0)
            abort();

        if (child == 0) {
            child_task(a_child, shm);

            printf("child %d: going home\n", a_child);
            if (close(shm_fd) == -1) {
                printf("%s\n", strerror(errno));
                abort();
            }

            return true;
        }
    }

    printf("parent: going home\n");

    if (close(shm_fd) == -1) {
        printf("%s\n", strerror(errno));
        abort();
    }

    return true;
}
