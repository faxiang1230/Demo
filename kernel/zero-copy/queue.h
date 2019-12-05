/*************************************************************************
    > File Name: queue.h
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2019年11月22日 星期五 18时52分28秒
 ************************************************************************/
#ifndef __LOG_QUEUE__
#define __LOG_QUEUE__

#define nr_of_pages	2
#define SET_PAGE    _IOW('t', 1, int)
#define GET_PAGE    _IOR('t', 2, int)
#define NUM	(100000000UL)
#define OFFSET	16

#define LOG_ENTRY_SIZE	32
#define MAGIC_SIZE	8

#define TEST_LOG
//#define TEST_ATOMIC

char *demo_log = "012345678998765432100123456789";
char magic[MAGIC_SIZE]={"LOGQUEU"};
struct log_manager {
	char magic_start[MAGIC_SIZE];
	int nr_pages;
	int max_entry;
	atomic_t sender_count, consumer_count;
	atomic_t prod_head, prod_tail, cons_head, cons_tail;
#ifdef TEST_ATOMIC
	int test_count;
#endif
	char magic_end[MAGIC_SIZE];
};
#endif /* __LOG_QUEUE__ */
