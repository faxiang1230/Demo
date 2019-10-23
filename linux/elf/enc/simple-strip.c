/*************************************************************************
    > File Name: simple-strip.c
    > Author: wangjx
 ************************************************************************/
#include "common.h"
#include <elf.h>
void my_exit(int exit)
{
	__asm__ volatile (
			"mov %0, %%rdi\n"
			"mov $60, %%rax\n"
			"syscall":	:"g"(exit)
			);
}
int my_printf(char *fmt, ...)
{
	char printf_buf[128];
	int size = 0;
	long ret = 0;
	va_list args;
	va_start(args, fmt);
	size = vsprintf(printf_buf, fmt, args);
	va_end(args);
	__asm__ volatile (
			"mov	$1,	%%rdi\n"
			"mov	%0,	%%rsi\n"
			"mov	%1,	%%rdx\n"
			"mov	$1,	%%rax\n"
			"syscall":	:"g"(printf_buf), "g"(size)
			);

	__asm__ volatile(
			"mov	%%rax,	%0":	"=r"(ret)
			);
	return ret;
}
void main(int argc, char **argv)
{
	int ret =0;
	Elf64_Ehdr *ehdr = NULL;
	Elf64_Shdr *shdr = NULL;
	Elf64_Sym *sym = NULL;
	char *mem = NULL, *shdr_strtab, *sym_strtab = NULL;
	int fd = 0, i =0, j = 0;
	struct stat st;

	if (argc < 2) {
		my_printf("\t%s target\n", argv[0]);
		my_exit(-1);
	}
	
	fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		my_printf("%s open failed\n", argv[1]);
		my_exit(-1);
	}
	fstat(fd, &st);

	mem = mmap(NULL, st.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	assert(mem != MAP_FAILED);

	ehdr = (Elf64_Ehdr *)mem;
	shdr = (Elf64_Shdr *)(mem + ehdr->e_shoff);
	shdr_strtab = mem + shdr[ehdr->e_shstrndx].sh_offset;
	/*TODO
	 *	1.erase section hdr string
	 *	2.erase symbol str and symbol
	 *	3.erase hdr section num and offset
	 */
	for (i = 0; i < ehdr->e_shnum; i++) {
		//my_printf("erase section str:%s\n", &shdr_strtab[shdr[i].sh_name]);
		memset(&shdr_strtab[shdr[i].sh_name], 0, strlen(&shdr_strtab[shdr[i].sh_name]));
		if (shdr[i].sh_type == SHT_SYMTAB) {
			sym_strtab = mem + shdr[shdr[i].sh_link].sh_offset;
			sym = (Elf64_Sym *)(mem + shdr[i].sh_offset);
			for (j = 0; j < shdr[i].sh_size/sizeof(Elf64_Sym); j++) {
				my_printf("erase sym value:%p :%s\n", sym[j].st_value, sym_strtab + sym[j].st_name);
				memset(sym_strtab + sym[j].st_name, 0, strlen(sym_strtab + sym[j].st_name)); 
				memset(sym + j, 0, sizeof(Elf64_Sym));
			}
		}
	}
	ehdr->e_shnum = 0;
	ehdr->e_shoff = 0;
	ehdr->e_shstrndx = 0;

	msync(mem, st.st_size, MS_SYNC);
	munmap(mem, st.st_size);
	close(fd);
}
