#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/miscdevice.h>
static int watch_queue_release(struct inode *inode, struct file *file)
{
	struct watch_filter *wfilter;
	struct watch_queue *wqueue = file->private_data;
	int i;

	watch_queue_clear(wqueue);

	if (wqueue->buffer)
		vunmap(wqueue->buffer);

	for (i = 0; i < wqueue->nr_pages; i++) {
		ClearPageUptodate(wqueue->pages[i]);
		wqueue->pages[i]->mapping = NULL;
		__free_page(wqueue->pages[i]);
	}

	wfilter = rcu_dereference_protected(wqueue->filter, true);
	if (wfilter)
		kfree_rcu(wfilter, rcu);
	kfree(wqueue->pages);
	put_cred(wqueue->cred);
	put_watch_queue(wqueue);
	return 0;
}

static int log_queue_open(struct inode *inode, struct file *file)
{
	struct log_queue *wqueue;

	wqueue = kzalloc(sizeof(*wqueue), GFP_KERNEL);
	if (!wqueue)
		return -ENOMEM;

	wqueue->mapping.a_ops = &log_queue_aops;
	wqueue->mapping.i_mmap = RB_ROOT_CACHED;
	init_rwsem(&wqueue->mapping.i_mmap_rwsem);
	spin_lock_init(&wqueue->mapping.private_lock);

	kref_init(&wqueue->usage);
	spin_lock_init(&wqueue->lock);
	init_waitqueue_head(&wqueue->waiters);
	wqueue->cred = get_cred(file->f_cred);

	file->private_data = wqueue;

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
	.llseek		= no_llseek,
};

static struct miscdevice log_queue_dev = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= "log_queue",
	.fops	= &log_queue_fops,
	.mode	= 0666,
};

static int __init__ log_queue_init(void)
{
	misc_register(&log_queue_dev);
}

static void __exti__ log_queue_exit(void)
{
	misc_unregister(&log_queue_dev);
}

module_init(log_queue_init);
module_exit(log_queue_exit);

MODULE_AUTHOR("Jianxing.Wang");
MODULE_DESCRIPTION("work demo");
MODULE_LICENSE("GPL v2");
