/*************************************************************************
    > File Name: array-dynamic.c
    > Author: wangjx
 ************************************************************************/
#include "array-order.h"

array_t * array_init(int size, int nmemb)
{
	array_t * arr = calloc(1, sizeof(array_t));
	if (MEMORY_CHECK(arr))
		goto err1;

	arr->cur_array = calloc(nmemb, size);
	if (MEMORY_CHECK(arr->cur_array))
		goto err2;
	arr->cur_max = nmemb;
	arr->size = size;
	
	return arr;

err2:
	free(arr);
err1:
	return NULL;
}

int array_insert(array_t* arr, array_entry e)
{
	int i = 0;
	if (arr->cur_num >= arr->cur_max) {
		return -1;
	} else {
		if (arr->cur_array[arr->cur_num - 1] < e) {
			arr->cur_array[arr->cur_num] = e;
			arr->cur_num++;	
			return 0;
		}
		int min = 0, max = arr->cur_num - 1, middle = (max + min)/2;
		while(middle < max && middle > min) {
			if (arr->cur_array[middle] > e) {
				max = middle;
			} else if (arr->cur_array[middle] < e){
				min = middle;
			}
			middle = (max + min)/2;
		}
		if (arr->cur_array[middle] < e)
			middle++;

		for (i = arr->cur_num; i > middle; i--) {
			arr->cur_array[i] = arr->cur_array[i - 1];
		}
		arr->cur_array[middle] = e;
		arr->cur_num += 1;
		return 0;
	}
	return 0;
}	
int array_remove(array_t *arr, array_entry e)
{
	int index = 0;
	index = array_find(arr, e);
	if (index >= 0) {
		for (; index < arr->cur_num - 1; index++)
			arr->cur_array[index] = arr->cur_array[index + 1];
		arr->cur_num -= 1;
		return 0;
	} else
		return -1;
}
int array_find(array_t *arr, array_entry e)
{
	int min = 0, max = arr->cur_num, middle = (max + min)/2;
	int find = 0;
	while(middle < max && middle > min) {
		if (arr->cur_array[middle] > e) {
			max = middle;
		} else if (arr->cur_array[middle] < e) {
			min = middle;
		} else {
			find = 1;
			break;
		}
		middle = (max + min)/2;
	}
	if (find)
		return middle;
	else
		return -1;
}
void array_destroy(array_t* arr)
{
	free(arr->cur_array);
	free(arr);
}
void array_show(array_t *arr)
{
	int i = 0;
	for (i = 0; i < arr->cur_num; i++)
		printf("%d ", arr->cur_array[i]);
	printf("\n");
}
