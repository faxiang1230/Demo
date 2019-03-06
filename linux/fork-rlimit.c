/*************************************************************************
    > File Name: fork-rlimit.c
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
	> Created Time: 2018年12月13日 星期四 20时20分02秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

void bfx_resource_set(void)
{
	struct rlimit rlim, rlim_new;
	if (getrlimit(RLIMIT_CORE, &rlim)==0) {
		printf("limit cur:%lu max:%lu\n",rlim.rlim_cur, rlim.rlim_max);
		rlim_new.rlim_cur = rlim_new.rlim_max = RLIM_INFINITY;
		if (setrlimit(RLIMIT_CORE, &rlim_new)!=0) {
			rlim_new.rlim_cur = rlim_new.rlim_max = rlim.rlim_max;
			(void) setrlimit(RLIMIT_CORE, &rlim_new);
		}                           
	}   
}


void main() {
	pid_t pid;
	bfx_resource_set();
	pid = fork();
	if(pid == 0) {
		struct rlimit rlim;
		getrlimit(RLIMIT_CORE, &rlim);
		printf("limit cur:%lu max:%lu\n",rlim.rlim_cur, rlim.rlim_max);
	}
}
