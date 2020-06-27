/*************************************************************************
    > File Name: minicrt.h
    > Author: wangjx
 ************************************************************************/
#ifndef MINICRT_H_
#define MINICRT_H_
#include "x86_64/types.h"
#include "x86_64/unistd_64.h"
#define NULL	0

void free(void *ptr);
void* malloc(unsigned size);
int mini_crt_heap_init();

int strlen(char *str);
void mini_printf(char *msg);
ssize_t write (int fd, const void *buf, size_t count);
#endif
