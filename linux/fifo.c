/*************************************************************************
    > File Name: pipe.c
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2018年08月29日 星期三 13时59分15秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#define FIFO_P1	"hello1"
#define FIFO_P2	"hello2"

void main(){
	int r_fd = 0, w_fd = 0;
	int ret = 0;

	ret = access(FIFO_P1, F_OK);
	if(!ret)
		unlink(FIFO_P1);

	ret = mkfifo(FIFO_P1, 0664);
	if(ret < 0) {
		printf("%s fifo create failed :%s", FIFO_P1, strerror(errno));
		exit(-1);
	}

	r_fd = open(FIFO_P1, O_RDONLY);
	if(r_fd < 0) {
		printf("can't open %s error:%s", FIFO_P1, strerror(errno));
		exit(-1);
	}
	pause();

}
