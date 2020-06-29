/*************************************************************************
    > File Name: max-subseq.c
    > Author: wangjx
 ************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define STR1 "ABCBDAB"
#define STR2 "BDCABA"
void lcs_length(char *x, char *y)
{
	int row = strlen(y) + 1;
	int column = strlen(x) + 1;
	char *b = calloc(1, row * column);
	char *c = calloc(1, row * column);
	int i, j;

	for (i = 1; i < column; i++)
	{
		for (j = 1; j < row; j++)
		{
			if (x[i - 1] == y[j - 1]) {
				c[i * row + j] = c[(i -1) * (row) + (j -1)] + 1;
				b[i*(row) + j] = '\\';
			} else if (c[(i - 1) * (row) + j] >= c[i * (row) + (j -1)]) {
				c[i * (row) + j] = c[(i - 1) *(row) + j];
				b[i * row + j] = '^';
			} else {
				c[i * row + j] == c[i * row + j -1];
				b[i * row + j] = '<';
			}
		}
	}


	for (i = 1; i < column; i++)
	{
		for (j = 1; j < row; j++)
		{
			printf("%4c", b[i * row + j]);
		}
		printf("\n");
	}
}

int main(int argc, char **argv)
{
	lcs_length(STR1, STR2);
}
