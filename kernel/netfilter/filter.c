#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/printk.h>
#include <linux/sched.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/netfilter_ipv4.h>

#define INBOUND     1 
#define OUTBOUND    2
#define NF_ALL    (NF_LOCAL_IN|NF_LOCAL_OUT|NF_POST_ROUTINE|NF_PRE_ROUTINE)
#define NF_INPUT  (NF_LOCAL_IN|NF_PRE_ROUTINE)
#define NF_OUTPUT (NF_LOCAL_OUT|NF_POST_ROUTINE) 

char *inet_ntoa(uint32_t ip)
{
    static char buf[16];
    const uint8_t *bytes = (uint8_t *)&ip;

    sprintf(buf, "%u.%u.%u.%u", bytes[0], bytes[1], bytes[2], bytes[3]);
    return buf;
}

typedef struct netinfo_s{
    pid_t pid;
    int protocol;
    uint16_t sport;            
    uint16_t dport;         
    uint32_t saddr;
    uint32_t daddr;        
    ulong direct;
    ulong payload;
}netinfo_t;

int parse_netinfo(struct sk_buff *skb ,netinfo_t *info ,ulong point)
{
    struct iphdr *iph = NULL;
    struct tcphdr *tcph = NULL;
    struct udphdr *udph = NULL;

    memset(info, 0, sizeof(*info));
    info->pid = current->pid;
    iph = ip_hdr(skb);
    info->protocol = iph->protocol;
    info->saddr = iph->saddr;
    info->daddr = iph->daddr;

    if(iph->protocol == IPPROTO_TCP){
        tcph = (struct tcphdr *)((char *)iph + (int)(iph->ihl*4));
        info->sport = tcph->source;
        info->dport = tcph->dest;
        info->payload += (skb->len-(iph->ihl << 2) - (tcph->doff * 4));
    }else if(iph->protocol == IPPROTO_UDP){
        udph = (struct udphdr *)((char *)iph + (int)(iph->ihl*4));
        info->sport = udph->source;
        info->dport = udph->dest;
        info->payload += ntohs(udph->len) - sizeof(struct udphdr);
    }

    info->direct = point;

    return 0;
}

struct file *get_mm_exe_file(struct mm_struct *mm)
{
    struct file *exe_file;

    /* We need mmap_sem to protect against races with removal of exe_file */
    down_read(&mm->mmap_sem);
    exe_file = mm->exe_file;
    if (exe_file)
        get_file(exe_file);
    up_read(&mm->mmap_sem);
    return exe_file;
}
int task_path(struct file *f, char *path, int len, char **ptr)
{
    struct mm_struct *mm;
    struct file *exe_file;

    struct task_struct *task = pid_task(f->f_owner.pid, PIDTYPE_PID);

    if(!task) {
        printk("task is null pid %d type %x\n", pid_nr(f->f_owner.pid), PIDTYPE_PID);
        return ENOENT;
    }
    mm = get_task_mm(task);
    if (!mm) {
        return ENOENT;
    }
    exe_file = get_mm_exe_file(mm);
    mmput(mm);
    if (mm) {
        *ptr = d_path(&exe_file->f_path, path, len);
        fput(exe_file);
        return 0;
    }
}
unsigned int filter_post_routing(unsigned int hooknum, 
        struct sk_buff *skb,
        const struct net_device *in,
        const struct net_device *out,
        int (*okfn)(struct sk_buff *))
{
    char *buff = kmalloc(PAGE_SIZE, GFP_ATOMIC);
    char *ptr = NULL;
    netinfo_t ni;
    parse_netinfo(skb, &ni, OUTBOUND);
    if (ni.sport == 22 || ni.dport == 22) {
        kfree(buff);
        return NF_ACCEPT;
    }

    if (skb && skb->sk && skb->sk->sk_socket && skb->sk->sk_socket->file) {
        struct task_struct *task = pid_task(find_pid_ns(pid_nr(skb->sk->sk_socket->file->f_owner.pid), &init_pid_ns), PIDTYPE_PID);
        task_path(skb->sk->sk_socket->file, buff, PAGE_SIZE, &ptr);
        printk("pid %u %s:%s postrouting %s %d\n", pid_nr(skb->sk->sk_socket->file->f_owner.pid), task?task->comm:"NULL",
                ptr, inet_ntoa(ni.daddr), ni.dport);
    }
    else
        printk("unkown pid\n");

    kfree(buff);
    return NF_ACCEPT;
}
struct nf_hook_ops nethooks[] = { 
        {.hook = filter_post_routing,.pf = PF_INET,.hooknum = NF_INET_LOCAL_OUT,.priority = NF_IP_PRI_FIRST,.owner = THIS_MODULE,},
};
static int __init my_nf_init(void) {
    nf_register_hooks(nethooks ,ARRAY_SIZE(nethooks));
	return 0;
}
static void __exit my_nf_exit(void) {
    nf_unregister_hooks(nethooks ,ARRAY_SIZE(nethooks));
}

module_init(my_nf_init);
module_exit(my_nf_exit);

MODULE_AUTHOR("Jianxing.Wang");
MODULE_DESCRIPTION("netfilter demo");
MODULE_LICENSE("GPL v2");
