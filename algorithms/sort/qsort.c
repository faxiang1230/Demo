/*************************************************************************
    > File Name: qsort.c
    > Author: wangjx
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void swap(int *x, int *y)
{
	int tmp = *x;
	*x = *y;
	*y = tmp;
}
void quick_sort(int *array, int left, int right)
{
	int i = left + 1, j = right;
	if (left >= right)
		return;
	swap(array + left, array + (left + right)/2);

	int st = array[left];
	while(i <= j) {
		while (array[j] > st && i < j)
			j--;
		while (array[i] < st && i < j)
			i++;
		swap(array + i, array + j);
		if (i >= j)
			break;
	}
	swap(array + left, array + i);
	quick_sort(array, left, i - 1);
	quick_sort(array, i + 1, right);
}

void main()
{
	int i = 0;
	int a[] = {6, 7, 9, 1, 10, 5, 2, 18, 20, 30};
	quick_sort(a, 0 , sizeof(a)/sizeof(int) - 1);
	for ( i = 0; i < sizeof(a)/sizeof(int); i++)
		printf("%d ", a[i]);
	printf("\n");
}
