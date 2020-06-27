#include "x86_64/unistd_64.h"
#include "x86_64/types.h"
#include <sys/stat.h>
#include <dirent.h>

/* Registers clobbered by syscall.  */                                          
# define REGISTERS_CLOBBERED_BY_SYSCALL "cc", "r11", "cx"
                                                                                
/* Create a variable 'name' based on type 'X' to avoid explicit types.          
 *    This is mainly used set use 64-bits arguments in x32.   */                   
#define TYPEFY(X, name) __typeof__ ((X) - (X)) name                             
/* Explicit cast the argument to avoid integer from pointer warning on          
 *    x32.  */                                                                     
#define ARGIFY(X) ((__typeof__ ((X) - (X))) (X))                                
                                                  
#define __syscall0(type,name) \
    type name(void) \
{ \
    long __res; \
    __asm__ volatile ("syscall" \
            : "=a" (__res) \
            : "a" (__NR_##name)); \
    return(type)__res; \
}

#define __syscall1(type,name,type1,arg1) \
    type name(type1 arg1) \
{ \
    long __res; \
    __asm__ volatile ("syscall" \
            : "=a" (__res) \
            : "a" (__NR_##name),"D" ((long)(arg1))); \
    return(type)__res; \
}


#define __syscall2(type,name,type1,arg1,type2,arg2) \
    type name(type1 arg1,type2 arg2) \
{ \
    long __res; \
    __asm__ volatile ("syscall" \
            : "=a" (__res) \
            : "a" (__NR_##name),"D" ((long)(arg1)),"S" ((long)(arg2))); \
    return(type)__res; \
}

#define __syscall3(type,name,type1,arg1,type2,arg2,type3,arg3) \
    type name(type1 arg1,type2 arg2,type3 arg3) \
{ \
    long __res; \
    __asm__ volatile ("syscall" \
            : "=a" (__res) \
            : "a" (__NR_##name),"D" ((long)(arg1)),"S" ((long)(arg2)), \
            "d" ((long)(arg3))); \
    return(type)__res; \
}
#define __syscall4(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4) \
    type name (type1 arg1, type2 arg2, type3 arg3, type4 arg4) \
{ \
    long __res; \
	TYPEFY (arg4, __arg4) = ARGIFY (arg4);	\
	register TYPEFY (arg4, _a4) asm ("r10") = __arg4;	\
    __asm__ volatile ("syscall" \
            : "=a" (__res) \
            : "a" (__NR_##name),"D" ((long)(arg1)),"S" ((long)(arg2)), \
            "d" ((long)(arg3)),"r" (_a4)	\
			:"memory", REGISTERS_CLOBBERED_BY_SYSCALL); \
    return(type)__res; \
}

#define __syscall5(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4, \
        type5,arg5) \
type name (type1 arg1,type2 arg2,type3 arg3,type4 arg4,type5 arg5) \
{ \
    long __res; \
	TYPEFY (arg4, __arg4) = ARGIFY (arg4);	\
	TYPEFY (arg5, __arg5) = ARGIFY (arg5);	\
	register TYPEFY (arg4, _a4) asm ("r10") = __arg4;	\
	register TYPEFY (arg5, _a5) asm ("r8") = __arg5;	\
    __asm__ volatile ("syscall" \
            : "=a" (__res) \
            : "a" (__NR_##name),"D" ((long)(arg1)),"S" ((long)(arg2)), \
            "d" ((long)(arg3)),"r" (_a4),"r" (_a5)	\
			:"memory", REGISTERS_CLOBBERED_BY_SYSCALL); \
    return(type)__res; \
}
#define __syscall6(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4, \
        type5,arg5,type6,arg6) \
type name (type1 arg1,type2 arg2,type3 arg3,type4 arg4,type5 arg5,type6 arg6) \
{ \
    long __res; \
	TYPEFY (arg4, __arg4) = ARGIFY (arg4);	\
	TYPEFY (arg5, __arg5) = ARGIFY (arg5);	\
	TYPEFY (arg6, __arg6) = ARGIFY (arg6);	\
	register TYPEFY (arg4, _a4) asm ("r10") = __arg4;	\
	register TYPEFY (arg5, _a5) asm ("r8") = __arg5;	\
	register TYPEFY (arg6, _a6) asm ("r9") = __arg6;	\
    __asm__ volatile ("syscall" \
            : "=a" (__res) \
            : "a" (__NR_##name),"D" ((long)(arg1)),"S" ((long)(arg2)), \
            "d" ((long)(arg3)),"r" (_a4),"r" (_a5), "r" (_a6)	\
			:"memory", REGISTERS_CLOBBERED_BY_SYSCALL); \
    return(type)__res; \
}

__syscall1(void, exit, int, status);
__syscall1(long, brk, void*, addr);
__syscall3(off_t, lseek, int, fildes, off_t, offset, int, whence);
__syscall2(int, fstat, int, fildes, struct stat * , buf);
__syscall2(int, rename, const char *, old, const char *, new);
__syscall3(int, open, const char *, pathname, int, flags, mode_t, mode);
__syscall1(int, close, int, fd);
__syscall3(int, read, int, fd, void *, buf, size_t, count);
__syscall3(ssize_t, write, int, fd, const void *, buf, size_t, count);
__syscall3(int, getdents, uint, fd, struct dirent *, dirp, uint, count);
__syscall2(int, stat, const char *, path, struct stat *, buf);

