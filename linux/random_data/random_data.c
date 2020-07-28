/*************************************************************************
    > File Name: random_data.c
    > Author: wangjx
 ************************************************************************/
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FILE_LEN (2L*1024L*1024L*1024L)

int main()
{
	int data;
	FILE *fp = NULL;
	long index = 0;

	srand(time(NULL));
	fp = fopen(argv[1], "w+");
	while (index ++ < FILE_LEN) { 
		data = rand()%(1<<15);	//控制范围
		fwrite(&data, sizeof(int), 1, fp);
	}
	fclose(fp);
	return 0;
}
