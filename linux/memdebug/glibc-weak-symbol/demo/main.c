#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
extern void *__libc_malloc(size_t size);

extern int malloc_hook_active;

extern void* my_malloc_hook (size_t size, void *caller);
void* malloc (size_t size)
{
  void *caller = __builtin_return_address(0);
  if (malloc_hook_active)
    return my_malloc_hook(size, caller);
  return __libc_malloc(size);
}
int main(int argc, char **argv)
{
	malloc(13);
	malloc(33);
	return 0;
}
