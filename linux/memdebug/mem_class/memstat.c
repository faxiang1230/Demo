#include "list.h"
#include "memstat.h"

LIST_HEAD(bfx_global_mem_class_list);

void show_meminfo(void)
{
	my_mem_class_t *alloc;
	struct list_head *pos, *tmp;
	pid_t pid = getpid();

	fprintf(stdout, "============Memory Not freed============\n");
	fprintf(stdout, "%8s %24s %24s\n", "count", "func name", "file:line");
	list_for_each_safe(pos, tmp, &bfx_global_mem_class_list) {
		alloc = list_entry(pos, my_mem_class_t, head);
		if (alloc->count)
			fprintf(stdout, "%8lu %24s %24s\n", alloc->count, alloc->funcname, alloc->info);
	}
	fprintf(stdout, "========================================\n");
}
