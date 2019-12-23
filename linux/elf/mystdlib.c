#define __syscall0(type,name) \
    type name(void) \
{ \
    long __res; \
    __asm__ volatile ("int $0x80" \
            : "=a" (__res) \
            : "0" (__NR_##name)); \
    return(type)__res; \
}

#define __syscall1(type,name,type1,arg1) \
    type name(type1 arg1) \
{ \
    long __res; \
    __asm__ volatile ("int $0x80" \
            : "=a" (__res) \
            : "0" (__NR_##name),"b" ((long)(arg1))); \
    return(type)__res; \
}


#define __syscall2(type,name,type1,arg1,type2,arg2) \
    type name(type1 arg1,type2 arg2) \
{ \
    long __res; \
    __asm__ volatile ("int $0x80" \
            : "=a" (__res) \
            : "0" (__NR_##name),"b" ((long)(arg1)),"c" ((long)(arg2))); \
    return(type)__res; \
}

#define __syscall3(type,name,type1,arg1,type2,arg2,type3,arg3) \
    type name(type1 arg1,type2 arg2,type3 arg3) \
{ \
    long __res; \
    __asm__ volatile ("int $0x80" \
            : "=a" (__res) \
            : "0" (__NR_##name),"b" ((long)(arg1)),"c" ((long)(arg2)), \
            "d" ((long)(arg3))); \
    return(type)__res; \
}
#define __syscall4(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4) \
    type name (type1 arg1, type2 arg2, type3 arg3, type4 arg4) \
{ \
    long __res; \
    __asm__ volatile ("int $0x80" \
            : "=a" (__res) \
            : "0" (__NR_##name),"b" ((long)(arg1)),"c" ((long)(arg2)), \
            "d" ((long)(arg3)),"S" ((long)(arg4))); \
    return(type)__res; \
}

#define __syscall5(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4, \
        type5,arg5) \
type name (type1 arg1,type2 arg2,type3 arg3,type4 arg4,type5 arg5) \
{ \
    long __res; \
    __asm__ volatile ("int $0x80" \
            : "=a" (__res) \
            : "0" (__NR_##name),"b" ((long)(arg1)),"c" ((long)(arg2)), \
            "d" ((long)(arg3)),"S" ((long)(arg4)),"D" ((long)(arg5))); \
    return(type)__res; \
}
#define __syscall6(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4, \
        type5,arg5,type6,arg6) \
type name (type1 arg1,type2 arg2,type3 arg3,type4 arg4,type5 arg5,type6 arg6) \
{ \
    long __res; \
    __asm__ volatile ("push %%ebp ; movl %%eax,%%ebp ; movl %1,%%eax ; int $0x80 ; pop %%ebp" \
            : "=a" (__res) \
            : "i" (__NR_##name),"b" ((long)(arg1)),"c" ((long)(arg2)), \
            "d" ((long)(arg3)),"S" ((long)(arg4)),"D" ((long)(arg5)), \
            "0" ((long)(arg6))); \
    return(type),__res; \
}

__syscall1(void, exit, int, status);
__syscall3(ssize_t, write, int, fd, const void *, buf, size_t, count);
__syscall3(off_t, lseek, int, fildes, off_t, offset, int, whence);
__syscall2(int, fstat, int, fildes, struct stat * , buf);
__syscall2(int, rename, const char *, old, const char *, new);
__syscall3(int, open, const char *, pathname, int, flags, mode_t, mode);
__syscall1(int, close, int, fd);
__syscall3(int, getdents, uint, fd, struct dirent *, dirp, uint, count);
__syscall3(int, read, int, fd, void *, buf, size_t, count);
__syscall2(int, stat, const char *, path, struct stat *, buf);

