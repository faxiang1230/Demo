/*************************************************************************
    > File Name: dynamic.c
    > Author: wangjx
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void dump(int *src, int len,int *record) {
	int index = 0;
	for (index = 0; index < len; index++) {
		if (record[index])
			printf("%d ", src[index]);
	}
	printf("\n");
}
int bag(int *src, int offset, int len, int *record, int res, int max)
{
	if (offset >= len || res >= max) {
		dump(src, len, record);
		return;
	}

	memset(record + offset, 0, len - offset);
	bag(src, offset + 1, len, record, res, max);

	if (res + src[offset] <= max) {
		record[offset] = 1;
		bag(src, offset + 1, len, record, res + src[offset], max);
		//dump(src, len, record);
		return;
	}
		
}
void main()
{
	int array[] = {2, 2 ,4 ,6 ,3};
	int *result = malloc(sizeof(array));
	memset(result, 0, sizeof(array));
	bag(array, 0, 5, result, 0, 9);
}
