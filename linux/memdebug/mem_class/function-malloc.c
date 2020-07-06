#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#define MEM_DEBUG_MAGIC			0xf06f19cdf06f19cd

LIST_HEAD(bfx_global_mem_class_list);

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define ADDRESS __FILE__ ":" TOSTRING(__LINE__)

typedef struct {
	char *info;
	const char *funcname;
	unsigned long count;
	struct list_head head;
}my_mem_class_t;

typedef struct {
	unsigned long magic;
	void *mem_class;
} my_mem_header_t;
static inline void bfx_list_add_cas(struct list_head *new,
		struct list_head *prev,
		struct list_head *next)
{
	int retry = 0;
	struct list_head *tmp = new;
again:
	new->next = next;
	new->prev = prev;
	tmp = __sync_val_compare_and_swap(&prev->next, (long)next, (long)new);
	if (tmp != new && retry <=20)   {retry++; goto again;}
	next->prev = new;
	if (retry > 20) perror("somebody add list failedn");
}
static inline void atomic_cas_dec(unsigned long *addr) {
	unsigned long tmp, oldcount ,newcount;
inc_again:
	oldcount = newcount = *addr;
	newcount--;
	tmp = __sync_val_compare_and_swap(addr, oldcount, newcount);
	if (tmp != oldcount) goto inc_again;
}
static inline void atomic_cas_inc(unsigned long *addr) {
	unsigned long tmp, oldcount ,newcount;
dec_again:
	oldcount = newcount = (unsigned long)*addr; newcount++;
	tmp = __sync_val_compare_and_swap(addr, oldcount, newcount);
	if (tmp != oldcount) goto dec_again;
}
void *bfx_malloc(size_t x)       
{                      
	void *ptr = malloc(sizeof(my_mem_header_t) + (x));    
	static my_mem_class_t my_mem_class = {					
		.info = ADDRESS, .funcname = __func__, .count = 0,	
		.head = {&(my_mem_class.head), &(my_mem_class.head)}	
	};			
	void *retaddr = __builtin_return_address(0);
	fprintf(stderr, "malloc addr:%p\n", retaddr);				
	if (list_empty(&my_mem_class.head))	
		bfx_list_add_cas(&my_mem_class.head, &bfx_global_mem_class_list, bfx_global_mem_class_list.next);	
	if (ptr) {				
		my_mem_header_t *hdr = (my_mem_header_t *)ptr;	
		atomic_cas_inc(&my_mem_class.count);  									
		hdr->mem_class = &my_mem_class;					
		hdr->magic = ((unsigned long)hdr ^ MEM_DEBUG_MAGIC);			
		ptr = hdr + 1;               
	}								
	return ptr;
}
void bfx_free(void *x)
{                      	
	if (x) { 
		my_mem_header_t *hdr = (my_mem_header_t*)(((unsigned long)x)-sizeof(my_mem_header_t));        
		if (hdr->magic != ((unsigned long)hdr ^ MEM_DEBUG_MAGIC)) {				
			fprintf(stderr, "MAGIC ERROR:%p %s %d\n", x, __FILE__, __LINE__); free(x);				
		} else {												
			my_mem_class_t *my_class = hdr->mem_class;				
			atomic_cas_dec(&my_class->count);						
			free(hdr);											
		}
	}
}
void *lib(int x)
{
	return malloc(x);
}
void funb(int x)
{
	printf("%s %p\n", __func__, funb);
	bfx_malloc(x);
}
void funa(int x)
{
	printf("%s %p\n", __func__, funa);
	bfx_malloc(x);
}
void main()
{

	printf("%s %p\n", __func__, main);
	void *ptr1, *ptr2;
	//malloc(32);
	funa(53);
	funb(35);
	funb(35);
	funa(64);
	funa(64);

	ptr1 = bfx_malloc(127);
	ptr1 = bfx_malloc(78);
	bfx_free(ptr1);

	ptr2 = lib(44);
	bfx_free(ptr2);
	ptr1 = bfx_malloc(78);
}
