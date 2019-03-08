/*************************************************************************
    > File Name: array-merge.c
    > Author: wangjx
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int array1[] = {1, 5, 9, 10, 30, 50};
int array2[] = {2, 5, 18, 20};

void main()
{
	int s = sizeof(array1) + sizeof(array2);
	printf("array1 %d array2 %d total %d\n", sizeof(array1), sizeof(array2), s);
	int *array = malloc(s);
	int i = 0, i1 = 0, i2 = 0;

	while(1) {
		if (i2 >= sizeof(array2)/sizeof(int) || i1 >= sizeof(array1)/sizeof(int))
			break;
		if (array1[i1] >= array2[i2]) {
			array[i++] = array2[i2++];
		} else
			array[i++] = array1[i1++];
	}

	if (i2 == sizeof(array2)/sizeof(int)) {
		for (; i1 < sizeof(array1)/sizeof(int); i1++) {
			array[i++] = array1[i1];
		}
	} else {
		for (; i2 < sizeof(array2)/sizeof(int); i2++) {
			array[i++] = array2[i2];
		}
	}
	for (i = 0; i < s/sizeof(int); i++)
		printf("%d ", array[i]);
	printf("\n");
}
