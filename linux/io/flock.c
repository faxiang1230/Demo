/*************************************************************************
    > File Name: flock.c
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2018年12月14日 星期五 16时21分48秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define FILE_NAME "flock.test"
void main() {
	int fd = open(FILE_NAME, O_RDONLY);
	if(flock(fd, LOCK_EX|LOCK_NB))
		exit(0);
	pause();
}
