/*************************************************************************
    > File Name: notify.c
    > Author: wangjx
 ************************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <assert.h>

int main(int argc, char **argv)
{
	int inotify_fd, wd = 0, ret= 0;
	struct inotify_event *event;
	char buf[1024];
   	inotify_fd = inotify_init();
	assert(inotify_fd >= 0);
	wd = inotify_add_watch(inotify_fd, "/proc/", IN_CREATE|IN_DELETE);
	if (wd < 0)
		perror("add watch");
	while (1) {
		ret = read(inotify_fd, buf, sizeof(buf));
		if (ret > 0) {
			event = (struct inotify_event *)buf;
			printf("name %s mask:%x\n", event->name, event->mask);
		} else
			perror("read");
	}
}
