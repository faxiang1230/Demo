/*************************************************************************
    > File Name: queue.h
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2019年11月22日 星期五 18时52分28秒
 ************************************************************************/
#ifndef __LOG_QUEUE__
#define __LOG_QUEUE__
struct watch_queue {
	struct rcu_head		rcu;
	struct address_space	mapping;
	const struct cred	*cred;		/* Creds of the owner of the queue */
	struct watch_filter __rcu *filter;
	wait_queue_head_t	waiters;
	struct hlist_head	watches;	/* Contributory watches */
	struct kref		usage;		/* Object usage count */
	spinlock_t		lock;
	bool			defunct;	/* T when queues closed */
	u8			nr_pages;	/* Size of pages[] */
	u8			flag_next;	/* Flag to apply to next item */
#ifdef DEBUG_WITH_WRITE
	u8			debug;
#endif
	u32			size;
	struct watch_queue_buffer *buffer;	/* Pointer to first record */

	/* The mappable pages.  The zeroth page holds the ring pointers. */
	struct page		**pages;
};
#endif /* __LOG_QUEUE__ */
