/*
 * Here's a sample kernel module showing the use of jprobes to dump
 * the arguments of _do_fork().
 *
 * For more information on theory of operation of jprobes, see
 * Documentation/kprobes.txt
 *
 * Build and insert the kernel module as done in the kprobe example.
 * You will see the trace data in /var/log/messages and on the
 * console whenever _do_fork() is invoked to create a new process.
 * (Some messages may be suppressed if syslogd is configured to
 * eliminate duplicate messages.)
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <net/sock.h>
#include <net/af_unix.h>
#include <linux/string.h>
#include <linux/inet.h>

/*
 * Jumper probe for _do_fork.
 * Mirror principle enables access to arguments of the probed routine
 * from the probe handler.
 */

/* Proxy routine having the same arguments as actual _do_fork() routine */
static int unix_mkname(char *unix_path, int len, struct socket *sock, struct msghdr *msg)
{
	struct sock *peer = NULL;
	if (msg->msg_namelen) {
		DECLARE_SOCKADDR(struct sockaddr_un *, sunaddr, msg->msg_name);
		if (!sunaddr->sun_path[0]) {
			unix_path[0] = '@';
			strncpy(unix_path + 1, sunaddr->sun_path + 1, len - 2);
		} else {
			strncpy(unix_path, sunaddr->sun_path, len - 1);
		}
	} else {
		if (unix_sk(sock->sk)->addr) {
			if (!unix_sk(sock->sk)->addr->name->sun_path[0]) {
				unix_path[0] = '@';
				strncpy(unix_path + 1, unix_sk(sock->sk)->addr->name->sun_path + 1, len - 2);
			} else {
				strncpy(unix_path, unix_sk(sock->sk)->addr->name->sun_path, len - 1);
			}
		} else {
			peer = unix_peer_get(sock->sk);
			if (peer && unix_sk(peer)->addr) {
				if (!unix_sk(peer)->addr->name->sun_path[0]) {
					unix_path[0] = '@';
					strncpy(unix_path + 1, unix_sk(peer)->addr->name->sun_path + 1, len - 2);
				} else {
					strncpy(unix_path, unix_sk(peer)->addr->name->sun_path, len - 1);
				}
			}
			if (peer) sock_put(peer);
		}
	}
	if (!unix_path[0]) printk(KERN_ERR "name_len:%d\n", msg->msg_namelen);
    return 0;
}
static int j_unix_dgram_sendmsg(struct socket *sock, struct msghdr *msg, size_t len) {
	struct iov_iter *i = &msg->msg_iter;
	char unix_path[INET_ADDRSTRLEN] = {0};
	char *buff = kzalloc(i->iov->iov_len, GFP_ATOMIC);;

	unix_mkname(unix_path, sizeof(unix_path), sock, msg);
	copy_from_user(buff, i->iov->iov_base, i->iov->iov_len);
	print_hex_dump(KERN_ERR, unix_path, DUMP_PREFIX_OFFSET, 32, 1, buff, i->iov->iov_len, "\n");
	kfree(buff);

	jprobe_return();
	return 0;
}
static int j_unix_stream_sendmsg(struct socket *sock, struct msghdr *msg, size_t len) {
	struct iov_iter *i = &msg->msg_iter;
	char unix_path[INET_ADDRSTRLEN] = {0};
	char *buff = kzalloc(i->iov->iov_len, GFP_ATOMIC);;

	unix_mkname(unix_path, sizeof(unix_path), sock, msg);
	copy_from_user(buff, i->iov->iov_base, i->iov->iov_len);
	print_hex_dump(KERN_ERR, unix_path, DUMP_PREFIX_OFFSET, 32, 1, buff, i->iov->iov_len, "\n");
	kfree(buff);
	jprobe_return();
	return 0;
}

static struct jprobe jp1 = {
	.entry			= j_unix_stream_sendmsg,
	.kp = {
		.symbol_name	= "unix_stream_sendmsg",
	},
};
static struct jprobe jp2 = {
	.entry			= j_unix_dgram_sendmsg,
	.kp = {
		.symbol_name	= "unix_dgram_sendmsg",
	},
};
static struct jprobe *jps[2] = {&jp1, &jp2};
static int __init jprobe_init(void)
{
	int ret;

	ret = register_jprobes(jps, 2);
	if (ret < 0) {
		pr_err("register_jprobe failed, returned %d\n", ret);
		return -1;
	}
	return 0;
}

static void __exit jprobe_exit(void)
{
	unregister_jprobes(jps, 2);
}

module_init(jprobe_init)
module_exit(jprobe_exit)
MODULE_LICENSE("GPL");
