#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

void sig_handler(int signo)
{
	printf("recv signal %d\n", signo);
	//return;
	//exit(-1);
}
void main()
{
	signal(SIGABRT, sig_handler);

	//abort();
	kill(getpid(), SIGABRT);
	printf("after abort\n");
	exit(0);
}
