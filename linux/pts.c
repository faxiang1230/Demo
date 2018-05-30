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
int main()
{

	int ptm, status;
	pid_t pid;
	ptm = open("/dev/ptmx", O_RDWR | O_CLOEXEC);	// | O_NOCTTY);
	if (ptm < 0) {
		printf("[ cannot open /dev/ptmx - %s ]\n", strerror(errno));
		return -1;
	}

	char devname[64];
	if (grantpt(ptm) || unlockpt(ptm)
	    || ptsname_r(ptm, devname, sizeof(devname)) != 0) {
		printf("[ trouble with /dev/ptmx - %s ]\n", strerror(errno));
		close(ptm);
		return -1;
	}

	pid = vfork();
	if (pid < 0) {
		printf("- fork failed: %s -\n", strerror(errno));
		close(ptm);
		return -1;
	}

	if (pid == 0) {

		int pts = open(devname, O_RDWR | O_CLOEXEC);
		if (pts < 0) {
			fprintf(stderr,
				"child failed to open pseudo-term slave: %s\n",
				devname);
			exit(-1);
		}

		dup2(pts, STDIN_FILENO);
		dup2(pts, STDOUT_FILENO);
		dup2(pts, STDERR_FILENO);

		close(pts);
		close(ptm);

		execv("/bin/bash", NULL);
		fprintf(stderr, "- exec '/bin/ls' failed: %s (%d) -\n",
			strerror(errno), errno);
		exit(-1);
	} else {
		return ptm;
	}
}
