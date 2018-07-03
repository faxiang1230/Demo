/*************************************************************************
    > File Name: parse.c
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: Monday, July 02, 2018 PM10:41:11 HKT
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <assert.h>
void main(int argc, char **argv)
{
	int fd = 0, i, ret;
	char *mem = NULL, *stringtab, *interp;
	struct stat st;
	
	Elf64_Ehdr *hdr;
	Elf64_Phdr *phdr;
	Elf64_Shdr *shdr;

	if(argc < 2) {
		perror("CMD:elf-parse file		Error\n");
		exit(-1);
	}
	fd = open(argv[1], O_RDONLY);
	if(fd < 0) {
		printf("Failed open file %s:%s", argv[1], strerror(errno));
		exit(-1);
	}
	ret = fstat(fd, &st);
	assert(ret == 0);
	mem = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
	assert(mem != MAP_FAILED);

	hdr = (Elf64_Ehdr *)mem;
	phdr = (Elf64_Phdr*)(mem + hdr->e_phoff);
	shdr = (Elf64_Shdr *)(mem + hdr->e_shoff);
	if(hdr->e_ident[0] != 0x7f && strncmp(&hdr->e_ident[1], "ELF", strlen("ELF"))) {
		perror("Wrong file format\n");
		exit(-1);
	}
	printf("Program Entry Point:0x%lx\n", hdr->e_entry);

	stringtab = &mem[shdr[hdr->e_shstrndx].sh_offset];
	printf("Section header list:\n\n");
	for (i = 1; i < hdr->e_shnum; i++)
		printf("%s: 0x%lx\n", &stringtab[shdr[i].sh_name], shdr[i].sh_addr);

	printf("\nProgram header list\n\n");
	for(i = 0; i < hdr->e_phnum; i++) {
		switch (phdr[i].p_type) {
		case PT_LOAD:
			if(phdr[i].p_offset == 0)
				printf("Text segment :0x%lx\n", phdr[i].p_vaddr);
			else
				printf("Data segment :0x%lx\n", phdr[i].p_vaddr);
			break;
		case PT_INTERP:
			interp = strdup((char *)&mem[phdr[i].p_offset]);
			printf("Interpreter :%s\n", interp);
			break;
		case PT_NOTE:
			printf("Note segment :0x%lx\n", phdr[i].p_vaddr);
			break;
		case PT_DYNAMIC:
			printf("Dynamic segment :0x%lx\n", phdr[i].p_vaddr);
			break;
		case PT_PHDR:
			printf("Phdr segment :0x%lx\n", phdr[i].p_vaddr);
			break;
		}
	}
}
