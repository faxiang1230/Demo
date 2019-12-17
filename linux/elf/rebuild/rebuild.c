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

long ptrace_attach(pid_t pid);
long ptrace_detach(pid_t pid);
long ptrace_read(void *to, int len, void *from, pid_t pid);
long ptrace_write(void *from, int len, void *to, pid_t pid);
#define SIZE_1M	(1024*1024*1024)
int main(int argc, char* argv[]) {
	int fd;
	handle_t h;
	struct stat st;
	long trap, orig;
	int status, pid;
	char *args[2];
	char *envp[] = {NULL};
	char *buf = NULL;
	Elf64_Ehdr ehdr;
	Elf64_Phdr *phdr;
	pid_t pid;
	long ret = 0;

	if(argc < 3) {
		printf("Usage :%s <pid>\n", argv[0]);
		return -1;
	}

	fd = open("123", O_CREAT|O_RDWR|O_TRUNC);
	pid = strtol(argv[1], NULL, 10);
	base_addr = get_mem_base(pid);

	pmem = calloc(1, SIZE_1M);
	ptrace_read(&ehdr, sizeof(ehdr), base_addr, pid);
	ptrace_read(pmem, ehdr.e_phentsize * ehdr.e_phnum, base_addr + ehdr.e_phoff, pid);

	phdr = (Elf64_Phdr *)pmem;

	for (i = 0; i < ehdr.e_phnum; i++) {
		if (phdr[i].p_type == PT_LOAD && !phdr[i].p_offset) {
			text_base = phdr[i].p_offset;
			text_size = phdr[i].p_filesz;
		}
		if (phdr[i].p_type == PT_LOAD && phdr[i].p_offset) {
			data_offset = phdr[i].p_offset;
			data_size = phdr[i].p_filesz;
			bss_len = phdr[i].p_memsz - phdr[i].p_filesz;
		}
		if (phdr[i].p_type == PT_DYNAMIC) {
			dynvaddr = phdr[i].p_vaddr;
			dynoffset = phdr[i].p_offset;
			dynsize = phdr[i].p_filesz;
		}
		if (phdr[i].p_type == PT_INTERP) {
			interp_vaddr = phdr[i].p_vaddr;
			interp_off = phdr[i].p_offset;
			interp_size = phdr[i].p_filesz;
		}
	}

	buf = calloc(1, text_size + data_size);
	ptrace_read(buf, text_size, base_addr, pid);
	ptrace_read(buf + text_size, data_size, base_addr + data_offset, pid);
	write(fd, buf, text_size + data_size);

	Elf64_Dyn *dyn = (Elf64_Dyn *)(buf + dynoffset);
	for (i = 0; dyn[i].d_tag != DT_NULL; i++) {
		switch(dyn[i].d_tag)
		{
			case DT_PLTGOT:
				printf("Located PLT GOT Vaddr 0x%x\n", got = (Elf32_Addr)dyn[i].d_un.d_ptr);
				printf("Relevant GOT entries begin at 0x%x\n", (Elf32_Addr)dyn[i].d_un.d_ptr + 12);
				got_off = dyn[i].d_un.d_ptr - data_vaddr;
				GLOBAL_OFFSET_TABLE = (Elf64_Addr *)(buf + data_offset + got_off);
				GLOBAL_OFFSET_TABLE += 3;
				break;
			case DT_PLTRELSZ:
				plt_siz = dyn[i].d_un.d_val / sizeof(Elf64_Rel);
				break;
			case DT_STRTAB:
				StringTable = (char *)dyn[i].d_un.d_ptr;
				break;
			case DT_SYMTAB:
				symtab = (Elf32_Sym *)dyn[i].d_un.d_ptr;
				break;
		}
	}

	
	close(fd);
}
