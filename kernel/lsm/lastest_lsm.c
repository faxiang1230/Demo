//LSM need builtin kernel other than module
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

static int extlsm_dentry_open(struct file *file) {
	printk(KERN_INFO "open file:%s\n", file->f_path.dentry->d_name.name);
	//printk(KERN_INFO "user:%u open file:%s\n", from_kuid(&init_user_ns, cred->uid), file->f_path.dentry->d_name.name);
	return 0;
}
static struct security_hook_list lsmdemo[] = {
	LSM_HOOK_INIT(file_open, extlsm_dentry_open),
};

static int __init lsm_demo_init(void) {
	printk(KERN_INFO "lsm_demo init\n");

	security_add_hooks(lsmdemo, ARRAY_SIZE(lsmdemo), "lsm_demo");

	return 0;
}
DEFINE_LSM(lsm_demo) = {
    .name = "lsm_demo",
    .init = lsm_demo_init,
};

MODULE_DESCRIPTION("lastes LSM Demo");
MODULE_LICENSE("GPL");
