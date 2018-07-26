#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/workqueue.h>
#include <linux/proc_fs.h>
#include <linux/kmsg_dump.h>
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
#include <linux/namei.h>

#define DEFAULT_PATH	"/home/linux/kdump_msg"
char spstore_path[1024];
static struct file *file;
char buff[4096];

#ifdef CONFIG_PROC_FS
static ssize_t write_spstore_path(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
	if (count) {
		if (copy_from_user(spstore_path, buf, count))
			return -EFAULT;
	}
	return count;
}
static ssize_t read_spstore_path(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
	if(copy_to_user(buf, spstore_path, strlen(spstore_path) + 1))
		return -EFAULT;
	else
		return strlen(spstore_path) + 1;
}
static void spstore_dump(struct kmsg_dumper *dumper,
			enum kmsg_dump_reason reason)
{
	int len = 0;
	if (!kmsg_dump_get_buffer(dumper, true, buff, 4096, &len))
		return;
	vfs_write(file, buff, len, 0);
	pr_err("%s write\n", __func__);
	return;
}
static const struct file_operations proc_spstore_trigger_operations = {
	.write		= write_spstore_path,
	.read		= read_spstore_path,
	.llseek		= noop_llseek,
};

static struct kmsg_dumper spstore_dumper = {
	.dump = spstore_dump,
};
static void spstore_init_procfs(void)
{
	if (!proc_create("spstore", S_IRWXU, NULL,
			 &proc_spstore_trigger_operations))
		pr_err("Failed to register proc interface\n");
}

#else

static inline void spstore_init_procfs(void)
{
}

#endif /* CONFIG_PROC_FS */
static int init_store_file(char *filename) {
	int error;
	file = filp_open(filename, O_RDWR | O_CREAT, 0);
	error = PTR_ERR(file);
	if (IS_ERR(file))
		goto out;
out:
	return 0;
}
static int __init spstore_init(void) {
	spstore_init_procfs();
	memcpy(spstore_path, DEFAULT_PATH, strlen(DEFAULT_PATH) + 1);
	init_store_file(DEFAULT_PATH);
	kmsg_dump_register(&spstore_dumper);
	return 0;
}
static void __exit spstore_exit(void) {
}

module_init(spstore_init);
module_exit(spstore_exit);

MODULE_AUTHOR("Jianxing.Wang");
MODULE_DESCRIPTION("Simple PSTORE");
MODULE_LICENSE("GPL v2");
