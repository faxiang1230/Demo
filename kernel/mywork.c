#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/workqueue.h>

static void my_work(struct work_struct *work);

DECLARE_DELAYED_WORK(mywork, my_work);

static void my_work(struct work_struct *work) {
	printk(KERN_DEBUG "my delay work\n");
}

static int __init my_work_init(void) {
	printk(KERN_DEBUG "my work init\n");
	schedule_delayed_work(&mywork, 1000);
	return 0;
}
static void __exit my_work_exit(void) {
}

module_init(my_work_init);
module_exit(my_work_exit);

MODULE_AUTHOR("Jianxing.Wang");
MODULE_DESCRIPTION("work demo");
MODULE_LICENSE("GPL v2");
