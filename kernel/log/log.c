#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/workqueue.h>
#include <linux/printk.h>
#include <linux/string.h>
#include <linux/list.h>
#include <stdarg.h>
#include <linux/miscdevice.h>
#include <linux/moduleparam.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/sched.h>

#define LOG_ENTRY_SIZE		128
#define LOG_ADDR_STEP_OFFSET	7
#define MAX_LOG_ENTRY_NUM	2048
#define ZSLOGGER	"zslog"
#define P2INT(x)	((unsigned long)x)

typedef struct {
	void *head, *tail, *highest;
	int size;
}logger_info;
static unsigned long rsize,wsize;
static logger_info logger;
static char log_buff[MAX_LOG_ENTRY_NUM * LOG_ENTRY_SIZE];
static int logger_size;
static inline void *avail_log_addr(void) {
	char *addr = logger.head;
	if(logger.head >= logger.highest) {
		logger.head = log_buff;
	} else {
		logger.head += LOG_ENTRY_SIZE;
	}
	if(logger.size < (MAX_LOG_ENTRY_NUM -1)) {
		logger.size += 1;
	} else {
		logger.size = MAX_LOG_ENTRY_NUM -1;
		logger.tail = logger.head;
	}
	wsize++;
	return addr;
}
void audit_log(const char *fmt, ...)
{
        va_list args;
	void *addr = NULL;

	addr = avail_log_addr(); 
        WARN_ON(addr > logger.highest); 
        va_start(args, fmt);
        vsnprintf(addr, LOG_ENTRY_SIZE, fmt, args);
        va_end(args);
	logger_size = logger.size;
	//if(!printk_ratelimit())
	//	pr_notice("%s logger head addr:%p tail:%p\n", __func__, logger.head, logger.tail);
        return;

}
static void test_work(struct work_struct *work);

DECLARE_DELAYED_WORK(testwork, test_work);

static void test_work(struct work_struct *work) {
        //printk(KERN_DEBUG "my delay work\n");
	audit_log("hello zslog %s %d", "abcdefg", 0x12345678);
	schedule_delayed_work(&testwork, 10);
}
static ssize_t logger_aio_write(struct file *file, const char __user * buf, size_t len, loff_t *ppos) {
	pr_err("%s try to write to logger\n", current->comm);
	pr_notice("%s NOTICE: empty logger\n", __func__);
	return -1;
}
static ssize_t logger_read(struct file *file, char __user *buf,
					size_t count, loff_t *pos) {
	size_t size = (count & (~0x7f));
	logger_size = logger.size;
	size_t part;
	if(logger.size == 0) {
		pr_info("%s no log cached\n", __func__);
		return 0;
	}
	//pr_notice("%s logger head addr:%p tail:%p\n", __func__, logger.head, logger.tail);
	if(size > 0) {
		if(size >= (logger.size * LOG_ENTRY_SIZE))
			size = (logger.size * LOG_ENTRY_SIZE);

		if((logger.tail + size) > logger.highest) {
			part = logger.highest - logger.tail;
			if(copy_to_user(buf, logger.tail, part) == 0) {
				logger.tail = log_buff;
				logger.size -= (part >> 7);
				if(copy_to_user(buf + part, logger.tail, size - part) == 0) {
					logger.tail += (size - part);
					logger.size -=((size - part) >> 7);
					pr_info("%s user copy %d bytes\n", __func__, size);
					rsize += size;
					return size;
				}
				pr_info("%s user copy part %d bytes\n", __func__, part);
				rsize += part;
				return part;
			} else {
				if(!printk_ratelimit())
				pr_info("%s user copy failed\n", __func__);
				goto copy_failed;
			}
		} else {
			if(copy_to_user(buf, logger.tail, size) == 0) {
				logger.tail += size;
				logger.size -= (size >> 7);
				//if(!printk_ratelimit())
				//pr_info("%s user copy %d bytes\n", __func__, (int)size);
				rsize += size;
				pr_info("%s user copy %d bytes\n", __func__, size);
				return size;
			} else {
				//if(!printk_ratelimit())
				//pr_err("%s copy bytes to user failed\n", __func__);
				goto copy_failed;
			}
		}
	}
	//if(!printk_ratelimit())
		pr_err("%s invalid size\n", __func__);
copy_failed:
	return 0;
}
static int logger_open(struct inode *inode, struct file *file) {
	//check who read the logger
	return 0;
}

struct file_operations zslogger_fops = {
	.read	= logger_read,
	.write	= logger_aio_write,
	.open	= logger_open,
};
static struct miscdevice zs_logger = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= ZSLOGGER,
	.fops	= &zslogger_fops,
	.parent	= NULL,
	.mode	= 0777,
};
static int __init logger_init(void) {
	int ret = 0;
	ret = misc_register(&zs_logger);
	if (ret < 0) {
		pr_err("register misc device failed\n");
		goto err1;
	}
	logger.head = logger.tail = log_buff;
	logger.highest = log_buff + ((MAX_LOG_ENTRY_NUM -1) * LOG_ENTRY_SIZE);
	logger.size = 0;
	wsize = rsize = 0;
	pr_err("log_buff addr:%p", log_buff);
	schedule_delayed_work(&testwork, 1000);
	return 0;
err1:
	return ret;
}
module_init(logger_init);
static void __exit logger_exit(void) {
	cancel_delayed_work(&testwork);	
	misc_deregister(&zs_logger);
}
module_exit(logger_exit);
module_param(logger_size, int, 0444);
module_param(wsize, ulong, 0444);
module_param(rsize, ulong, 0444);
MODULE_AUTHOR("Jianxing.Wang");
MODULE_DESCRIPTION("ZShield Log");
MODULE_LICENSE("GPL v2");
