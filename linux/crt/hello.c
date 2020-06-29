/*************************************************************************
    > File Name: hello.c
    > Author: wangjx
 ************************************************************************/
//#include <stdio.h>
#include "minicrt.h"
#include <fcntl.h>
int main(int argc, char **argv)
{
	int i = 0;
	char **v = malloc(argc * sizeof(char *));
	int fd = 0;
	for (i = 0; i < argc; i++)
		printf("%s ", argv[i]);

	fd = open("1234.txt", O_CREAT|O_RDWR, 0666);
	for (i = 0; i < argc; i++) {
		v[i] = malloc(strlen(argv[i]) + 1);
		if (v[i] == NULL) {printf("NULL malloc addr\n");continue;}
		strcpy(v[i], argv[i]);
		write(fd, v[i], strlen(v[i]));
	}
	close(fd);
	printf("str:%s\nnum:%d %d %d\n", "nihao", 13, 5, 11);
	return 0;
}
