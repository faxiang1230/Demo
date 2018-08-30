#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <elf.h>
#include <signal.h>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <sys/mman.h>

//#include "arch.h"

typedef struct handle {
	Elf64_Ehdr *ehdr;
	Elf64_Phdr *phdr;
	Elf64_Shdr *shdr;
	uint8_t *mem;
	char *symname;
	Elf64_Addr symaddr;
	struct user_regs_struct pt_reg;
	char *exec;
}handle_t;
Elf64_Addr lookup_symbol(handle_t *h, const char *symname);
int main(int argc, char* argv[]) {
	int fd;
	handle_t h;
	struct stat st;
	long trap, orig;
	int status, pid;
	char *args[2];
	char *envp[] = {NULL};

	if(argc < 3) {
		printf("Usage :%s <program> <function>\n", argv[0]);
		return -1;
	}

	if((h.exec = strdup(argv[1])) == NULL) {
		return -1;
	}
	
	args[0] = h.exec;
	args[1] = NULL;
	if((h.symname = strdup(argv[2])) == NULL) {
		return -1;
	}
	if((fd = open(argv[1], O_RDONLY)) < 0) {
		perror("open");
		return -1;
	}

	if(fstat(fd, &st) < 0) {
		perror("fstat");
		return -1;
	}
	printf("open file fd:%d, size:%d\n", fd, st.st_size);
	//if(st.st_size % (1 << 31))
	//	st.st_size = ((st.st_size >> 31) + 1) << 31;
	h.mem = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if(h.mem == MAP_FAILED) {
		perror("mmap");
		return -1;
	}
	h.ehdr = (Elf64_Ehdr *)h.mem;
	h.phdr = (Elf64_Phdr *)(h.mem + h.ehdr->e_phoff);
	h.shdr = (Elf64_Shdr *)(h.mem + h.ehdr->e_shoff);

	if(h.mem[0] != 0x7f || strncmp((char *)&h.mem[1], "ELF", 3)) {
		perror("Elf format error");
		return -1;
	}

	if(h.ehdr->e_type != ET_EXEC) {
		perror("elf is not an excutable elf\n");
		return -1;
	}

	if(h.ehdr->e_shstrndx == 0 || h.ehdr->e_shoff == 0 || h.ehdr->e_shnum == 0) {
		perror("Section header table not found\n");
		return -1;
	}
	if((h.symaddr = lookup_symbol(&h, h.symname)) == 0) {
		printf("Unable find symbol :%s\n", h.symname);
		return -1;
	}
	close(fd);
	if((pid = fork()) < 0) {
		perror("fork");
		return -1;
	}
	if(pid == 0) {
		assert(ptrace(PTRACE_TRACEME, pid, NULL, NULL) >=0 );
		execve(h.exec, args, envp);
		exit(0);
	}

	wait(&status);
	printf("Beginning analysis of pid :%d at %lx\n", pid, h.symaddr);

	assert((orig = ptrace(PTRACE_PEEKTEXT, pid, h.symaddr, NULL)) >= 0);

	trap = (orig & ~0xff) | 0xcc;
	assert(ptrace(PTRACE_POKETEXT, pid, h.symaddr, trap) >= 0);
	printf("first poketext %p instruction :%x\n", h.symaddr, ptrace(PTRACE_PEEKTEXT, pid, h.symaddr, NULL));

trace:
	assert(ptrace(PTRACE_CONT, pid, NULL, NULL) >= 0);
	wait(&status);
	printf("after cont,receive signal %llx\n", status);
	if(WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP) {
		assert(ptrace(PTRACE_GETREGS, pid, NULL, &h.pt_reg) >= 0);
		
		printf("\nExecutable %s (pid:%d) has bit breakpoint 0x%lx\n", h.exec, pid, h.symaddr);
		printf("%%rcx: %llx\n%%rdx: %llx\n%%rbx: %llx\n%%rax: %llx\n%%rdi: %llx\n%%rsi: %llx\n%%r8: %llx\n"
			"%%r9: %llx\n%%r10: %llx\n%%r11: %llx\n%%r12: %llx\n%%r13: %llx\n%%r14: %llx\n%%r15: %llx\n%%rsp: %llx\n", 
			h.pt_reg.rcx, h.pt_reg.rdx, h.pt_reg.rbx, h.pt_reg.rax, h.pt_reg.rdi, h.pt_reg.rsi, h.pt_reg.r8,
			h.pt_reg.r9, h.pt_reg.r10, h.pt_reg.r11, h.pt_reg.r12, h.pt_reg.r13, h.pt_reg.r14, h.pt_reg.r15, h.pt_reg.rsp);
		printf("Please hit any key to continue: ");
		getchar();
	
		assert(ptrace(PTRACE_POKETEXT, pid, h.symaddr, orig) >= 0);	
		h.pt_reg.rdi = h.pt_reg.rdi + 1;
		//h.pt_reg.rip = h.pt_reg.rip + 1;
		assert(ptrace(PTRACE_SETREGS, pid, NULL, &h.pt_reg) >= 0);
		assert(ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL) >= 0);
		wait(NULL);
		assert(ptrace(PTRACE_SETREGS, pid, NULL, &h.pt_reg) >= 0);
		printf("after poketext %p instruction :%x\n", h.symaddr, ptrace(PTRACE_PEEKTEXT, pid, h.symaddr, NULL));
		goto trace;
	}
	if(WIFEXITED(status))
		printf("Completed tracing pid: %d\n", pid);
	exit(0);
	return 0;	
}
Elf64_Addr lookup_symbol(handle_t *h, const char *symname) {
	int i, j;
	char *strtab;
	Elf64_Sym *symtab;
	for(i = 0; i < h->ehdr->e_shnum; i++) {
		if(h->shdr[i].sh_type == SHT_SYMTAB) {
			strtab = (char *)&h->mem[h->shdr[h->shdr[i].sh_link].sh_offset];
			symtab = (Elf64_Sym *)&h->mem[h->shdr[i].sh_offset];
			for(j = 0; j < (h->shdr[i].sh_size /sizeof(Elf64_Sym)); j++) {
				if(strcmp(&strtab[symtab->st_name], symname) == 0) {
					return (symtab->st_value);
				}
				symtab++;
			}
		}
	}
	return 0;
}
