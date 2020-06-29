/*************************************************************************
    > File Name: minicrt.h
    > Author: wangjx
 ************************************************************************/
#ifndef MINICRT_H_
#define MINICRT_H_
#include "x86_64/types.h"
#include "x86_64/unistd_64.h"
#include <stdarg.h>
#ifndef NULL
#define NULL	0
#endif
#define stdin	0
#define stdout	1
#define stderr	2

void free(void *ptr);
void* malloc(unsigned size);
int mini_crt_heap_init();
int mini_crt_io_init();

int strlen(const char *str);
char *strncpy(char *dest, const char *src, size_t n);
char *strcpy(char *dest, const char *src);
int strncmp(const char *s1, const char *s2, size_t n);
int strcmp(const char *s1, const char *s2);
void mini_printf(char *msg);
int printf(const char *format, ...);
ssize_t write (int fd, const void *buf, size_t count);
int close(int);
long brk(void *addr);

#endif
