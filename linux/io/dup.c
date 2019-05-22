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
	FILE *fp;
	pts = open("console-log", O_RDWR | O_CLOEXEC | O_CREAT);	// | O_NOCTTY);

	dup2(pts, STDIN_FILENO);
	fp = fdopen(pts, "w+");
	fprintf(fp, "HELLO\n");
	fflush(fp);
	write(STDIN_FILENO, "std output", 10);
}
