#include <unistd.h>
#include <sys/types.h>		/* See NOTES */
#include <sys/socket.h>
#include <linux/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define PATH "/home/wangjx/localsocket"

void clean_up()
{
	unlink(PATH);
}

static void handleSignal(int signo)
{
	printf("recive signal:%d\n", signo);
	clean_up();
	signal(signo, SIG_DFL);
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
}

main()
{
	int i, ret, len;
	char buf[16];
	pid_t pid = fork();
	struct sockaddr_un un, client;

	atexit(clean_up);
	registerSigHandler();

	if (pid < 0) {
		printf("fork failed\n");
		exit(-1);
	}

	i = socket(PF_UNIX, SOCK_DGRAM | SOCK_CLOEXEC, 0);
	memset(&un, 0, sizeof(struct sockaddr_un));
	un.sun_family = AF_UNIX;
	strcpy(un.sun_path, PATH);

	if (pid == 0) {
		while (1) {
			ret =
			    sendto(i, "hello", 6, 0, (struct sockaddr *)&un,
				   sizeof(struct sockaddr_un));
			printf("client send:%d\n", ret);
			sleep(1);
		}
	} else {
		bind(i, (struct sockaddr *)&un, sizeof(struct sockaddr_un));
		while (1) {
			len = sizeof(struct sockaddr_un);
			ret =
			    recvfrom(i, buf, 16, 0, (struct sockaddr *)&client,
				     &len);
			printf("server recv:%s\n", buf);
		}
	}
}
