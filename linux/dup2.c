#define _XOPEN_SOURCE		/* See feature_test_macros(7) */
#define _GNU_SOURCE
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
//## UNFINISHED
main()
{
	int pts;
	pid_t pid;
	pid = fork();
	if(pid == 0) {
	pts = open("console-log", O_RDWR | O_CLOEXEC | O_CREAT);	// | O_NOCTTY);

	dup2(pts, STDIN_FILENO);
	dup2(pts, STDOUT_FILENO);
	dup2(pts, STDERR_FILENO);

	execv("/bin/bash", NULL);
	} else {
	execv("/bin/bash", NULL);
	}
}
