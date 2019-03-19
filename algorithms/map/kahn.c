#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array-queue.c"

typedef struct edge{
	//int vec;
	int weight;
}edge_t;
#define MAX	65535
#define VEC_NUM 9
edge_t *map_init(int vec_num)
{
	edge_t *m = calloc(vec_num, sizeof(edge_t) * vec_num);
    return m;
}
void map_insert(edge_t *e, int vec1, int vec2)
{
    e[vec1 * VEC_NUM + vec2].weight = 1;
}
void kahn(edge_t *e)
{
    edge_t (*m)[VEC_NUM] = (edge_t (*)[VEC_NUM])e;
    int i = 0, j = 0, find = 1, x = 0;
    int *processed = calloc(VEC_NUM, sizeof(int));
    while (find) {
        find = 0;
        for (i = 1; i < VEC_NUM; i++) {
            for(j = 1; j < VEC_NUM; j++) {
                if (m[i][j].weight) {
                    break;
                }
            }
            if (j < VEC_NUM)
                continue;
            if (!processed[i]) {
                processed[i] = 1;
                find = 1;
                printf("%d ", i);
            }
            for (x = 1; x < VEC_NUM; x++){
                if (m[x][i].weight)
                    m[x][i].weight = 0;
            }
        }
    }
}
void main()
{
    edge_t *map = map_init(VEC_NUM);
    map_insert(map, 1, 2);
    //map_insert(map, 2, 1);
    map_insert(map, 1, 3);
    map_insert(map, 2, 4);
    map_insert(map, 2, 3);
    map_insert(map, 2, 5);
    map_insert(map, 3, 5);
    map_insert(map, 3, 7);
    map_insert(map, 3, 8);
    map_insert(map, 4, 5);
    map_insert(map, 5, 6);
    map_insert(map, 7, 8);
    kahn(map);
}
