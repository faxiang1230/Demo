/*************************************************************************
    > File Name: test.c
    > Author: wangjx
 ************************************************************************/
#include <unistd.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main()
{
	int status;
	pid_t ppid = 0;
	prctl(PR_SET_CHILD_SUBREAPER, 1);
	printf("grandfather pid:%d\n", getpid());
	if (fork() == 0) {
		sleep(1);
		printf("father pid:%d\n", getpid());
		if (fork() == 0) {
			ppid = getppid();
			printf("child pid:%d father pid:%d\n", getpid(), ppid);
			while (1) {
				if (ppid != getppid())
					printf("child pid:%d father pid:%d\n", getpid(), ppid = getppid());
				sleep(1);
			}
		} else {
			exit(-1);
		}	
	} else {
		while(1) {
			printf("pid %d exit\n", waitpid(0, &status, 0));
		}
	}
	return 0;
}
