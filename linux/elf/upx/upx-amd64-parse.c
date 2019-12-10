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
void upx_parse(char *header)
{
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)header;
	Elf64_Phdr *phdr = NULL;
	int pnum = ehdr->e_phnum;
	int index = 0;

	for (; index < pnum; index++) {
		phdr = (Elf64_Phdr *)(ehdr + 1) + index;
	}

	struct l_info *linfo = (struct l_info *)(phdr + 1);
	struct p_info *pinfo = (struct p_info *)(linfo + 1);
	struct b_info *binfo = (struct b_info *)(pinfo + 1);
	for (index = 0; index < 3; index++) {
		binfo = (char *)(binfo + 1) + binfo->sz_cpr;
	}
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
