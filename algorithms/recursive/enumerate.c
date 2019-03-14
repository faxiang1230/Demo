/*************************************************************************
    > File Name: enumerate.c
    > Author: wangjx
 ************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char example[] = {'3', '4', '5', '6', '7', '8'};
void fun(char *str, int offset, int max, char *res)
{
	int i = 0, j = 0, x = 0;

	if (offset == max - 1) {
		res[offset] = *str;
		for (i = 0; i < max; i++)
			printf("%c ", res[i]);
		printf("\n");
		return;
	}
	char *tmp = malloc(max - offset -1);
	for (i = 0; i < max - offset; i++) {
		res[offset] = str[i];
		x = 0;
		for (j = 0; j < max - offset - 1; x++) {
			if (x != i) {
				tmp[j] = str[x];
				j++;
			}
		}
		fun(tmp, offset + 1, max, res);
	}
	free(tmp);
}
void main()
{
	char *res = malloc(sizeof(example));
	fun(example, 0, sizeof(example), res);
}
