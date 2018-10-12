#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/printk.h>
//#include <>
#include <linux/sched.h>
void run_task(unsigned long value);
DECLARE_TASKLET(mytasklet, run_task, 0);
void run_task(unsigned long value) {
	printk(KERN_DEBUG "%s pid is:%u %s", __func__, current->pid, current->comm);
	printk(KERN_DEBUG "my tasklet\n");
}
static int __init my_tasklet_init(void) {
	tasklet_schedule(&mytasklet);
	return 0;
}
static void __exit my_tasklet_exit(void) {
}
module_init(my_tasklet_init);
module_exit(my_tasklet_exit);

MODULE_AUTHOR("Jianxing.Wang");
MODULE_DESCRIPTION("tasklet demo");
MODULE_LICENSE("GPL v2");
