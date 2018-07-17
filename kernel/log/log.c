#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/printk.h>
#include <linux/skbuff.h>
#include <linux/netlink.h>
#include <net/netlink.h>
#include <net/sock.h>
#include <linux/string.h>
#include <linux/freezer.h>
#include <linux/kthread.h>
#include <linux/list.h>
#include <stdarg.h>

#define NETLINK_SZLOG 30
#define MAX_MSGSIZE 1024
#define LOG_ENTRY_SIZE 128
#define MAX_LOG_ENTRY_NUM 64


struct sock *sz_nlsk = NULL;
static struct task_struct *ksz_task;
static struct list_head zslog_list,zslog_free_list;
static pid_t pid = 0;
static int free_entry = 0;

DECLARE_WAIT_QUEUE_HEAD(backlog_wait);

typedef struct {
	char log[LOG_ENTRY_SIZE];
}zslog_entry;
typedef struct {
	struct list_head list;
	struct sk_buff* skb;
}zslog;
void send_log(struct sk_buff *skb);
void init_log_release(void);
static void nl_recv(struct sk_buff *skb) {
	if(!skb){
		pr_err("%s recv NULL skbuff\n", __func__);
		goto out;
	}
	//Need to check the process's cred
	pid = NETLINK_CREDS(skb)->pid;
	pr_info("%d process register to szlog\n", pid);
out:
	return ;
}
int report(const char *fmt, ...) {
	zslog *log = NULL;
	va_list args;
	struct nlmsghdr *nlh;
	int avail;
	if(!sz_nlsk)
		return;
	if(free_entry >= 0) {
		log = list_entry(zslog_free_list.next, zslog, list);
		nlh = nlmsg_put(log->skb, 0, 0, 0, LOG_ENTRY_SIZE, 0);
		if(nlh == NULL) {
			pr_err("nlmsg_put err\n");
			return -ENOMEM;
		}
		//NETLINK_CB(log->skb).portid = 0;
		//NETLINK_CB(log->skb).dst_group = 0;
		avail = skb_tailroom(log->skb);
		if (avail == 0) { 
			pr_err("No Avail room\n");
		} else
			pr_err("avail %d bytes\n", avail);
		va_start(args, fmt);
		vsnprintf(NLMSG_DATA(nlh), avail , fmt, args);
//		vsnprintf(skb_tail_pointer(log->skb),  avail , fmt, args);
		va_end(args);
		//nlh->nlmsg_len = log->skb->len - NLMSG_HDRLEN;
		//nlmsg_unicast(sz_nlsk, log->skb, 0);
		list_del(&log->list);
		list_add_tail(&log->list, &zslog_list);
		free_entry--;
		wake_up_interruptible(&backlog_wait);
	} else {
		pr_err("sz log list is full,drop log\n");
		return -ENOMEM;
	}
	pr_info("report log\n");
}
static int ksz_thread(void *dummy)
{
	zslog *log = NULL;
	struct list_head *iter, *tmp;
	set_freezable();
	while (!kthread_should_stop()) {
		if(!list_empty(&zslog_list)) {
			list_for_each_safe(iter, tmp, &zslog_list) {
				log = list_entry(iter, zslog, list);
				send_log(log->skb);
				log->skb->tail = log->skb->data - log->skb->head;
				log->skb->sk = NULL;
				list_del(&log->list);
				list_add_tail(&log->list, &zslog_free_list);
				free_entry++;
				continue;
			}
		}
		wait_event_freezable(backlog_wait, (!list_empty(&zslog_list)) || kthread_should_stop());
	}
	return 0;
}
void init_log_release(void) {
	zslog *log = NULL;
	struct list_head *iter, *tmp;
	list_for_each_safe(iter, tmp, &zslog_free_list) {
		log = list_entry(iter, zslog, list);
		list_del(&log->list);
		nlmsg_free(log->skb);
		kfree(log);
		free_entry--;
	}
	if(!list_empty(&zslog_list)) {
		list_for_each_safe(iter, tmp, &zslog_list) {
			log = list_entry(iter, zslog, list);
		//	send_log(log->skb);
			list_del(&log->list);
			nlmsg_free(log->skb);
			kfree(log);
			free_entry--;
		}
	}
	if(free_entry == 0)
		pr_info("log mem free wholy\n");
	else
		pr_err("log mem free partly\n");
} 
int init_log_list(void) {
	int i = 0;
	zslog *log = NULL;
	INIT_LIST_HEAD(&zslog_free_list);
	INIT_LIST_HEAD(&zslog_list);
	for(i = 0; i < MAX_LOG_ENTRY_NUM; i++) {
		log = kmalloc(sizeof(zslog), GFP_KERNEL);
		if(!log) {
			pr_err("%s failed alloc memory\n", __func__);
			return -ENOMEM;
		}
		log->skb = nlmsg_new(LOG_ENTRY_SIZE, GFP_KERNEL);
		if(!log->skb) {
			pr_err("%s failed alloc memory for skb\n", __func__);
			return -ENOMEM;
		}
		list_add_tail(&log->list, &zslog_free_list);
		free_entry++;
	}
	return 0;
}
void send_log(struct sk_buff *skb) {
	netlink_unicast(sz_nlsk, skb, pid, MSG_DONTWAIT);
	return ;
}
static void my_work(struct work_struct *work);

DECLARE_DELAYED_WORK(mywork, my_work);

static void my_work(struct work_struct *work) {
        printk(KERN_DEBUG "my delay work\n");
	report("hello zslog");
	schedule_delayed_work(&mywork, 1000);
}

static int __init plog_init(void) {
	int ret = 0;
	struct netlink_kernel_cfg cfg = {
		.input = nl_recv,
	};

	sz_nlsk = netlink_kernel_create(&init_net, NETLINK_SZLOG, &cfg);
	if (sz_nlsk == NULL) {
		pr_err("cannot initialize netlink socket in namespace");
		ret = -ENOMEM;
		goto err1;
	}
	ret = init_log_list();
	if(ret < 0) {
		pr_err("init log failed\n");
		goto err2;
	}
	if (!ksz_task) {
		ksz_task = kthread_run(ksz_thread, NULL, "ksz_log");
		if (IS_ERR(ksz_task)) {
			ret = PTR_ERR(ksz_task);
			ksz_task = NULL;
			goto err3; 
		}    
	}
	schedule_delayed_work(&mywork, 1000);
	return 0;
err3:
	init_log_release();
err2:
	netlink_kernel_release(sz_nlsk);
err1:
	return ret;
}
module_init(plog_init);
static void __exit plog_exit(void) {
	cancel_delayed_work(&mywork);	
	init_log_release();
	kthread_stop(ksz_task);
	netlink_kernel_release(sz_nlsk);
}
module_exit(plog_exit);
MODULE_AUTHOR("Jianxing.Wang");
MODULE_DESCRIPTION("ZShield Log");
MODULE_LICENSE("GPL v2");
