/*************************************************************************
    > File Name: kmp.c
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2018年10月11日 星期四 18时45分05秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void Next(char *s, int *next) {
	int len = 0;
	len = strlen(s);

	int k = -1, j = 0;
	next[0] = -1;

	while(j < len - 1) {
		if(k == -1 || s[j] == s[k]) {
			next[++j] = ++k;
		} else {
			k = next[k];
		}
	}
}


void main(int argc, char *argv[]) {
	if(argc < 3) {
		perror("Usage: kmp mainString subString\n");
		exit(-1);
	}
	int mainlen = strlen(argv[1]);
	int sublen = strlen(argv[2]);
	printf("mainlen:%d sublen:%d\n", mainlen, sublen);
	int *next = (int *)malloc(sublen * sizeof(int));
	int i = 0,j = 0;
	Next(argv[2], next);

	for(int tmp = 0; tmp < sublen; tmp++)
		printf("%d ", next[tmp]);

	while(i < mainlen && j < sublen) {
		if(j == -1 || argv[1][i] == argv[2][j]) {
			i++;
			j++;
		} else {
			j = next[j];
		}
	}

	printf("mainlen:%d i:%d  sublen:%d j:%d\n", mainlen, i, sublen, j);
	if(j == sublen) {
		printf("substring index %d\n", i - j);
	}
	else {
		printf("not substring\n");
	}

	free(next);
}
