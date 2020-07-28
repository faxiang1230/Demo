/*************************************************************************
  > File Name: sort.c
  > Author: wangjx
 ************************************************************************/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <inttypes.h>
#include <errno.h>
#include <pthread.h>
#include <mm_malloc.h>


//#define FILE_LENGTH	(2L*1024L*1024L*1024L*1024L)
#define FILE_LENGTH	(8L*1024L*1024L*1024L)

#define BUF_LEN	(1024L*1024L*1024L)
typedef struct sort_info {
	char *filepath;
	uint64_t data_offset, data_length;
	int *hashtable;
	uint16_t hash_max_index;
} sort_info_t;

const int hashtable_max_index = (1 << 15);

void *sort_int(void *data)
{
	sort_info_t *info = (sort_info_t *) data;
	int fd = 0, *ptr;
	char *buf = NULL;
	int64_t ret = 0, index = 0, offset = 0;
	
	buf = _mm_malloc(BUF_LEN, 4096);
	if (!buf) {
		printf("failed to malloc info\n");
		return NULL;
	}

	fd = open(info->filepath, O_RDONLY|O_DIRECT);
	if (fd < 0) {
		printf("failed to open file:%s, err:%s\n", info->filepath, strerror(errno));
		free(buf);
		return NULL;
	}
	lseek(fd, info->data_offset, SEEK_SET);

	for (index = 0; index < info->data_length; index += BUF_LEN) {
		ret = read(fd, buf, BUF_LEN);
		if (ret < BUF_LEN) {
			printf("failed to read file:%s, ret:%ld err:%s\n", info->filepath, ret, strerror(errno));
			break;
		}
		printf("read ret:%ld\n", ret);
		for (offset = 0; offset < ret; offset+=sizeof(int)) {
			ptr = (int *)(buf + offset);
			//printf("%d\n", *ptr);
			info->hashtable[*ptr]++;
		}
	}

	free(buf);
	close(fd);
	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	int *hashtable1 = NULL, *hashtable2 = NULL, *ptr = NULL;
	pthread_t p1, p2;
	uint16_t index = 0;
	uint64_t tmp = 0;
	int fd1 = 0, fd2 = 0, ret = 0, offset = 0;
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

	info1.filepath = argv[1];
	info1.data_offset = 0;
	info1.data_length = FILE_LENGTH >> 1;
	info1.hashtable = hashtable1;
	info1.hash_max_index = hashtable_max_index;
	pthread_create(&p1, NULL, sort_int, &info1);

	info2.filepath = argv[1];
	info2.data_offset = FILE_LENGTH >> 1;
	info2.data_length = FILE_LENGTH >> 1;
	info2.hashtable = hashtable2;
	info2.hash_max_index = hashtable_max_index;
	pthread_create(&p2, NULL, sort_int, &info2);

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);

	fd2 = open(argv[2], O_CREAT | O_RDWR | O_TRUNC | O_DIRECT, 0666);
	if (fd2 < 0) {
		printf("Failed to create %s, error:%s\n", argv[2], strerror(errno));
		goto err;
	}

	origin_data = _mm_malloc(BUF_LEN, 4096);
	if (!origin_data) {
		printf("Failed to malloc info:%s\n", strerror(errno));
		goto err;
	}
	ptr = (int *)origin_data;
	offset = 0;
	for (index = 0; index < hashtable_max_index; index++) {
		if (hashtable1[index] > 0) {
			for (tmp = 0; tmp < hashtable1[index]; tmp++) {
				*ptr++ = index;
				if (++offset >= BUF_LEN/sizeof(int)) {
					offset = 0;
					write(fd2, origin_data, BUF_LEN);
					ptr = (int *)origin_data;
				}
			}
		}
		if (hashtable2[index] > 0) {
			for (tmp = 0; tmp < hashtable2[index]; tmp++) {
				*ptr++ = index;
				if (++offset >= BUF_LEN/sizeof(int)) {
					offset = 0;
					write(fd2, origin_data, BUF_LEN);
					ptr = (int *)origin_data;
				}
			}
		}
	}
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
