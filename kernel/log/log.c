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
spinlock_t log_list_lock;
static struct sk_buff_head szlog_skb_queue;

DECLARE_WAIT_QUEUE_HEAD(backlog_wait);

typedef struct {
	struct list_head list;
	struct sk_buff* skb;
}zslog;

void nlmsg_send(struct sk_buff *skb);
void init_logger_release(void);
static void nl_recv(struct sk_buff *skb) {
	if(!skb){
		pr_err("%s recv NULL skbuff\n", __func__);
		return;
	}
	//Need to check the process's cred
	pid = NETLINK_CREDS(skb)->pid;
	pr_info("%d process register to szlog\n", pid);
}
static struct sk_buff * szlog_buffer_alloc(int payload, gfp_t gfp_mask)
{
        unsigned long flags;
        struct audit_buffer *ab = NULL;
        struct nlmsghdr *nlh;
	struct sk_buff *skb;


        skb = nlmsg_new(payload, gfp_mask);
        if (!skb)
                goto err;

        nlh = nlmsg_put(skb, 0, 0, 0, payload, 0);
        if (!nlh)
                goto out_kfree_skb;

        return skb;

out_kfree_skb:
        kfree_skb(skb);
        skb = NULL;
err:
        return NULL;
}
void audit_log_vformat(struct sk_buff *skb, const char *fmt, ...)
{
        va_list args;
	int avail, len;
                         
        va_start(args, fmt);
        avail = skb_tailroom(skb);
        if (avail == 0) {
		pr_warn("skb no available room\n");
                goto out;
        }
        len = vsnprintf(skb_tail_pointer(skb), avail, fmt, args);
        if (len > 0)
                skb_put(skb, len);
        va_end(args);
out:
        return;

}
static void audit_printk_skb(struct sk_buff *skb)
{
        struct nlmsghdr *nlh = nlmsg_hdr(skb);
	char *data = nlmsg_data(nlh);

	if (printk_ratelimit())
		pr_notice("type=%d %s\n", nlh->nlmsg_type, data);
	else
		pr_notice("printk limit exceeded\n");
}

int audit_log(const char *fmt, ...) {
	va_list args;
	struct sk_buff *skb;
	int avail, len;
	if(!sz_nlsk) {
		pr_err("nlsk init failed\n");
		return -1;
	}
	if(skb_queue_len(&szlog_skb_queue) > MAX_LOG_ENTRY_NUM) {
		pr_notice("%s drop log\n", __func__);
		return -1;
	}
	skb = szlog_buffer_alloc(MAX_MSGSIZE, GFP_KERNEL);
	if (!skb) {
		pr_notice("%s fail to alloc sk_buff", __func__);
		return -1;
	}
                         
        va_start(args, fmt);
        avail = skb_tailroom(skb);
        if (avail == 0) {
		pr_warn("skb no available room\n");
                goto out;
        }
        vsnprintf(NLMSG_DATA(nlmsg_hdr(skb)), avail, fmt, args);
	va_end(args);
	if (pid) {
		skb_queue_tail(&szlog_skb_queue, skb);
		wake_up_interruptible(&backlog_wait);
	} else {
		skb_queue_tail(&szlog_skb_queue, skb);
		wake_up_interruptible(&backlog_wait);
	}
	return 0;
out:
	kfree_skb(skb);
	return 0;
}
static int ksz_log_server(void *dummy)
{
	struct sk_buff *skb;
	set_freezable();
	while (!kthread_should_stop()) {
		skb = skb_dequeue(&szlog_skb_queue);
		if (skb) {
			if (pid) {
				audit_printk_skb(skb);
				nlmsg_send(skb);
			} else {
				audit_printk_skb(skb);
				kfree_skb(skb);
			}
			continue;
		}
		wait_event_freezable(backlog_wait, (!list_empty(&zslog_list)) || kthread_should_stop());
	}
	return 0;
}
void nlmsg_send(struct sk_buff *skb) {
	netlink_unicast(sz_nlsk, skb, pid, MSG_DONTWAIT);
	return ;
}
static void my_work(struct work_struct *work);

DECLARE_DELAYED_WORK(mywork, my_work);

static void my_work(struct work_struct *work) {
        //printk(KERN_DEBUG "my delay work\n");
	audit_log("hello zslog %s %d", "abcdefg", 0x12345678);
	schedule_delayed_work(&mywork, 1000);
}

static int __init logger_init(void) {
	int ret = 0;
	struct netlink_kernel_cfg cfg = {
		.input = nl_recv,
		.flags  = NL_CFG_F_NONROOT_RECV,
		.groups = 0,
	};

	sz_nlsk = netlink_kernel_create(&init_net, NETLINK_SZLOG, &cfg);
	if (sz_nlsk == NULL) {
		pr_err("cannot initialize netlink socket in namespace");
		ret = -ENOMEM;
		goto err1;
	}
	//sz_nlsk->sk_sndtimeo = MAX_SCHEDULE_TIMEOUT;
	skb_queue_head_init(&szlog_skb_queue);
	if (!ksz_task) {
		ksz_task = kthread_run(ksz_log_server, NULL, "k_szlog");
		if (IS_ERR(ksz_task)) {
			ret = PTR_ERR(ksz_task);
			ksz_task = NULL;
			goto err2; 
		}    
	}
	schedule_delayed_work(&mywork, 100);
	return 0;
err2:
	netlink_kernel_release(sz_nlsk);
err1:
	return ret;
}
module_init(logger_init);
static void __exit logger_exit(void) {
	cancel_delayed_work(&mywork);	
	kthread_stop(ksz_task);
	netlink_kernel_release(sz_nlsk);
}
module_exit(logger_exit);
MODULE_AUTHOR("Jianxing.Wang");
MODULE_DESCRIPTION("ZShield Log");
MODULE_LICENSE("GPL v2");
