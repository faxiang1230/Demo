/*
 * (1) helper创建一个子进程H，注意要用clone()和CLONE_FS，使得helper和H可以共享根目录、当前目录、等等；
   (2) helper降权后执行execve("worker")；
   (3) worker(原helper进程)请求H去执行chroot()；
   (4) H执行chroot()，新的根目录会对H和worker同时生效。
   (5) H退出
*/
#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#define SANDBOX_ROOT    "."
#define STACK_SIZE (1024 * 1024)    /* Stack size for cloned child */
#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); } while (0)
extern char** environ;
int h()
{
    int ret = 0;
    ret = chroot(SANDBOX_ROOT);
    if (ret < 0)
        errExit("faield to chroot");
    return ret;
}

int my_clone(int (*fn)(void *), void *argv)
{
    char *stack;                    /* Start of stack buffer */
    char *stackTop;                 /* End of stack buffer */
    pid_t pid;


    /* Allocate stack for child */

    stack = malloc(STACK_SIZE);
    if (stack == NULL)
        errExit("malloc");
    stackTop = stack + STACK_SIZE;  /* Assume stack grows downward */

    /* Create child that has its own UTS namespace;
     *               child commences execution in childFunc() */

    pid = clone(fn, stackTop, CLONE_FS | SIGCHLD, argv);
    if (pid == -1)
        errExit("clone");
    printf("clone() returned %ld\n", (long) pid);

    return 0;
}
int rlimit(void)
{
    //resource limit,do nothing
    return 0;
}
int main(int argc, char **argv)
{
    int ret = 0, fd = 0;

    fd = open(argv[1], O_RDONLY|O_CLOEXEC);
    if (fd < 0) errExit("failed to open file");

    ret = rlimit();
    if (ret < 0) errExit("failed to resource limit");
    
    my_clone(h, NULL);

    ret = setuid(1010);
    if (ret < 0) errExit("failed to setuid");

    fexecve(fd, &argv[2], environ);
    
    return 0;
}
