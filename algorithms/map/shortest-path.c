#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void dijkstra(edge_t *e, int begin, int end)
{
    int i = 0;
    int *costs = calloc(6, sizeof(int));
    int *parent = calloc(6, sizeof(int));
    int *processed = calloc(6, sizeof(int));
    int min = MAX,min_index = -1, new_cost = 0;
    for (i = 0; i < 6; i++) {
        if (e[begin * 6 + i].weight > 0)
            costs[i] = e[begin * 6 + i].weight;
        else
            costs[i] = MAX;
    }
again:
    min = MAX,min_index = -1;
    for(i = 0; i < 6; i++) {
        if (min > costs[i] &&processed[i] == 0) {
            min = costs[i];
            min_index = i;
        }
    }
    if(min == MAX && min_index < 0) {
        goto out;
    }
    for (i = 0; i < 6; i++) {
        if (e[min_index * 6 + i].weight == 0)
            continue;
        new_cost = e[min_index * 6 + i].weight + min;
        if (new_cost < costs[i]) {
            costs[i] = new_cost;
            parent[i] = min_index;
        }
    }
    processed[min_index] = 1;
    goto again;
out:
    for (i = end; i != begin;) {
        printf("%d <- ", i);
        i = parent[i];
    }
    printf("%d\n", begin);
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
    dijkstra(map, 0, 5);
}
