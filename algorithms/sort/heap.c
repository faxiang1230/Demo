/*************************************************************************
    > File Name: bubble.c
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2018年10月15日 星期一 14时52分49秒
 ************************************************************************/

#include<stdio.h>
void init(int* array, int len);
void adjust(int* array, int n, int size);
void showArray(int *array, int offset, int len) {
	for(int i = offset; i < offset + len; i++)
		printf("%d ", array[i]);

	printf("\n");
}
void sort(int* array, int len) {
	init(array, len);
	showArray(array, 0, len);
	for (int i = 1; i < len; i++) {
		int temp = array[0];
		int end = len - i;
		array[0] = array[end];
		array[end] = temp;
		adjust(array, 0, end);
		showArray(array, 0, end);
	}
	showArray(array, 0, len);
}
void init(int* array, int len) {
	for (int i = len / 2 - 1; i >= 0; i--) {
		adjust(array, i, len);
	}
}



void adjust(int* array, int n, int size) {
	int temp = array[n];
	int child = n * 2 + 1;
	while (child < size) {
		if (child + 1 < size && array[child + 1] > array[child]) {
			child++;
		}

		if (array[child] > temp) {
			array[n] = array[child];
			n = child;
			child = n * 2 + 1;
		} else {
			break;
		}
	}
	array[n] = temp;
}

int main() {
	int array1[] = {1, 5, 10, 15, 32, 12, 8, 44, 0, 2};
	sort(array1, sizeof(array1)/sizeof(int));
	return 0;
}
