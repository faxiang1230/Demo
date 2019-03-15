/*************************************************************************
    > File Name: prio-queue.c
    > Author: wangjx
	> 优先级队列:入队，出队
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NUM	8
typedef struct queue_s {
	int num;
	int array[MAX_NUM + 1];
}queue_t;
void swap(int *v1, int *v2)
{
	int temp = *v1;
	*v1 = *v2;
	*v2 = temp;
}
int adjust(int *arr, int offset, int size)
{
	int node = size / 2, swap_index = 1; 
	for (; node > 0 && swap_index; node--) {
		swap_index = 0;
		if (arr[node] < arr[node * 2])
			swap_index = node * 2;
		if (((node * 2 + 1) <= size) && arr[node] < arr[node * 2 + 1]) {
			if (swap_index) {
				if (arr[swap_index] < arr[node * 2 + 1])
					swap_index = node * 2 + 1;
			} else
				swap_index = node * 2 + 1;
		}
		if (swap_index)
			swap(&arr[node], &arr[swap_index]);
	}
}
queue_t *queue_init(void)
{
	queue_t *q = calloc(1, sizeof(queue_t));

	return q;
}
void queue_destroy(queue_t *q)
{
	free(q);
}
void queue_show(queue_t * q)
{
	int index = 0;
	for (index = 1; index <= q->num; index++)
		printf("%d ", q->array[index]);
	printf("\n");
}
int enqueue(queue_t *q, int val)
{
	if (q->num >= MAX_NUM)
		return -1;
	q->num++;
	q->array[q->num] = val;
	adjust(q->array, 1, q->num);

	return 0;
}
int dequeue(queue_t *q)
{
	if (q->num <= 0)
		return -1;
	int i = 0, swap_index = 1, ret;
	ret = q->array[1];
	swap(&(q->array[1]), &(q->array[q->num]));
	q->num--;
	for (i = 1; i < q->num/2 && swap_index;) {
		swap_index = 0;
		if (q->array[i] < q->array[i * 2])
			swap_index = i * 2;
		if (((i * 2 + 1) <= q->num) && q->array[i * 2] < q->array[i * 2 + 1])
			swap_index = i * 2 + 1;
		if (swap_index) {
			swap(&(q->array[i]), &(q->array[swap_index]));
			i = swap_index;
		}
	}
	return ret;
}
void main()
{
	queue_t *q = queue_init();
	enqueue(q, 1);
	enqueue(q, 20);
	enqueue(q, 19);
	enqueue(q, 27);
	enqueue(q, 38);
	enqueue(q, 2);
	enqueue(q, 7);
	enqueue(q, 17);
	enqueue(q, 45);
	enqueue(q, 68);
	queue_show(q);
	printf("%d\n", dequeue(q));
	enqueue(q, 3);
	queue_show(q);
	printf("%d\n", dequeue(q));
	queue_show(q);
	printf("%d\n", dequeue(q));
	queue_show(q);
	printf("%d\n", dequeue(q));
	queue_show(q);
	printf("%d\n", dequeue(q));
	queue_show(q);
	enqueue(q, 34);
	queue_show(q);
	queue_destroy(q);
}
