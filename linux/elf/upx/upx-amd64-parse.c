#include <elf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "upx-header.h"
#include <assert.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
unsigned long pad4(unsigned long off)
{
	if (off%4)
		off += (4 - off%4);
	return off;
}
void upx_parse(char *header)
{
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)header;
	Elf64_Phdr *phdr = NULL;
	int pnum = ehdr->e_phnum;
	int index = 0;
	char *ptr = NULL;
	unsigned long offset = 0;
	struct packHeader *ph = NULL;

	printf("phum:%d entry:%lx\n", ehdr->e_phnum, ehdr->e_entry);
	for (; index < pnum; index++) {
		phdr = (Elf64_Phdr *)(ehdr + 1) + index;
		printf("offset:%lx filesize:%x memsize:%x vaddr:%lx\n", phdr->p_offset, phdr->p_filesz, phdr->p_memsz,phdr->p_vaddr);
	}

	struct l_info *linfo = (struct l_info *)(phdr + 1);
	struct p_info *pinfo = (struct p_info *)(linfo + 1);
	struct b_info *binfo = (struct b_info *)(pinfo + 1);

	for (index = 0; index < 3; index++) {
		printf("unc:%x cpr:%x \n", binfo->sz_unc, binfo->sz_cpr);
		binfo = (struct b_info *)((char *)(binfo + 1) + binfo->sz_cpr);
	}
	ptr = (char *)binfo;
	binfo = (struct b_info*)(ptr + 3 + linfo->l_lsize);
	for (index = 0; index < 10; index++) {
		printf("next unc:%x cpr:%x \n", binfo->sz_unc, binfo->sz_cpr);
		binfo = (struct b_info *)((char *)(binfo + 1) + binfo->sz_cpr);
		if (binfo->sz_unc == 0) {
			printf("EOF flag:%x\n", binfo->sz_cpr);
			break;
		}
	}
	offset = pad4(binfo + 1);
	ph = (struct packHeader *)offset;	
	printf("magic:%x version:%x checksum:%hx\n", ph->magic, ph->magic2, ph->checksum);
	printf("overlay_offset:%x", *(unsigned*)(ph + 1));
}
void main(int argc, char **argv)
{
	int fd = -1;
	char *buf = NULL;
	struct stat st;
	if (argc < 2) {
		printf("ERROR:\n\t%s file\n", argv[0]);
		exit(-1);
	}

	fd = open(argv[1], O_RDONLY);
	assert(fd > 0);

	fstat(fd, &st);

	buf = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	assert(buf);

	upx_parse(buf);

	munmap(buf, st.st_size);
	close(fd);
}
