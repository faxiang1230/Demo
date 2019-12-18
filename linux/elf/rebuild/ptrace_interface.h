/*************************************************************************
    > File Name: ptrace_interface.h
    > Author: wangjx
 ************************************************************************/
#ifndef __PTRACE_INTERFACE__
#define __PTRACE_INTERFACE__
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <elf.h>
long ptrace_attach(pid_t pid);
long ptrace_detach(pid_t pid);
long ptrace_read(void *to, int len, void *from, pid_t pid);
long ptrace_write(void *from, int len, void *to, pid_t pid);
Elf64_Addr get_mem_base(pid_t pid);
#endif
