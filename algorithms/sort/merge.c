/*************************************************************************
    > File Name: bubble.c
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2018年10月15日 星期一 14时52分49秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
static int *address = NULL;
void merge(int *array, int left, int right) {
	int middle = (right + left) / 2;
	int begin1 = left, begin2 = middle + 1;
	int k = 0;
	printf("Befor merge left:%d right:%d\n", left, right);
	for(int x = left; x <= right; x++)
		printf("%d ", array[x]);
	printf("\n");
	while(begin1 <= middle && begin2 <= right) {
		if(array[begin1] > array[begin2]){
			address[k++] = array[begin2++];
		} else{
			address[k++] = array[begin1++];
		}
	}
	while(begin1 <= middle) {
		address[k++] = array[begin1++];
	}
	while(begin2 <= right) {
		address[k++] = array[begin2++];
	}
	for(int i = 0; i < k; i++) {
		array[left + i] = address[i];;
	}
	printf("After merge left:%d right:%d\n", left, right);
	for(int x = left; x <= right; x++)
		printf("%d ", array[x]);
	printf("\n");
}
void merge_sort(int *array, int left, int right) {
	int middle;
	printf("Befor sort left:%d right:%d\n", left, right);
	for(int x = left; x <= right; x++)
		printf("%d ", array[x]);
	printf("\n");

	middle = (right + left) / 2;
	if(left < right) {
		if(middle >= left && middle <= right) {
			merge_sort(array, left, middle);
			merge_sort(array, middle + 1, right);
			merge(array, left, right);
		}
	}

	printf("After sort left:%d right:%d\n", left, right);
	for(int x = left; x <= right; x++)
		printf("%d ", array[x]);
	printf("\n");
}
void main() {
	int array1[] = {1, 5, 10, 15, 32, 12, 8, 44, 0, 2};
	address = (int *)malloc(sizeof(array1));
	merge_sort(array1, 0, sizeof(array1)/sizeof(int) - 1);
	free(address);
}
