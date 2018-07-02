/*************************************************************************
    > File Name: parse.c
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: Monday, July 02, 2018 PM10:41:11 HKT
 ************************************************************************/

#include<stdio.h>
#include <elf.h>
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <assert.h>

void main(int argc, char **argv)
{
	int fd = 0;
	char *mem = NULL;
	struct stat st;
	Elf64_Ehdr *hdr;
	if(argc < 2) {
		perror("CMD:elf-parse file		Error\n");
		exit(-1);
	}
	fd = open(argv[1], O_RDONLY);
	if(fd < 0) {
		perror("Failed open file %s:%s", argv[1], strerror(errno));
		exit(-1);
	}
	ret = fstat(fd, &st);
	assert(ret == 0);
	mem = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
	assert(mem != MAP_FAILED);

	hdr = (Elf64_Ehdr *)mem;
	if(hdr)
	
}
