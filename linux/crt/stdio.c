/*************************************************************************
    > File Name: stdio.c
    > Author: wangjx
 ************************************************************************/
#include "minicrt.h"
//TODO:check parameter is NULL
int mini_crt_io_init()
{
	return 0;
}
int strcmp(const char *s1, const char *s2){
	while (*s1 && *s2) {
		if (*s1 > *s2)
			return 1;
		else if(*s2 < *s2)
			return -1;
		else
			s1++,s2++;
	}
	if (*s1 > *s2)
		return 1;
	else if (*s2 < *s2)
		return -1;
	else
		return 0;
}
int strncmp(const char *s1, const char *s2, size_t n)
{
	int i = 0;
	for (; i < n; i++,s1++,s2++) {
		if (*s1 > *s2)
			return 1;
		else if(*s2 < *s2)
			return -1;
	
	}

	return 0;
}
char *strcpy(char *dest, const char *src)
{
	char *ptr = dest;
	while (*src) {
		*dest = *src;
		dest++;
		src++;
	}
	return ptr;
}
char *strncpy(char *dest, const char *src, size_t n)
{
	size_t i;

	for (i = 0; i < n && src[i] != '\0'; i++)
		dest[i] = src[i];
	for ( ; i < n; i++)
		dest[i] = '\0';

	return dest;

}
int strlen(char *str)
{
	int len = 0;
	while(*str) {str++;len++;}
	return len;
}
#if 0
int mywrite(int fd, char *buf, long len)
{
	long ret = 0;
	asm volatile("movq $1,%%rdi\n"
			"movq %1,%%rsi\n"
			"movq %2,%%rdx\n"
			"movq $1,%%rax\n"
			"syscall\n"
			"movq %%rax,%0":"=r"(ret):"g"(buf),"g"(len));
	return ret;
}
#endif
void mini_printf(char *msg)
{
	write(1, msg, strlen(msg));
	//mywrite(1, msg, strlen(msg));
}
#define va_list (char*);
#define va_start(ap, arg)	(ap=(va_list)&arg+sizeof(arg))
#define va_arg(ap, t)	(*(t*)((ap+=sizeof(t)) - sizeof(t)))
#define va_end(ap)	(ap=(va_list)0)
char *itoa(int n, char *str, int radix)
{
	char digit[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char *p = str;
	char *head = str;
	if (*p || radix < 2 || radix > 36)
		return p;
	if (radix != 10 || n < 0)
		return p;

	if (n == 0)
	{
		*p++ = '\0';
		*p = '\0';
		return p;
	}
}
int putchar(int fd, int c)
{
	return write(fd, &c, sizeof(c));
}
int puts(int fd, const char *s)
{
	return write(fd, s ,strlen(s));
}

int dprintf(int fd, const char *format, va_list ap)
{
	int translating = 0;
	int ret = 0;
	const char *p = 0;
	for (p = format; *p != 0; p++) {
		switch (*p)
		{
			case '%':
				if (!translating)
					translating = 1;
				else {
					write(fd, "%", strlen("%"));
					++ret;
					translating = 0;
				}
				break;
			case 'd':
				if (translating) {
					char buf[16];
					translating = 0;
					itoa(va_arg(arglist, int), buf, 10);
					puts(fd, buf);
					ret += strlen(buf);
				} else {
					putchar(fd, 'd');
					++ret;
				}
				break;
			case 's':
				if (translating) {
					const char *str = va_arg(arglist, const char *);
					translating = 0;
					puts(fd, str);
					ret+=strlen(str);
				} else {
					putchar(fd, 's');
					ret++;
				}
				break;
			default:
				if (translating) {
					translating = 0;
				}
				putchar(fd, *p);
				ret++;
				break;
		}
	}
}
int printf(const char *format, ...)
{
	va_list(arglist);
	va_start(arglist, format);
	return dprintf(stdout, format, arglist);
}
