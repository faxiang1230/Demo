/*************************************************************************
    > File Name: array-dynamic.c
    > Author: wangjx
 ************************************************************************/
#include "array.h"
#if 0
typedef int array_entry;                                                           
typedef struct array_s {                                                        
	int last_max, last_num, cur_max, cur_num;                                   
	array_entry * last_array, *cur_array;                                       
	int size;                                                                   
}array_t;                                                                       
#endif
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
	if (unlikely(arr->cur_num == arr->cur_max - 1)) {
		arr->last_array = arr->cur_array;
		arr->cur_array = calloc(arr->cur_max * 2, arr->size);
		if (MEMORY_CHECK(arr->last_array))
			goto err1;
		arr->last_max = arr->cur_max;
		arr->last_num = arr->cur_num;
		arr->cur_max = arr->cur_max * 2;
		//printf("%s max_cur is %d\n", arr->last_max);
		return array_insert(arr, e);
	}
	arr->cur_array[arr->cur_num] = e;
	arr->cur_num++;

	if (arr->last_array) {
		for (i = 0; i < 3; i++) {
			if (arr->last_num >= 0) {
				arr->cur_array[arr->cur_num++] = arr->last_array[arr->last_num];
				arr->last_num--;
			} else {
				free(arr->last_array);
				arr->last_array = NULL;
				break;
			}
		}
	}
	return 0;
err1:
	return -1;
}	
static int array_remove_entry(array_entry *entrys, int max, array_entry e)
{
	int i = 0;
	for (i = 0; i < max; i++) {
		if (entrys[i] == e)
			break;
	}
	if (i != max) {
		for (;i < max; i++) {
			entrys[i] = entrys[i + 1];
		}
		return 0;
	} else
		return -1;
}
static int array_find_entry(array_entry *entrys, int max, array_entry e) {
	int i = 0;
	for (i = 0; i < max; i++) {
		if (entrys[i] == e)
			return i;
	}
}
int array_delete(array_t* arr, array_entry e) {
	int ret = 0;
	if (array_remove_entry(arr->cur_array, arr->cur_num, e) < 0) {
		ret = array_remove_entry(arr->last_array, arr->last_num, e);
		if (ret < 0)
			return -1;
		else {
			arr->last_num--;
			if (arr->last_num == 0) {
				free(arr->last_array);
				arr->last_array = NULL;
			}
			return 0;
		}
	} else {
		arr->cur_num--;
		return 0;
	}
}                                   
void array_destroy(array_t* arr) {
	free(arr->cur_array);
	free(arr->last_array);
	free(arr);
}
