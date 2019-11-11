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
	int section_len,index;
	
	Elf64_Ehdr *hdr;
	Elf64_Phdr *phdr;
	Elf64_Shdr *shdr;
	char *tmp;
	Elf64_Sym *sym;
	Elf64_Rel *rel;
	Elf64_Rela *rela;
	Elf64_Dyn *dynamic;

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
	shdr = (Elf64_Shdr *)(mem + hdr->e_shoff);
	if(hdr->e_ident[0] != 0x7f && strncmp(&hdr->e_ident[1], "ELF", strlen("ELF"))) {
		perror("Wrong file format\n");
		exit(-1);
	}

	stringtab = &mem[shdr[hdr->e_shstrndx].sh_offset];
	for (i = 1; i < hdr->e_shnum; i++) {
		printf("%32s%16x", &stringtab[shdr[i].sh_name], shdr[i].sh_addr);
		if(shdr[i].sh_type < SHT_NUM) {
			printf("%16s", elf_section_type_names[shdr[i].sh_type]);
		} else {
			printf("%16x", shdr[i].sh_type);

		}
		printf("%16x", shdr[i].sh_flags);
		printf("%16x", shdr[i].sh_offset);
		printf("%16x", shdr[i].sh_size);
		if(shdr[i].sh_type == SHT_DYNAMIC || shdr[i].sh_type == SHT_HASH || 
				shdr[i].sh_type == SHT_REL || shdr[i].sh_type == SHT_RELA || 
				shdr[i].sh_type == SHT_SYMTAB ||shdr[i].sh_type == SHT_DYNSYM) {
			if(shdr[i].sh_link < SHT_NUM) {
				printf("%16s", elf_section_type_names[shdr[i].sh_link]);
			} else {
				printf("%16x", shdr[i].sh_link);
			}
			if(shdr[i].sh_type == SHT_REL || shdr[i].sh_type == SHT_RELA ||
					shdr[i].sh_type == SHT_SYMTAB ||shdr[i].sh_type == SHT_DYNSYM) {
				if(shdr[i].sh_info < SHT_NUM) {
					printf("%16s", elf_section_type_names[shdr[i].sh_info]);
				} else {
					printf("%16x", shdr[i].sh_info);
				}
			}

		}
		printf("\n");
	}
	for (i = 1; i < hdr->e_shnum; i++) {
		if(!strcmp(&stringtab[shdr[i].sh_name],".dynamic")){}
		else if(!strcmp(&stringtab[shdr[i].sh_name],".got")){}
		else if(!strcmp(&stringtab[shdr[i].sh_name],".got.plt")){}
		else if(!strcmp(&stringtab[shdr[i].sh_name],".symtab")){}
		else if(!strcmp(&stringtab[shdr[i].sh_name],".rela.dyn")){}
		
		if(shdr[i].sh_type == SHT_REL) {
			printf("%s Relocatable table info\n", &stringtab[shdr[i].sh_name]);	
			printf("%16s%16s%16s%16s\n", "Offset", "Info", "Type", "SYMBOL");
			for(index = 0; index < shdr[i].sh_size; index += sizeof(Elf64_Rel)) {
				rel = (Elf64_Rel *)(mem + shdr[i].sh_offset + index);
				printf("%16x", rel->r_offset);
				printf("%16x", rel->r_info);
				//printf("%16s", rel_type[ELF64_R_TYPE(rel->r_info)]);
				sym = (Elf64_Sym *)(mem + shdr[shdr[i].sh_link].sh_offset + (sizeof(Elf64_Sym) * ELF64_R_SYM(rel->r_info)));
				printf("%16s", mem + shdr[shdr[shdr[i].sh_link].sh_link].sh_offset + sym->st_name);
				printf("\n");
			}
		} else if(shdr[i].sh_type == SHT_RELA) {
			printf("%s Relocatable table info\n", &stringtab[shdr[i].sh_name]);	
			printf("%16s%16s %16s%16s\n", "Offset", "Info", "Type", "SYMBOL");
			for(index = 0; index < shdr[i].sh_size; index += sizeof(Elf64_Rela)) {
				rela = (Elf64_Rela *)(mem + shdr[i].sh_offset + index);
				printf("%16x", rela->r_offset);
				printf("%16x ", rela->r_info);
				printf("%16s", rel_type[ELF64_R_TYPE(rela->r_info)]);
				sym = (Elf64_Sym *)(mem + shdr[shdr[i].sh_link].sh_offset + (sizeof(Elf64_Sym) * ELF64_R_SYM(rela->r_info)));
				printf("%16s", mem + shdr[shdr[shdr[i].sh_link].sh_link].sh_offset + sym->st_name);
				printf("\n");
			}
		} else if(shdr[i].sh_type == SHT_SYMTAB || shdr[i].sh_type == SHT_DYNSYM) {
			printf("%s Symbol table info\n", &stringtab[shdr[i].sh_name]);
			printf("%16s%16s%16s%16s%16s%16s%16s\n", "Type", "Bind", "Ndx","Visible", "Value", "Size", "Name");
			for(index = 0; index < shdr[i].sh_size; index += sizeof(Elf64_Sym)) {
				sym = (Elf64_Sym *)(mem + shdr[i].sh_offset + index);
				printf("%16s", symbol_type[ELF64_ST_TYPE(sym->st_info)]);
				printf("%16s", symbol_bind[ELF64_ST_BIND(sym->st_info)]);
				printf("%16x", sym->st_shndx);
				printf("%16s", symbol_visible[ELF64_ST_VISIBILITY(sym->st_other)]);
				printf("%16x", sym->st_value);
				printf("%16x", sym->st_size);
				printf(" %s", mem + shdr[shdr[i].sh_link].sh_offset + sym->st_name);
				printf("\n");
			}
		} else if(shdr[i].sh_type == SHT_DYNAMIC) {
			printf("%s Dynamic table info\n", &stringtab[shdr[i].sh_name]);
			printf("%16s%16s\n", "Type", "Value");
			for(index = 0; index < shdr[i].sh_size; index += sizeof(Elf64_Dyn)) {
				dynamic = (Elf64_Dyn *)(mem + shdr[i].sh_offset + index);
				printf("%16s ", get_dynamic_type(dynamic->d_tag));
				if(dynamic->d_tag == DT_NEEDED) {
					//tmp = dynamic->d_un.d_val;
				}
				printf("\n");
			}
				
		}
	}
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
