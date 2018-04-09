#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/security.h>
#include <linux/mount.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/mman.h>
#include <asm/uaccess.h>
#include <linux/magic.h>
#include <linux/dcache.h>
#include <linux/file.h>
#include <linux/namei.h>
#include <linux/lsm_hooks.h>
#include <linux/cred.h>

static int extlsm_dentry_open(struct file *file, const struct cred *cred) {
	printk(KERN_INFO "user:%u open file:%s\n", from_kuid(&init_user_ns, cred->uid), file->f_path.dentry->d_name.name);
	return 0;
}
static struct security_hook_list selinux_hooks[] = {
	LSM_HOOK_INIT(file_open, extlsm_dentry_open),
};

static int __init my_lsm_init(void) {
	printk(KERN_INFO "my_lsm_init\n");
	if (!security_module_enable("my_lsm")) {
		return 0;
	}

	security_add_hooks(selinux_hooks, ARRAY_SIZE(selinux_hooks), "my_lsm");

	return 0;
}
static void __exit my_lsm_exit(void) {
	return;
}
module_init(my_lsm_init);
module_exit(my_lsm_exit);

MODULE_DESCRIPTION("LSM Log");
MODULE_LICENSE("GPL");
