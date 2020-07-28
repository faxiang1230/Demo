/*************************************************************************
  > File Name: sort.c
  > Author: wangjx
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <inttypes.h>
#include <errno.h>
#include <pthread.h>


#define FILE_LENGTH	(8UL*1024UL*1024UL*1024UL)
//#define FILE_LENGTH	(2UL*1024UL*1024UL*1024UL*1024UL)

typedef struct sort_info {
	char *data;
	uint64_t data_length;
	int *hashtable;
	uint16_t hash_max_index;
} sort_info_t;
const int hashtable_max_index = (1 << 15);

void *sort_int(void *data)
{
	sort_info_t *info = (sort_info_t *) data;
	int *ptr = (int *)info->data;
	uint64_t max_count = info->data_length / sizeof(int);
	int index = 0;

	for (; index < max_count; index++, ptr++) {
		info->hashtable[*ptr]++;
	}
	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	int *hashtable1 = NULL, *hashtable2 = NULL, *ptr = NULL;
	pthread_t p1, p2;
	uint16_t index = 0;
	uint64_t tmp = 0;
	int fd1 = 0, fd2 = 0, ret = 0;
	sort_info_t info1, info2;
	struct stat st = { 0 };
	char *origin_data;

	if (argc < 3) {
		printf("\tUsage:%s origin_file sorted_file\n", argv[0]);
		return -1;
	}

	hashtable1 = calloc(1, (1 << 15) * sizeof(uint64_t));
	hashtable2 = calloc(1, (1 << 15) * sizeof(uint64_t));
	if (!hashtable1 || !hashtable2) {
		printf("failed to malloc memory\n");
		goto err;
	}

	fd1 = open(argv[1], O_RDONLY);
	if (fd1 < 0) {
		printf("failed to open file:%s, err:%s\n", argv[1], strerror(errno));
		goto err;
	}

	ret = fstat(fd1, &st);
	if (ret < 0) {
		printf("failed to stat file:%s\n", argv[1]);
		goto err;
	}
	printf("file %s size:%ld\n", argv[1], st.st_size);
	assert(st.st_size == FILE_LENGTH);

	origin_data = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd1, 0);

	info1.data = origin_data;
	info1.data_length = st.st_size >> 1;
	info1.hashtable = hashtable1;
	info1.hash_max_index = hashtable_max_index;
	pthread_create(&p1, NULL, sort_int, &info1);

	info2.data = origin_data + (st.st_size >> 1);
	info2.data_length = st.st_size >> 1;
	info2.hashtable = hashtable2;
	info2.hash_max_index = hashtable_max_index;
	pthread_create(&p2, NULL, sort_int, &info2);

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	munmap(origin_data, st.st_size);
	close(fd1);

	fd2 = open(argv[2], O_CREAT | O_RDWR, 0666);
	if (fd2 < 0) {
		printf("Failed to create %s, error:%s\n", argv[2], strerror(errno));
		goto err;
	}
	ftruncate(fd2, st.st_size);

	origin_data = mmap(NULL, st.st_size, PROT_WRITE, MAP_SHARED, fd2, 0);
	if (origin_data == MAP_FAILED) {
		printf("Failed to mmap file:%s\n", strerror(errno));
		goto err;
	}
	ptr = (int *)origin_data;
	for (index = 0; index < hashtable_max_index; index++) {
		if (hashtable1[index] > 0) {
			for (tmp = 0; tmp < hashtable1[index]; tmp++)
				*ptr++ = index;
		}
		if (hashtable2[index] > 0) {
			for (tmp = 0; tmp < hashtable2[index]; tmp++)
				*ptr++ = index;
		}
	}
	munmap(origin_data, st.st_size);
	close(fd2);

	free(hashtable1);
	free(hashtable2);

	return 0;

err:
	if (hashtable1)
		free(hashtable1);
	if (hashtable2)
		free(hashtable2);
	if (fd1)
		close(fd1);
	if (fd2)
		close(fd2);

	return -1;
}
