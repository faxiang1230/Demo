/*************************************************************************
    > File Name: malloc_hook_library.c
    > Author: wangjx
 ************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int malloc_hook_active = 1;
void* my_malloc_hook (size_t size, void *caller)
{
  void *result;

  // deactivate hooks for logging
  malloc_hook_active = 0;

  result = malloc(size);

  printf("caller:%p\n", caller);

  // reactivate hooks
  malloc_hook_active = 1;

  return result;
}
