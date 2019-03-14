/*************************************************************************
    > File Name: list-test.c
    > Author: wangjx
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct queue{
	int *arr;
	int max;
	int head, tail;
}queue_t;
queue_t *queue_init(int num)
{
	queue_t *q = calloc(1, sizeof(queue_t));
	q->arr = calloc(num, sizeof(int));
	q->head = q->tail = 0;
	q->max = num;
	return q;
}
int queue_in(queue_t *q, int val)
{
	if ((q->head + 1) % q->max == q->tail) {
		return -1;	
	}

	q->arr[q->head] = val;
	q->head = (q->head + 1) % q->max;
	return 0;
}
int queue_out(queue_t *q)
{
	if (q->head == q->tail)
		return -1;	
	int val = q->arr[q->tail];
	q->tail = (q->tail + 1) % q->max;
	return val;
}
void queue_destroy(queue_t *q)
{
	free(q->arr);
	free(q);
}
void list_show(queue_t *q)
{
	int index = q->tail;
	printf("current queue:\n");
	for (; index != q->head; index = (index + 1)%q->max)
		printf("%d ", q->arr[index]);
	printf("\n");
}
void main()
{
	queue_t *q = queue_init(4+1);

	queue_in(q, 1);
	queue_in(q, 10);
	queue_in(q, 3);
	queue_in(q, 25);
	queue_in(q, 28);
#if 0
	queue_in(&q, 17);
	queue_in(&q, 23);
	queue_in(&q, 38);
	queue_in(&q, 77);
	queue_in(&q, 15);
	queue_in(&q, 18);
#endif
	list_show(q);

	printf("%d \n", queue_out(q));
	printf("%d \n", queue_out(q));
	printf("%d \n", queue_out(q));
	printf("%d \n", queue_out(q));
	printf("%d \n", queue_out(q));
	printf("%d \n", queue_out(q));
	printf("%d \n", queue_out(q));
	printf("%d \n", queue_out(q));
	printf("%d \n", queue_out(q));
	list_show(q);

	queue_destroy(q);
}
