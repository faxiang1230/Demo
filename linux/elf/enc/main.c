#include "common.h"

void enc_data(unsigned char *data, int len, unsigned char *key, int key_len)
{


}
void run_cmd(const char *fmt, ...)
{
	char printf_buf[128];
	va_list args;
	va_start(args, fmt);
	vsprintf(printf_buf, fmt, args);
	va_end(args);
	system(printf_buf);
}
int resolve_symbol(char *sym, char *mem)
{
	char *strtab = NULL;
	int i = 0;
	Elf64_Ehdr *hdr = (Elf64_Ehdr *)mem;
	Elf64_Shdr *shdr = (Elf64_Shdr *)(mem + hdr->e_shoff);
	strtab = &mem[shdr[hdr->e_shstrndx].sh_offset];

	for (i = 1; i < hdr->e_shnum; i++) {
		printf("section:%s\n", &strtab[shdr[i].sh_name]);
		if (strcmp(&strtab[shdr[i].sh_name], ".data") == 0)
			break;
	}
	printf("section:%s offset:%d\n", &strtab[shdr[i].sh_name], shdr[i].sh_offset);
	return shdr[i].sh_offset;
}
void main(int argc, char **argv)
{
	payload_t *payload = NULL;
	int fd = -1, ret = 0, offset = 0;
	struct stat st;
	char *mem = NULL;

	if (argc < 5) {
		printf("%s infile outfile interperter key\n", argv[0]);
		exit(-1);
	}

	payload = malloc(sizeof(payload_t));
	assert(payload != NULL);
	memset(payload, 0, sizeof(payload_t));

	strcpy(payload->inter, argv[3]);
	strcpy(payload->key, argv[4]);
	payload->key_len = strlen(argv[4]);

	fd = open(argv[1], O_RDONLY);
	assert(fd > 0);
	ret = fstat(fd, &st);
	assert(ret >= 0);
	if (st.st_size > MAX_DATA) {
		printf("infile is larger than %u,please 扩容\n", MAX_DATA);
		goto err;
	}

	ret = read(fd, payload->data, st.st_size);
	close(fd);

	enc_data(payload->data, payload->payload_len, payload->key, payload->key_len);

	fd = open(STUB_FILE, O_RDWR|O_CREAT);
	assert(fd >= 0);
	fstat(fd, &st);
	mem = mmap(NULL, st.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	offset = resolve_symbol("payload", mem);
	ret = memcpy(mem + offset, payload, sizeof(*payload));
	printf("before %p after:%p\n", mem+offset, ret);
	close(fd);

	return 0;
err:
	if (fd > 0)
		close(fd);
	if (payload != NULL)
		free(payload);
	exit(-1);
}
