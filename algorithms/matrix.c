/*************************************************************************
    > File Name: matrix.c
    > Author: wangjx
 ************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#define ROW	4
#define COLOUM	5
void **matrix_malloc(int row, int coloum)
{
	int index = 0;
	void *ptr = malloc(row * sizeof(void*));
	void *ptr1 = malloc(row*coloum);
	for (; index < row; index++)
	{
		ptr[index] = ptr1 + (row * index);
	}
	return &ptr;
}
int main(int argc, char **argv)
{
	int x = 0, y = 0;
	char *ptr[ROW] = matrix_malloc(ROW, COLOUM);
	while(x++ < ROW) {
		y = 0;
		while(y++ < COLOUM) {
			*(ptr[ROW]+y) = x+y;
		}
	}
	x = 0, y = 0;
	while(x++ < ROW) {
		y = 0;
		while(y++ < COLOUM) {
			printf("%d\n", *(ptr[ROW]+y));
		}
	}

	return 0;
}
