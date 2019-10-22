#include "common.h"
payload_t payload __attribute__((section(".data"))) = {0x00};
void main()
{
	pid_t pid;
	int status;
	char *argv[] = {NULL, NULL, NULL};
	printf("inter:%s script:%s\n", payload.inter, payload.data);
	pid = fork();
	if (pid == 0) {
		argv[0] = payload.inter;
		argv[1] = payload.data;
		execv(payload.inter, argv);
	}
	waitpid(pid, &status, 0);
}