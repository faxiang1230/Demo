/*************************************************************************
    > File Name: ptrace-interface.c
    > Author: wangjx
 ************************************************************************/
#include <sys/ptrace.h>
#include "ptrace_interface.h"

long ptrace_attach(pid_t pid);
long ptrace_detach(pid_t pid);
long ptrace_read(void *to, int len, void *from, pid_t pid);
long ptrace_write(void *from, int len, void *to, pid_t pid);

long ptrace_attach(pid_t pid)
{
	int status;
	long ret = ptrace(PTRACE_ATTACH, pid, NULL, NULL);
	if (ret < 0) {
		dprintf(2, "failed to attach %d, error:%s\n", pid, strerror(errno));
		return ret;
	}

	waitpid(pid, &status, 0);
	return 0;
	//return ptrace(PTRACE_CONT, pid, NULL, NULL);
}

long ptrace_detach(pid_t pid)
{
	return ptrace(PTRACE_DETACH, pid, NULL, NULL);
}
long ptrace_read(void *to, int len, void *from, pid_t pid)
{
	int offset = 0;
	int size = len / sizeof(long);
	long word;
	unsigned char *src = from;
	unsigned char *dest = to;

	while (size) {
		word = ptrace(PTRACE_PEEKDATA, pid, src + offset, NULL);
		if (word == -1 && errno != 0) {
			printf("%s size:%d offset:%d addr:%p error:%s\n", __func__, size, offset, src, strerror(errno));
			return 1;
		}
		*(long *)(dest + offset) = word;
		offset += sizeof(long);
		size--;
	}
	return offset;
}
long ptrace_write(void *from, int len, void *to, pid_t pid)
{
	int offset = 0;
	int size = len / sizeof(long);
	long word;
	unsigned char *src = from;
	unsigned char *dest = to;

	while (size) {
		word = ptrace(PTRACE_POKEDATA, pid, dest + offset, src + offset);
		if (word == -1 && errno != 0) {
			printf("%s size:%d offset:%d addr:%p error:%s\n", __func__, size, offset, src, strerror(errno));
			return 1;
		}
		offset += sizeof(long);
		size--;
	}
	return offset;
}
Elf64_Addr get_mem_base(pid_t pid)
{
	char path[128], proto[16];
	char *line = NULL;
	FILE *fp = NULL;
	size_t len = 0;
	Elf64_Addr base_start = 0, base_end = 0;

	snprintf(path, sizeof(path), "/proc/%d/maps", pid);
	fp = fopen(path, "r");
	if (!fp) return 0;

	if (getline(&line, &len, fp) > 0)
		sscanf(line, "%lx-%lx %s %*s %*s %*s %*s", &base_start, &base_end, proto);

	printf("base_start %lx\n", base_start);
	free(line);
	fclose(fp);
	return base_start;
}
