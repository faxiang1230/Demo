/*************************************************************************
    > File Name: ldpreload.c
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2018年08月14日 星期二 11时19分20秒
 ************************************************************************/
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if( strcmp(argv[1], "test") )
	{
		printf("Incorrect password\n");
	}
	else
	{
		printf("Correct password\n");
	}
	return 0;
}
