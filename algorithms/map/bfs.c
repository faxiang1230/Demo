#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array-queue.c"

typedef struct edge{
	//int vec;
	int weight;
}edge_t;
#define MAX	65535
edge_t *map_init(int vec_num)
{
	edge_t *m = calloc(vec_num, sizeof(edge_t) * vec_num);
    return m;
}
void map_insert(edge_t *e, int vec1, int vec2, int weight)
{
    e[vec1 * 6 + vec2].weight = weight;
}
void bfs(edge_t *m, queue_t *q, int end, int *p, int *parent)
{
    edge_t (*map)[6] = (edge_t (*)[6])m;
    int i = 0, index = 0;
    queue_t *q1 = queue_init(6);
    while((index = dequeue(q)) != -1) {
        if (index == end) { 
            for (;index;) {
                printf("%d <- ", index);
                index = parent[index];
            }
            printf("\n");
            return;
        }
        for (i = 0; i < 6; i++) {
            if (map[index][i].weight && !p[i]) {
                enqueue(q1, i);
                p[i] = 1;
                parent[i] = index;
            }
        }
    }
    bfs(m, q1, end, p, parent);
    queue_destroy(q1);
}
void path_find(edge_t *m, int begin, int end)
{
    int *processed = calloc(6, sizeof(int));
    int *parent = calloc(6, sizeof(int));
    edge_t (*map)[6] = (edge_t (*)[6])m;
    queue_t *q = queue_init(6);
    int i = 0;
    for (i = 0; i < 6; i++) {
        if (map[begin][i].weight) {
            enqueue(q, i);
            parent[i] = begin;
            processed[i] = 1;
        }
    }
    processed[begin] = 1;
    bfs(m, q, end, processed, parent);
}
void main()
{
    edge_t *map = map_init(6);
    map_insert(map, 0, 1, 5);
    map_insert(map, 0, 2, 2);
    map_insert(map, 2, 1, 8);
    map_insert(map, 1, 3, 4);
    map_insert(map, 1, 4, 2);
    map_insert(map, 2, 4, 7);
    map_insert(map, 3, 4, 6);
    map_insert(map, 3, 5, 3);
    map_insert(map, 4, 5, 1);
    path_find(map, 0, 5);
}
