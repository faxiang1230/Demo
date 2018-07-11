#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/workqueue.h>
#include <linux/proc_fs.h>
#include <linux/kmsg_dump.h>

char spstore_path[1024];

#ifdef CONFIG_PROC_FS
static ssize_t write_sysrq_trigger(struct file *file, const char __user *buf,
				   size_t count, loff_t *ppos)
{
	struct file *file;
	static const struct open_flags flags = {
		.open_flag = O_LARGEFILE | O_RDWR,
		.acc_mode = MAY_OPEN,
		.intent = LOOKUP_OPEN
	};

	if (count) {
		char c;

		if (copy_from_user(spstore, buf, count))
			return -EFAULT;
		file = do_filp_open(AT_FDCWD, spstore_path, &open_exec_flags, LOOKUP_FOLLOW);
		if (IS_ERR(file))
			goto out;
		
	}
	return count;
out:
	return -1;
}
static ssize_t read_spstore_patch(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
	if(copy_to_user(buf, spstore, strlen(spstore) + 1))
		return -EFAULT;
	else
		return strlen(spstore) + 1;
}
static void spstore_dump(struct kmsg_dumper *dumper,
			enum kmsg_dump_reason reason)
{
	if (!kmsg_dump_get_buffer(dumper, true, dst, size, &len))
			break;
	
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
	kmsg_dump_register(&spstore_dumper);
}

#else

static inline void spstore_init_procfs(void)
{
}

#endif /* CONFIG_PROC_FS */

static int __init spstore_init(void) {
	spstore_init_procfs();
	return 0;
}
static void __exit spstore_exit(void) {
}

module_init(spstore_init);
module_exit(spstore_exit);

MODULE_AUTHOR("Jianxing.Wang");
MODULE_DESCRIPTION("Simple PSTORE");
MODULE_LICENSE("GPL v2");
