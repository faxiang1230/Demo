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

#define DEBUG 1
typedef struct {
	unsigned int w_off, r_off;
	int size;
	unsigned long rsize, wsize;
	spinlock_t lock;
}logger_info;
static logger_info logger;
static char log_buff[MAX_LOG_ENTRY_NUM * LOG_ENTRY_SIZE];
static inline void *avail_log_addr(void) {
	void *addr = (logger.w_off << LOG_ADDR_STEP_OFFSET) + log_buff;;
	spin_lock(&logger.lock);
	logger.w_off = (logger.w_off + 1) % MAX_LOG_ENTRY_NUM;
	if(++logger.size > MAX_LOG_ENTRY_NUM) {
		logger.size = MAX_LOG_ENTRY_NUM;
		logger.r_off = logger.w_off;
		//if(!printk_ratelimited())
			pr_notice("%s log cache is full\n", __func__);
		
	}
	spin_unlock(&logger.lock);
	return addr;
}
void audit_log(const char *fmt, ...)
{
        va_list args;
	void *addr = NULL;

	addr = avail_log_addr(); 
        va_start(args, fmt);
        vsnprintf(addr, LOG_ENTRY_SIZE, fmt, args);
        va_end(args);
	logger.wsize++;
	//if(!printk_ratelimit())
	//	pr_notice("%s logger head addr:%p tail:%p\n", __func__, logger.head, logger.tail);
        return;

}
#if DEBUG
static void test_work(struct work_struct *work);

DECLARE_DELAYED_WORK(testwork1, test_work);
DECLARE_DELAYED_WORK(testwork2, test_work);
DECLARE_DELAYED_WORK(testwork3, test_work);
DECLARE_DELAYED_WORK(testwork4, test_work);

static void test_work(struct work_struct *work) {
        //printk(KERN_DEBUG "my delay work\n");
	audit_log("hello zslog %s %d\n", "abcdefg", 0x12345678);
	schedule_delayed_work(work, 100);
}
#endif
static ssize_t logger_aio_write(struct file *file, const char __user * buf, size_t len, loff_t *ppos) {
	pr_err("%s try to write to logger\n", current->comm);
	pr_notice("%s NOTICE: empty logger\n", __func__);
	logger.r_off = logger.w_off;
	logger.size = 0;
	return 0;
}
static ssize_t logger_read(struct file *file, char __user *buf,
					size_t count, loff_t *pos) {
	size_t size = (count >> LOG_ADDR_STEP_OFFSET);
	size_t part1, part2;
	if(logger.size == 0) {
		pr_info("%s no log cached\n", __func__);
		return 0;
	}
	//pr_notice("%s logger head addr:%p tail:%p\n", __func__, logger.head, logger.tail);
	if(size > 0) {
		if(size >= logger.size)
			size = logger.size;

		if((logger.r_off + size) > MAX_LOG_ENTRY_NUM) {
			part1 = MAX_LOG_ENTRY_NUM - logger.r_off;
			part2 = size - part1;
			if(copy_to_user(buf, (logger.r_off << LOG_ADDR_STEP_OFFSET) + log_buff, (part1 << LOG_ADDR_STEP_OFFSET)) == 0) {

				spin_lock(&logger.lock);
				logger.r_off = 0;
				logger.size -= part1;
				spin_unlock(&logger.lock);
				logger.rsize += part1;

				if(copy_to_user(buf + (part1 << LOG_ADDR_STEP_OFFSET), log_buff, (part2 << LOG_ADDR_STEP_OFFSET)) == 0) {
					spin_lock(&logger.lock);
					logger.r_off += part2;
					logger.size -= part2;
					spin_unlock(&logger.lock);
					//pr_info("%s user copy %d bytes\n", __func__, size);
					logger.rsize += part2;
					return (size << LOG_ADDR_STEP_OFFSET);
				}
				//pr_info("%s user copy part %d bytes\n", __func__, part);
				return (part1 << LOG_ADDR_STEP_OFFSET);
			} else {
				//if(!printk_ratelimit())
				//pr_info("%s user copy failed\n", __func__);
				goto copy_failed;
			}
		} else {
			if(copy_to_user(buf, (logger.r_off << LOG_ADDR_STEP_OFFSET) + log_buff, size << LOG_ADDR_STEP_OFFSET) == 0) {

				spin_lock(&logger.lock);
				logger.r_off += size;
				logger.size -= size;
				spin_unlock(&logger.lock);

				//if(!printk_ratelimit())
				//pr_info("%s user copy %d bytes\n", __func__, (int)size);
				logger.rsize += size;
				pr_info("%s user copy %d bytes\n", __func__, size);
				return (size << LOG_ADDR_STEP_OFFSET);
			} else {
				//if(!printk_ratelimit())
				//pr_err("%s copy bytes to user failed\n", __func__);
				goto copy_failed;
			}
		}
	}
	//if(!printk_ratelimit())
	pr_err("%s min size is 128 bytes,failed to read\n", __func__);
	return -1;
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
	spin_lock_init(&logger.lock);
	logger.w_off = logger.r_off = 0;
	logger.size = 0;
	logger.wsize = logger.rsize = 0;
#if DEBUG
	schedule_delayed_work(&testwork1, 1000);
	schedule_delayed_work(&testwork2, 1000);
	schedule_delayed_work(&testwork3, 1000);
	schedule_delayed_work(&testwork4, 1000);
#endif
	return 0;
err1:
	return ret;
}
module_init(logger_init);
static void __exit logger_exit(void) {
#if DEBUG
	cancel_delayed_work(&testwork1);	
	cancel_delayed_work(&testwork2);	
	cancel_delayed_work(&testwork3);	
	cancel_delayed_work(&testwork4);
#endif	
	misc_deregister(&zs_logger);
}
module_exit(logger_exit);
module_param_named(entry_num, logger.size, int, 0444);
module_param_named(wsize, logger.wsize, ulong, 0444);
module_param_named(rsize, logger.rsize, ulong, 0444);
MODULE_AUTHOR("Jianxing.Wang");
MODULE_DESCRIPTION("ZShield Log");
MODULE_LICENSE("GPL v2");
