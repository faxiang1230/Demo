#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <sys/ptrace.h>
/*
 * Just for fun, GDB can skip the ptrace result
 * Please mix up with assemble instruction
 */
main()
{
	int fd = -1;
	char buf[128];
	if (ptrace(PTRACE_TRACEME, 0, 1, 0) < 0) {
		printf("SOMEBODY TRACE ME!\n");
		return;
	}
	fd = open("hello", O_RDWR);
	assert(fd > 0);
	read(fd, buf, 128);
}
