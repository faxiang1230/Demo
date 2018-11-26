#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

static void handleSignal(int signo)
{
	printf("recive signal:%d\n", signo);
}
static void registerSigHandler()
{
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handleSignal;
	sa.sa_flags |= SA_RESTART;
    sigaction(SIGHUP, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    printf("sigaction SIGKILL ret:%d\n", sigaction(SIGKILL, &sa, NULL));
    printf("sigaction SIGSTOP ret:%d\n", sigaction(SIGSTOP, &sa, NULL));
}

main()
{
	registerSigHandler();
	int fd = -1;
	char buf[128];
	fd = open("hello", O_RDWR);
	assert(fd > 0);
	read(fd, buf, 128);
}
