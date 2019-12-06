#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h> /* copy_from_user */
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h> /* min */
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/rbtree.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include "queue.h"

struct log_queue {
	struct page **pages;
	char *buffer;
	int nr_pages;
	atomic_t refcount;
};

#ifdef TEST_LOG
struct task_struct *sender1 = NULL, *sender2 = NULL;
int log_isfull(struct log_manager *lm)
{
	return ((atomic_read(&lm->prod_head) + 1)%lm->max_entry) == atomic_read(&lm->cons_tail);
}
void log_update_sender_count(struct log_manager *lm)
{
	int old, new, error;
	atomic_t *val = (atomic_t *)(&lm->sender_count);
	
	error = atomic_read(val);
	do {
		old = error;
		new = old + 1;
		error = atomic_cmpxchg(val, old, new);
	} while (error != old);
}
int log_sender_header(struct log_manager *lm)
{
	int old, new, error;
	atomic_t *val = (atomic_t *)(&lm->prod_head);

	error = atomic_read(val);
	do {
		while (log_isfull(lm));
		old = error;
		new = (old + 1)%lm->max_entry;
		error = atomic_cmpxchg(val, old, new);
	} while (error != old);
	return old;	
}
static void log_sender_tail(struct log_manager *lm, int oldtail, int new)
{
	atomic_t *val = (atomic_t *)(&lm->prod_tail);
	int error;
	
	while(atomic_read(val) != oldtail);
	error = atomic_cmpxchg(val, oldtail, new);
	if (error != oldtail)
		printk(KERN_ERR "%s failed update prod tail:%d %d\n", __func__, oldtail, error);
}
static int send_log(void *data) {
	long long index = 0, i = 0, j = 0, new = 0;
	struct log_queue *queue = (struct log_queue *)data;
	struct log_manager *lm = (struct log_manager*)queue->buffer;
	char *log = NULL;

	for (j = 0; j < 100; j++) {
		for (i = 0; i < 1000; i++) {
			index = log_sender_header(lm);
			log = queue->buffer + PAGE_SIZE + (index*LOG_ENTRY_SIZE);
			snprintf(log, LOG_ENTRY_SIZE, "%s", demo_log);
			new = (index + 1)%lm->max_entry;
			log_sender_tail(lm, index, new);
			log_update_sender_count(lm);
		}
		//mdelay(10);
	}
	if (current == sender1) sender1 = NULL;
	if (current == sender2) sender2 = NULL;
	printk(KERN_INFO "send %d prod head:%d tail:%d\n", 
			atomic_read(&lm->sender_count),
			atomic_read(&lm->prod_head), atomic_read(&lm->prod_tail));
	
	return 0;
}
#endif
#ifdef TEST_ATOMIC
static int my_work(void *data) {
	int flags_old, flags_new, error;
	long long i = 0;
	struct log_queue *queue = (struct log_queue *)data;
	struct log_manager *lm = (struct log_manager *)queue->buffer;
	atomic_t *val = (atomic_t *)(&lm->test_count);

	for (i = 0; i < NUM; i++) {
		error = atomic_read(val);
		do {
			flags_old = error;
			flags_new = flags_old-1;
			error = atomic_cmpxchg(val, flags_old, flags_new);
		} while (error != flags_old);
	}
	printk(KERN_ERR "end %d\n", atomic_read(val));
	return 0;
}
#endif
static int log_queue_release(struct inode *inode, struct file *file)
{
	int i = 0;
	struct log_queue *queue = file->private_data;
	printk(KERN_ERR "%s\n", __func__);
#ifdef TEST_LOG
	if (sender1) kthread_stop(sender1);
	if (sender2) kthread_stop(sender2);
	sender1 = sender2 = NULL;
#endif
	for(i = 0; i < queue->nr_pages; i++) {
		put_page(queue->pages[i]);
	}
	kfree(queue->pages);
	kfree(queue);
	return 0;
}

static int log_queue_open(struct inode *inode, struct file *file)
{
	struct log_queue *queue;

	printk(KERN_ERR "%s\n", __func__);
	queue = kzalloc(sizeof(*queue), GFP_KERNEL);
	if (!queue)
		return -ENOMEM;

	file->private_data = queue;

	return 0;
}

static long log_queue_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	long ret = -1;
	struct log_queue *queue = file->private_data;
	printk(KERN_ERR "%s\n", __func__);

	switch(cmd) {
		case SET_PAGE:
			queue->nr_pages = arg;
			ret = 0;
			break;
		case GET_PAGE:
			ret = queue->nr_pages;
			break;
		default:
			break;
	}
	return ret;
}
static unsigned int log_queue_poll(struct file *file, struct poll_table_struct *poll)
{
	return 0;
}
static void log_vm_open(struct vm_area_struct * area)
{
	printk(KERN_ERR "%s\n", __func__);
}
static void log_vm_close(struct vm_area_struct * area)
{
	printk(KERN_ERR "%s\n", __func__);
}
static int log_vm_fault(struct vm_fault *vmf)
{
	printk(KERN_ERR "%s\n", __func__);
	return VM_FAULT_SIGBUS;
}
static struct vm_operations_struct log_vm_ops = {
	.open = log_vm_open,
	.close = log_vm_close,
	.fault = log_vm_fault,
};
static int log_queue_mmap (struct file *file, struct vm_area_struct *vma)
{
	int i = 0;
	char *buf = NULL;
	struct log_queue *queue = file->private_data;
	struct log_manager *lm = NULL;

	//printk(KERN_ERR "%s %d pages start:%lx end:%lx\n", __func__, queue->nr_pages, vma->vm_start, vma->vm_end);
	if ((vma->vm_end - vma->vm_start > queue->nr_pages * PAGE_SIZE)) {
		return -EINVAL;
	}

	vma->vm_ops = &log_vm_ops;
	vma->vm_flags |= VM_DONTEXPAND | VM_DONTDUMP;
	vma->vm_private_data = file->private_data;

	if (!queue->pages) {
		queue->pages = kcalloc(queue->nr_pages, sizeof(struct page *), GFP_KERNEL);
		if (!queue->pages)
			goto err;
	}

	for (i = 0; i < queue->nr_pages; i++) {
		queue->pages[i] = alloc_page(GFP_KERNEL | __GFP_ZERO);
		if (!queue->pages[i])
			goto err_some_pages;
		vm_insert_page(vma, vma->vm_start + (i * PAGE_SIZE), queue->pages[i]);
		atomic_inc(&queue->refcount);
	}

	if (!queue->buffer) {
		buf = vmap(queue->pages, queue->nr_pages, VM_MAP, PAGE_KERNEL);
		if (!buf) {
			goto err_some_pages;
			atomic_dec(&queue->refcount);
		}
		queue->buffer = buf;
	} else
		buf = queue->buffer;

	lm = (struct log_manager*)buf;
	memset(lm, 0, sizeof(*lm));
	lm->nr_pages = queue->nr_pages;
	lm->max_entry = (lm->nr_pages - 1) * PAGE_SIZE / LOG_ENTRY_SIZE;
	memcpy(lm->magic_start, magic, sizeof(lm->magic_start));
	memcpy(lm->magic_end, magic, sizeof(lm->magic_end));
#ifdef TEST_LOG
	sender1 = kthread_run(send_log, queue, "log_sender1");
	sender2 = kthread_run(send_log, queue, "log_sender2");
#endif
	
#ifdef TEST_ATOMIC
	kthread_run(my_work, queue, "log_count1");
	kthread_run(my_work, queue, "log_count2");
#endif
	return 0;

err_some_pages:
	printk(KERN_ERR "%s failed to alloc page\n", __func__);
	if (atomic_read(&queue->refcount) == 0) {
		printk(KERN_ERR "%s free pages\n", __func__);
		kfree(queue->pages);
		for(i = 0; i < queue->nr_pages; i++) {
			put_page(queue->pages[i]);
		}
	}
err:
	printk(KERN_ERR "%s failed to alloc memory\n", __func__);
	return -1;
}
#ifdef DEBUG_WITH_WRITE
static ssize_t log_queue_write(struct file *file, const char __user *cred, size_t, loff_t *offset)
{
	return 0;
}
static loff_t log_queue_llseek (struct file *file, loff_t offset, int whence)
{
	return 0;
}
#endif
static const struct file_operations log_queue_fops = {
	.owner		= THIS_MODULE,
	.open		= log_queue_open,
	.release	= log_queue_release,
	.mmap		= log_queue_mmap,
	.unlocked_ioctl	= log_queue_ioctl,
	.poll		= log_queue_poll,
#ifdef DEBUG_WITH_WRITE
	.write		= log_queue_write,
	.llseek		= log_queue_llseek,
#endif
};

static struct miscdevice log_queue_dev = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= "log_queue",
	.fops	= &log_queue_fops,
	.mode	= 0666,
};

static int log_queue_init(void)
{
	misc_register(&log_queue_dev);
	return 0;
}

static void log_queue_exit(void)
{
	misc_deregister(&log_queue_dev);
}

module_init(log_queue_init);
module_exit(log_queue_exit);

MODULE_AUTHOR("Jianxing.Wang");
MODULE_DESCRIPTION("mmap demo");
MODULE_LICENSE("GPL v2");
