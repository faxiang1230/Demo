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

struct log_queue {
	int nr_pages;
	volatile int head, head_next, tail, tail_next;
	struct page **pages;
	char *buffer;
	struct address_space mapping;
	char buf[0];
};

#define NUM (100000000UL)
#define OFFSET  16

static void my_work(struct work_struct *work);
DECLARE_DELAYED_WORK(mywork, my_work);
static void my_work(struct work_struct *work) {
	int flags_old, flags_new;
	long long i = 0;
	struct log_queue *queue = (struct log_queue *)(atomic_read(work->data));
	int *val = (int *)(queue->buffer + OFFSET);
	printk(KERN_ERR "begin %d\n", *val);

	for (i = 0; i < NUM; i++) {
		do {
			flags_old = atomic_read(val);
			flags_new = flags_old++;
		} while (atomic_cmpxchg(val, flags_old, flags_new) != flags_old);
	}
	printk(KERN_ERR "end %d\n", *val);
}

static int log_queue_set_page_dirty(struct page *page)
{
	SetPageDirty(page);
	return 0;
}

static const struct address_space_operations log_queue_aops = {
	.set_page_dirty	= log_queue_set_page_dirty,
};
static int log_queue_release(struct inode *inode, struct file *file)
{
	printk(KERN_ERR "%s\n", __func__);
	struct log_queue *queue = file->private_data;
	kfree(queue);
	return 0;
}

static int log_queue_open(struct inode *inode, struct file *file)
{
	struct log_queue *wqueue;

	printk(KERN_ERR "%s\n", __func__);
	wqueue = kzalloc(sizeof(*wqueue), GFP_KERNEL);
	if (!wqueue)
		return -ENOMEM;

	wqueue->mapping.a_ops = &log_queue_aops;
	wqueue->mapping.i_mmap = RB_ROOT;

	file->private_data = wqueue;

	return 0;
}

#define SET_PAGE	_IOW('t', 1, int)
#define GET_PAGE	_IOR('t', 2, int)
static long log_queue_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	printk(KERN_ERR "%s\n", __func__);
	long ret = -1;
	struct log_queue *queue = file->private_data;

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
static int log_vm_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
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

	printk(KERN_ERR "%s %d pages start:%lx end:%lx\n", __func__, queue->nr_pages, vma->vm_start, vma->vm_end);
	if ((vma->vm_end - vma->vm_start > queue->nr_pages * PAGE_SIZE)) {
		return -EINVAL;
	}

	vma->vm_ops = &log_vm_ops;
	vma->vm_flags |= VM_DONTEXPAND | VM_DONTDUMP;
	vma->vm_private_data = file->private_data;

	printk(KERN_ERR "%s 1\n", __func__);
	queue->pages = kcalloc(queue->nr_pages, sizeof(struct page *), GFP_KERNEL);
	if (!queue->pages)
		goto err;

	printk(KERN_ERR "%s 2\n", __func__);
	for (i = 0; i < queue->nr_pages; i++) {
		queue->pages[i] = alloc_page(GFP_KERNEL | __GFP_ZERO);
		if (!queue->pages[i])
			goto err_some_pages;
		//queue->pages[i]->mapping = &queue->mapping;
		printk(KERN_ERR "%s %p insert page\n", __func__, vma->vm_start + (i * PAGE_SIZE));
		vm_insert_page(vma, vma->vm_start + (i * PAGE_SIZE), queue->pages[i]);
	}
	printk(KERN_ERR "%s 3\n", __func__);

	buf = vmap(queue->pages, queue->nr_pages, VM_MAP, PAGE_KERNEL);
	if (!buf)
		goto err_some_pages;
	printk(KERN_ERR "%s 4\n", __func__);
	queue->buffer = buf;
	memcpy(buf, "hello", sizeof("hello"));
	printk(KERN_ERR "%s 5\n", __func__);
	atomic_set(&my_work.data, queue);
	schedule_work(&my_work);
	return 0;

err_some_pages:
	printk(KERN_ERR "%s err1\n", __func__);
	kfree(queue->pages);
	for(i = 0; i < queue->nr_pages; i++) {
		put_page(queue->pages[i]);
	}
err:
	printk(KERN_ERR "%s err2\n", __func__);
	return -1;
}
#ifdef DEBUG_WITH_WRITE
static ssize_t log_queue_write(struct file *file, const char __user *cred, size_t, loff_t *offset)
{
	return 0;
}
#endif
static loff_t log_queue_llseek (struct file *file, loff_t offset, int whence)
{
	return 0;
}
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
