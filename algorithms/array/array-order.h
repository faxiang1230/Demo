/*************************************************************************
    > File Name: array.h
    > Author: wangjx
 ************************************************************************/
#ifndef __ORDER_ARRAY_H
#define __ORDER_ARRAY_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifndef likely
# define likely(x)		__builtin_expect(!!(x), 1)
#endif

#ifndef unlikely
# define unlikely(x)		__builtin_expect(!!(x), 0)
#endif

typedef int array_entry;
typedef struct array_s {
	int cur_max, cur_num;
	array_entry *cur_array;
	int size;
}array_t;
array_t * array_init(int size, int nmemb);
int array_insert(array_t *arr, array_entry e);
int array_remove(array_t *arr, array_entry e);
int array_find(array_t *arr, array_entry e);
void array_destroy(array_t *arr);
void array_show(array_t *arr);
static inline int MEMORY_CHECK(void *x)
{
	if (!x)	 {				
		printf("%s:%d memory alloc failed %s\n",	
				__func__, __LINE__, strerror(errno));
		return 1;	
	} else				
		return 0;		
}
#endif /*__ORDER_ARRAY_H*/
