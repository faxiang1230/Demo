/*
 * * You will see the trace data in /var/log/messages and on the console
 * * whenever do_fork() is invoked to create a new process.
 * */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/slab.h>

//定义要Hook的函数，本例中do_fork
static struct kprobe kp = 
{
	.symbol_name = "vfs_open",
};

static int donotexec(void) {
	/* if not a root process and kill is enabled, kill it */
#if 0
	if (tpe_kill && get_task_uid(current)) {
		(void)send_sig_info(SIGKILL, NULL, current);
		/* only kill the parent if it isn't root */
		if (get_task_uid(get_task_parent(current)))
			(void)send_sig_info(SIGKILL, NULL, get_task_parent(current));
	}
#endif
	printk(KERN_INFO "process[%s] open file refused\n", current->group_leader->comm);
	return -EACCES;
}

static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
	struct path *path = (struct path *)regs->di;
	struct file *file = (struct file *)regs->si;
	char *buff = kmalloc(256, GFP_KERNEL);
	char *ptr = NULL;
	memset(buff, 0, 256);

	ptr = d_path(path, buff, 256);
	if(ptr)
		printk(KERN_INFO "process(%s) open file:%s\n", current->group_leader->comm, ptr);
	kfree(buff);
	regs->ip = (unsigned long)donotexec;
	return 0;
}

static void handler_post(struct kprobe *p, struct pt_regs *regs, unsigned long flags)
{  
	struct path *path = (struct path *)regs->di;
	struct file *file = (struct file *)regs->si;
	char *buff = kmalloc(256, GFP_KERNEL);
	char *ptr = NULL;
	memset(buff, 0, 256);

	ptr = d_path(path, buff, 256);
	if(ptr)
		printk(KERN_INFO "process(%s) open file:%s\n", current->group_leader->comm, ptr);
	kfree(buff);
	//return -ENOENT;
}

static int handler_fault(struct kprobe *p, struct pt_regs *regs, int trapnr)
{
	printk(KERN_INFO "fault_handler: p->addr = 0x%p, trap #%dn",
			p->addr, trapnr);
	regs->ip = (unsigned long)donotexec;
	return 0;
}

/*
 * 内核模块加载初始化，这个过程和windows下的内核驱动注册分发例程很类似
 */
static int __init kprobe_init(void)
{
	int ret;
	kp.pre_handler = handler_pre;
#if 1
	kp.post_handler = handler_post;
	//kp.fault_handler = handler_fault;
#endif

	ret = register_kprobe(&kp);
	if (ret < 0) 
	{
		printk(KERN_INFO "register_kprobe failed, returned %d\n", ret);
		return ret;
	}
	printk(KERN_INFO "Planted kprobe at %p\n", kp.addr);
	return 0;
}

static void __exit kprobe_exit(void)
{
	unregister_kprobe(&kp);
	printk(KERN_INFO "kprobe at %p unregistered\n", kp.addr);
}

module_init(kprobe_init)
module_exit(kprobe_exit)
MODULE_LICENSE("GPL");
