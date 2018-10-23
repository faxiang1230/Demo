/*************************************************************************
    > File Name: bubble.c
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2018年10月15日 星期一 14时52分49秒
 ************************************************************************/

#include<stdio.h>
void select_sort(int *array, int len) {
	int min = 0;
	printf("Before sort:\n");
	for(int x = 0; x < len; x++)
		printf("%d ", array[x]);
	printf("\n");
	for(int i = 0; i < len; i++) {
		min = i;
		for(int j = i + 1; j < len; j++) {
			if(array[min] > array[j]) {
				min = j;
			}
		}
		int tmp = array[min];
		array[min] = array[i];
		array[i] = tmp;
	}
	printf("After sort:\n");
	for(int x = 0; x < len; x++)
		printf("%d ", array[x]);
	printf("\n");
}

void main() {
	int array1[] = {1, 5, 10, 15, 32, 12, 8, 44, 0, 2};
	select_sort(array1, sizeof(array1)/sizeof(int));
}
