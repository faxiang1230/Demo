/*************************************************************************
    > File Name: bubble.c
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2018年10月15日 星期一 14时52分49秒
 ************************************************************************/

#include<stdio.h>
void sort(int* array, int len) {
	for (int i = 1; i < len; i++) { // 默认第一个是有序的

		int temp = array[i]; // 拿出要插入的数据

		int j = i;
		while (j > 0 && temp < array[j - 1]) {
			array[j] = array[j - 1];
			j--;
		}
		array[j] = temp;
	}
	printf("After sort:\n");
	for(int x = 0; x < len; x++)
		printf("%d ", array[x]);
	printf("\n");
}
void bubble_sort(int *array, int len) {
	int temp, j;
	printf("Before sort:\n");
	for(int x = 0; x < len; x++)
		printf("%d ", array[x]);
	printf("\n");

	for(int i = 1; i < len; i++) {
		temp = array[i];
		j = i;
		while(j > 0 && array[j - 1] > temp ) { 
			array[j] = array[j - 1];
			j--;
		}
		array[j] = temp;
	}

	printf("After sort:\n");
	for(int x = 0; x < len; x++)
		printf("%d ", array[x]);
	printf("\n");
}

void main() {
	int array1[] = {1, 5, 10, 15, 32, 12, 8, 44, 0, 2};
	bubble_sort(array1, sizeof(array1)/sizeof(int));
	sort(array1, sizeof(array1)/sizeof(int));
}
