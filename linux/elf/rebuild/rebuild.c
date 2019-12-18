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
#include "ptrace_interface.h"

void dump_ehdr(Elf64_Ehdr *ehdr)
{
	printf("Elf64_Ehdr info:\n\tentry:%lx \tphnum:%d \tpentry size:%d\n",
			ehdr->e_entry, ehdr->e_phnum, ehdr->e_phentsize);
}
int main(int argc, char* argv[]) {
	int fd;
	char *buf = NULL, *pmem = NULL;
	Elf64_Ehdr ehdr;
	Elf64_Phdr *phdr;
	pid_t pid;
	long ret = 0, i = 0;
	Elf64_Addr text_base, text_size, data_offset, data_size, data_vaddr, bss_len, dynvaddr, dynoffset, dynsize, base_addr;
	Elf64_Addr interp_vaddr, interp_off, interp_size;
	Elf64_Addr stringTable, plt_siz, got_off, got;
	Elf64_Addr *GLOBAL_OFFSET_TABLE;
	Elf64_Sym *symtab;

	if(argc < 2) {
		printf("Usage :%s <pid>\n", argv[0]);
		return -1;
	}

	fd = open("123", O_CREAT|O_RDWR|O_TRUNC);
	pid = strtol(argv[1], NULL, 10);
	base_addr = get_mem_base(pid);

	ret = ptrace_attach(pid);
	printf("attach pid:%d ret:%d\n", pid, ret);
	printf("base addr is:%lx\n", base_addr);
	ret = ptrace_read(&ehdr, sizeof(ehdr), (void *)base_addr, pid);
	dump_ehdr(&ehdr);

	printf("ehdr ret:%d\n", ret);
	pmem = malloc(ehdr.e_phentsize * ehdr.e_phnum);
	ret = ptrace_read(pmem, ehdr.e_phentsize * ehdr.e_phnum, base_addr + ehdr.e_phoff, pid);
	printf("phdr ret:%d\n", ret);

	phdr = (Elf64_Phdr *)pmem;

	for (i = 0; i < ehdr.e_phnum; i++) {
		//printf("phnum:%d\n", i);
		if (phdr[i].p_type == PT_LOAD && !phdr[i].p_offset) {
			text_base = phdr[i].p_offset;
			text_size = phdr[i].p_filesz;
		}
		if (phdr[i].p_type == PT_LOAD && phdr[i].p_offset) {
			data_offset = phdr[i].p_offset;
			data_size = phdr[i].p_filesz;
			data_vaddr = phdr[i].p_vaddr;
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

	printf("prepare text\n");
	buf = calloc(1, data_offset + data_size);
	ret = ptrace_read(buf, text_size, base_addr, pid);
	printf("text ret:0x%x\n", ret);
	ret = ptrace_read(buf + data_offset, data_size, base_addr + data_vaddr, pid);
	printf("data ret:0x%x\n", ret);

	printf("dynamic offset:%x\n", dynoffset);
	Elf64_Dyn *dyn = (Elf64_Dyn *)(buf + dynoffset);
	for (i = 0; dyn[i].d_tag != DT_NULL; i++) {
		switch(dyn[i].d_tag)
		{
			case DT_PLTGOT:	//.got section
				printf("Located PLT GOT Vaddr 0x%lx index:%d\n", got = (Elf64_Addr)dyn[i].d_un.d_ptr, i);
				printf("Relevant GOT entries begin at 0x%x\n", (Elf64_Addr)dyn[i].d_un.d_ptr + );
				got_off = dyn[i].d_un.d_ptr - data_vaddr;
				GLOBAL_OFFSET_TABLE = (Elf64_Addr *)(buf + data_offset + got_off);
				GLOBAL_OFFSET_TABLE[0] = dynvaddr;
				GLOBAL_OFFSET_TABLE += 3;
				break;
			case DT_PLTRELSZ:
				plt_siz = dyn[i].d_un.d_val / sizeof(Elf64_Rel);
				break;
			case DT_STRTAB:
				stringTable = (char *)dyn[i].d_un.d_ptr;
				break;
			case DT_SYMTAB:
				symtab = (Elf64_Sym *)dyn[i].d_un.d_ptr;
				break;
		}
	}

	//clear 2,3 entry
	uint8_t *gp = &buf[data_offset + got_off + 8];
	for (i = 0; i < 8 * 2; i++)
		*(gp + 8) = 0;
	
	Elf64_Addr PLT_VADDR = GLOBAL_OFFSET_TABLE[0]; /* gmon_start */
	printf("[+] Resolved PLT: 0x%lx\n", PLT_VADDR);
	
	write(fd, buf, data_offset + data_size);
	close(fd);
}
