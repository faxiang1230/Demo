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

struct log_queue {
	int nr_pages;
	volatile int head, head_next, tail, tail_next;
	struct page **pages;
	char *buffer;
	struct address_space	mapping;
	char buf[0];
};
static int log_queue_release(struct inode *inode, struct file *file)
{
	struct log_queue *queue = file->private_data;
	kfree(queue);
	return 0;
}

static int log_queue_open(struct inode *inode, struct file *file)
{
	struct log_queue *wqueue;

	wqueue = kzalloc(sizeof(*wqueue), GFP_KERNEL);
	if (!wqueue)
		return -ENOMEM;

	file->private_data = wqueue;

	return 0;
}
#define SET_PAGE	_IOW('t', 1, int)
#define GET_PAGE	_IOR('t', 2, int)
static long log_queue_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
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
	printk("%s\n", __func__);
}
static void log_vm_close(struct vm_area_struct * area)
{
	printk("%s\n", __func__);
}
static int log_vm_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
	printk("%s\n", __func__);
	return VM_FAULT_NOPAGE;
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
	if ((vma->vm_end - vma->vm_start > queue->nr_pages * PAGE_SIZE)) {
		return -EINVAL;
	}
	vma->vm_ops = &log_vm_ops;
	vma->vm_flags |= VM_DONTEXPAND | VM_DONTDUMP;
	vma->vm_private_data = file->private_data;

	queue->pages = kcalloc(queue->nr_pages, sizeof(struct page *), GFP_KERNEL);
	if (!queue->pages)
		goto err;

	for (i = 0; i < queue->nr_pages; i++) {
		queue->pages[i] = alloc_page(GFP_KERNEL | __GFP_ZERO);
		if (!queue->pages[i])
			goto err_some_pages;
		queue->pages[i]->mapping = &queue->mapping;
		SetPageUptodate(queue->pages[i]);
	}

	buf = vmap(queue->pages, queue->nr_pages, VM_MAP, PAGE_SHARED);
	if (!buf)
		goto err_some_pages;
	queue->buffer = buf;
	memcpy(buf, "hello", sizeof("hello"));
	return 0;

err_some_pages:
	kfree(queue->pages);
	for(i = 0; i < queue->nr_pages; i++) {
		ClearPageUptodate(queue->pages[i]);
		put_page(queue->pages[i]);
	}
err:
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
	.unlocked_ioctl	= log_queue_ioctl,
	.poll		= log_queue_poll,
	.mmap		= log_queue_mmap,
#ifdef DEBUG_WITH_WRITE
	.write		= log_queue_write,
#endif
	.llseek		= log_queue_llseek,
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
