#include <sys/types.h>		/* See NOTES */
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/in.h>
#include <unistd.h>
#include <net/if.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX_EVENTS 16
int sv[2];

void *child()
{
	while (1) {
		write(sv[0], "hello\n", 7);
		sleep(1);
	}
}

void main()
{
	int ret = 0, n = 0;
	struct epoll_event ev, events[MAX_EVENTS];
	int nfds, epollfd, filefd;
	char buf[1024];
	pthread_t p1;

	ret = socketpair(AF_INET, SOCK_STREAM, 0, sv);
	if (ret < 0) {
		printf("socketpair failed, err %s", strerror(ret));
	}

	filefd = sv[1];
	epollfd = epoll_create(10);
	if (epollfd == -1) {
		perror("epoll_create");
		exit(EXIT_FAILURE);
	}

	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = filefd;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, filefd, &ev) == -1) {
		perror("epoll_ctl: listen_sock");
		exit(EXIT_FAILURE);
	}
	pthread_create(&p1, NULL, child, NULL);

	for (;;) {
		nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
		if (nfds == -1) {
			perror("epoll_pwait");
			exit(EXIT_FAILURE);
		}

		for (n = 0; n < nfds; ++n) {
			if (events[n].data.fd == filefd) {
				read(filefd, buf, 1024);
				printf("socketpair recv: %s", buf);
			}
		}
	}
}
