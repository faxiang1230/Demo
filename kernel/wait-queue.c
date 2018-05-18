#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/workqueue.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/sched/signal.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/miscdevice.h>
#include <linux/of_device.h>

#define FIFO_SIZE 1024

typedef struct {
	struct mutex myfifo_lock;
	int current_count;
	char mem[FIFO_SIZE];
	wait_queue_head_t r_wait;
	wait_queue_head_t w_wait;
	struct miscdevice miscdev;
}myfifo_t;
static myfifo_t *g_fifo;
static int myfifo_open(struct inode *ip, struct file *fp)
{
	return 0;
}
static ssize_t myfifo_read(struct file *fp, char __user *buf,
		size_t nbytes, loff_t *ppos) {
	int ret = 0;
	DECLARE_WAITQUEUE(wait, current);
	add_wait_queue(&g_fifo->r_wait, &wait);

	while(g_fifo->current_count == 0) {
        if (fp->f_flags & O_NONBLOCK) {
            ret = -EAGAIN;
            goto out;
        }

		set_current_state(TASK_INTERRUPTIBLE);
		if(signal_pending(current)) {
			ret = -ERESTARTSYS;
			goto out;
		}	
		pr_info("my_fifo no data,sleep\n");
		schedule();
	}
	pr_info("my_fifo have %d data\n", g_fifo->current_count);
	if(nbytes > g_fifo->current_count)
		nbytes = g_fifo->current_count;

	if(!copy_to_user(buf, g_fifo->mem, nbytes)) {
		pr_info("my_fifo read %d data\n", nbytes);
		g_fifo->current_count -= nbytes;
		memcpy(g_fifo->mem, g_fifo->mem + nbytes, g_fifo->current_count);
		ret = nbytes;
		wake_up_interruptible(&g_fifo->w_wait);
	}
out:
	set_current_state(TASK_RUNNING);
	remove_wait_queue(&g_fifo->r_wait, &wait);
	pr_info("my_fifo read %d bytes, leave %d bytes\n", ret, g_fifo->current_count);
	return ret;
}

static ssize_t myfifo_write(struct file *fp, const char __user *buffer,
				  size_t nbytes, loff_t *ppos) {
	int ret = 0;

	DECLARE_WAITQUEUE(wait, current);
	add_wait_queue(&g_fifo->w_wait, &wait);
retry:
	if((nbytes + g_fifo->current_count) > FIFO_SIZE) {
		pr_info("my_fifo write %d data and current:%d\n", nbytes, g_fifo->current_count);
		copy_from_user(g_fifo->mem + g_fifo->current_count, buffer, FIFO_SIZE - g_fifo->current_count);
		nbytes -= (FIFO_SIZE - g_fifo->current_count);
		g_fifo->current_count = FIFO_SIZE;
		ret = FIFO_SIZE - g_fifo->current_count;
		wake_up_interruptible(&g_fifo->r_wait);
		while(g_fifo->current_count == FIFO_SIZE) {
			set_current_state(TASK_INTERRUPTIBLE);
			schedule();
			set_current_state(TASK_RUNNING);
			goto retry;
		}
	} else {
		pr_info("my_fifo write %d data\n", nbytes);
		copy_from_user(g_fifo->mem + g_fifo->current_count, buffer, nbytes);	
		ret = nbytes;
		g_fifo->current_count += nbytes;
		wake_up_interruptible(&g_fifo->r_wait);
	}
	remove_wait_queue(&g_fifo->w_wait, &wait);
	return ret;
}
static struct file_operations myfifo_ops = {
	.owner = THIS_MODULE,
	.read = myfifo_read,
	.write = myfifo_write,
	.open = myfifo_open,
};
static int __init myfifo_init(void) {
	g_fifo = kzalloc(sizeof(myfifo_t), GFP_KERNEL);
	mutex_init(&g_fifo->myfifo_lock);
	g_fifo->current_count = 0;
	init_waitqueue_head(&g_fifo->r_wait);
	init_waitqueue_head(&g_fifo->w_wait);
	
	g_fifo->miscdev.name = "myfifo";
	g_fifo->miscdev.minor = MISC_DYNAMIC_MINOR;
	g_fifo->miscdev.fops = &myfifo_ops;
	misc_register(&g_fifo->miscdev);

	printk(KERN_INFO "my fifo init\n");
	return 0;
}

static void __exit myfifo_exit(void) {
	misc_deregister(&g_fifo->miscdev);
}

module_init(myfifo_init);
module_exit(myfifo_exit);

MODULE_AUTHOR("Jianxing.Wang");
MODULE_DESCRIPTION("fifo demo");
MODULE_LICENSE("GPL v2");
