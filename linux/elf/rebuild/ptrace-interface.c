/*************************************************************************
    > File Name: ptrace-interface.c
    > Author: wangjx
 ************************************************************************/
#include <sys/ptrace.h>

long ptrace_attach(pid_t pid);
long ptrace_detach(pid_t pid);
long ptrace_read(void *to, int len, void *from, pid_t pid);
long ptrace_write(void *from, int len, void *to, pid_t pid);

long ptrace_attach(pid_t pid)
{
	long ret = ptrace(PTRACE_ATTACH, pid, NULL, NULL);
	if (ret < 0) {
		fwrite(2, "failed to attach %d, error:%s\n", pid, strerror(errno));
		return ret;
	}

	return ptrace(PTRACE_CONT, pid, NULL, NULL);
}

long ptrace_detach(pid_t pid)
{
	return ptrace(PTRACE_DETACH, pid, NULL, NULL);
}
long ptrace_read(void *to, int len, void *from, pid_t pid)
{
	int offset = 0;
	while(ptrace(PTRACE_PEEKUSER, pid, addr + offset, to + offset) > 0)
		offset += sizeof(int);
	return offset;
}
long ptrace_write(void *from, int len, void *to, pid_t pid)
{
	int offset = 0;
	while(ptrace(PTRACE_POKEUSER, pid, addr + offset, to + offset) > 0)
		offset += sizeof(int);
	return offset;
}
unsigned long get_mem_base(pid_t pid)
{
	char path[128], proto[16];
	char *line = NULL;
	FILE *fp = NULL;
	unsigned long base_start = 0, base_end = 0;

	snprintf(path, sizeof(path), "/proc/%d/maps", pid);
	fp = fopen(path, "r");
	if (!fp) return 0;

	if (getline(&line, &len, fp) > 0)
		sscanf(line, "%x-%x %s %*s %*s %*s %*s", &base_start, &bast_end, proto);

	free(line);
	fclose(fp);
	return base_start;
}
