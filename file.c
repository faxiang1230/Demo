#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//strace -T -e open ./a.out
void main() {
	printf("open file fd:%d\n", open("client-daemon.c", O_RDWR));
	printf("open file fd:%d\n", open("client-daemon.c", O_RDWR));
	printf("open file fd:%d\n", open("client-daemon.c", O_RDWR));
}
