#include <sys/epoll.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/in.h>
#include <unistd.h>
#include <net/if.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_EVENTS 10
#define PIPE_PATH "hello"
main() {
	struct epoll_event ev, events[MAX_EVENTS];
	int sockfd, conn_sock, nfds, epollfd;
	int filefd = 0;
	char buf[1024];

	/* Set up listening socket, 'listen_sock' (socket(),
	   bind(), listen()) */
    int addrlen = 0,n = 0;
    struct sockaddr_in *server = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    struct sockaddr_in local;

    addrlen = sizeof(struct sockaddr);

    server->sin_family =AF_INET;
    server->sin_port = htons(23456);
    server->sin_addr.s_addr = htonl(INADDR_ANY);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        perror("create socket failed\n");
		exit(EXIT_FAILURE);
	}
    if(bind(sockfd, (struct sockaddr*)server, sizeof(struct sockaddr_in))) {
        printf("bind failed\n");
		exit(EXIT_FAILURE);
    } 
	if(listen(sockfd, 5)) {
		perror("listen failed\n");
		exit(EXIT_FAILURE);
	}
	if((filefd = open(PIPE_PATH, O_RDONLY)) < 0) {
		perror("open failed\n");
		exit(EXIT_FAILURE);
	}
	epollfd = epoll_create(10);
	if (epollfd == -1) {
		perror("epoll_create");
		exit(EXIT_FAILURE);
	}

	ev.events = EPOLLIN;
	ev.data.fd = sockfd;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
		perror("epoll_ctl: listen_sock");
		exit(EXIT_FAILURE);
	}

	ev.events = EPOLLIN|EPOLLET;
	ev.data.fd = filefd;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, filefd, &ev) == -1) {
		perror("epoll_ctl: listen_sock");
		exit(EXIT_FAILURE);
	}

	for (;;) {
		nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
		if (nfds == -1) {
			perror("epoll_pwait");
			exit(EXIT_FAILURE);
		}

		for (n = 0; n < nfds; ++n) {
			if (events[n].data.fd == sockfd) {
				conn_sock = accept(sockfd,
						(struct sockaddr *) &local, &addrlen);
				if (conn_sock == -1) {
					perror("accept");
					exit(EXIT_FAILURE);
				}
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = conn_sock;
				if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock,
							&ev) == -1) {
					perror("epoll_ctl: conn_sock");
					exit(EXIT_FAILURE);
				}
			} else if(events[n].data.fd == filefd){
				read(filefd, buf, 1024);
				printf("PIPE %s recv: %s", PIPE_PATH, buf);
			}
		}
	}

}
