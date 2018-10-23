/*************************************************************************
    > File Name: bubble.c
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2018年10月15日 星期一 14时52分49秒
 ************************************************************************/

#include<stdio.h>
void sort(int* array, int len) {

	for (int step = len / 2; step > 0; step /= 2) {

		for (int i = step; i < len; i++) {

			int temp = array[i];

			int j = i;

			while (j >= step && temp < array[j - step]) {

				array[j] = array[j - step];

				j -= step;

			}

			array[j] = temp;

		}

	}

	printf("After sort:\n");
	for(int x = 0; x < len; x++)
		printf("%d ", array[x]);
	printf("\n");
}
void shell_sort(int *array, int len) {
	int temp, j;
	printf("Before sort:\n");
	for(int x = 0; x < len; x++)
		printf("%d ", array[x]);
	printf("\n");

	for(int step = len/2; step > 0; step = step/2) {
		for(int i = step; i < len; i++) {
			temp = array[i];
			j = i;
			while(j >= step && array[j] < array[j - step]) { 
				array[j] = array[j - step];
				j -= step;
			}
			array[j] = temp;
		}
	}

	printf("After sort:\n");
	for(int x = 0; x < len; x++)
		printf("%d ", array[x]);
	printf("\n");
}

void main() {
	int array1[] = {1, 5, 10, 15, 32, 12, 8, 44, 0, 2};
	shell_sort(array1, sizeof(array1)/sizeof(int));
	//sort(array1, sizeof(array1)/sizeof(int));
}
