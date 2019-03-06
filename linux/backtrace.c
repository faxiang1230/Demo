#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
void mysighandler(int signo)
{
	int j, nptrs;
#define SIZE 100
	void *buffer[100];
	char **strings;

	nptrs = backtrace(buffer, SIZE);
	printf("backtrace() returned %d addresses\n", nptrs);

	/* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
	 *               would produce similar output to the following: */

	strings = backtrace_symbols(buffer, nptrs);
	if (strings == NULL) {
		perror("backtrace_symbols");
		exit(EXIT_FAILURE);
	}

	for (j = 0; j < nptrs; j++)
		printf("%s\n", strings[j]);

	free(strings);
}
const int g = 2;
int main(int argc, char *argv[])
{
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags |= SA_RESETHAND;
	sa.sa_handler = mysighandler;
	sigaction(SIGSEGV, &sa, NULL);
	add();
}
