#include <event.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

static void pipe_fun(int e, short s, void *a)
{
	char msg[128];
	read(e, msg, 128);
	printf("pipe_fun %s\n", msg);
}

main()
{
	struct event fir, sec;
	int fd2;
	fd2 = open("hello", O_RDWR);
	if(fd2 < 0) {
		printf("%s\n", strerror(fd2));
		exit(-1);
	}
	assert(fd2 > 0);
	event_init();
	event_set(&sec, fd2, EV_READ | EV_PERSIST, pipe_fun, NULL);

	event_add(&sec, NULL);

	event_dispatch();
}
