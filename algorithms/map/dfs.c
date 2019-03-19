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
void dfs(edge_t *m, int begin, int end, int *p, int *parent)
{
    edge_t (*map)[6] = (edge_t (*)[6])m;
    int i = 0;
    if (begin == end) {
        printf("%d <- ", end);
        for (i = end; i;) {
            printf("%d <- ", parent[i]);
            i = parent[i]; 
        }
        printf("\n");
        return;
    }
    for (i = 0; i < 6; i++) {
        if (map[begin][i].weight && !p[i]) {
            p[i] = 1;
            parent[i] = begin;
            dfs(m, i, end, p, parent);
            p[i] = 0;
            //printf("%d <- ", i);
        }
    }
}
void path_find(edge_t *m, int begin, int end)
{
    int *processed = calloc(6, sizeof(int));
    int *parent = calloc(6, sizeof(int));
    edge_t (*map)[6] = (edge_t (*)[6])m;
    int i = 0;
    for (i = 0; i < 6; i++) {
        if (map[begin][i].weight) {
            memset(processed, 0, 6 * sizeof(int));
            memset(parent, 0, 6 * sizeof(int));
            processed[i] = 1;
            parent[i] = begin;
            dfs(m, i, end, processed, parent);
            processed[i] = 0;
            //printf("%d <- ", i);
        }
    }
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
