/*************************************************************************
  > File Name: kernel/bkpt-test.c
  > Author: wangjx
 ************************************************************************/
/*
 * test-bkpt.c
 *
 *  Created on: Jan 30, 2015
 *      Author: bfleming
 */


#include <linux/module.h>
#include <linux/init.h>

#include <linux/hrtimer.h>
#include <linux/hw_breakpoint.h>
#include <linux/kallsyms.h>
#include <linux/ktime.h>
#include <linux/perf_event.h>
#include <linux/sched.h>

static int test_value = 0;  /* location to watch */

struct perf_event* __percpu *test_hbp = NULL;

static struct hrtimer my_hrtimer;

static void test_hbp_handler(struct perf_event* bp,
		struct perf_sample_data* data, struct pt_regs *regs)
{
	static int i=0;
	printk("test_value = %d ", test_value);
	printk("data: ip=%p, tid=%u, time=%llu, addr=%p\n", 
			(void*)(uintptr_t)data->ip, data->tid_entry.tid, data->time,
			(void*)(uintptr_t)data->addr);

	if (i++ > 10) {
		printk("Self-unregistering breakpoint\n");
		unregister_wide_hw_breakpoint(test_hbp);
		test_hbp = NULL;
	}
}

static enum hrtimer_restart test_time_fn(struct hrtimer *timer)
{
	printk("Running test_time_fn...\n");
	test_value++;
	printk("Ran test_time_fn, test_value=%d\n", test_value);
	return HRTIMER_NORESTART;
}

int __init init_test(void)
{
	int result = 0;
	struct perf_event_attr attr;

	printk("&test_value=%p\n", &test_value);

	/* install a timer to write to the memory location I'm
	 * watching */
	hrtimer_init(&my_hrtimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	my_hrtimer.function = &test_time_fn;
	if(hrtimer_start(&my_hrtimer, ns_to_ktime(5ull*1000*1000*1000), HRTIMER_MODE_REL)) {
		result = -EFAULT;
		goto fail;
	}

	hw_breakpoint_init(&attr);
	attr.bp_addr = kallsyms_lookup_name("test_value");
	attr.bp_len = HW_BREAKPOINT_LEN_4;
	attr.bp_type = HW_BREAKPOINT_W;
	printk("attr.bp_addr=%p\n", (void*)(uintptr_t)attr.bp_addr);

	test_hbp = register_wide_hw_breakpoint(&attr, test_hbp_handler, NULL);
	if (IS_ERR((void __force*)test_hbp)) {
		result = PTR_ERR((void __force*)test_hbp);
		goto fail;
	}
	printk("Installed breakpoint\n");
	return 0;

fail:
	return result;
}

static void cleanup_test(void)
{
	if (test_hbp)
		unregister_wide_hw_breakpoint(test_hbp);

	printk("unloaded module\n");
}

module_init(init_test);
module_exit(cleanup_test);
MODULE_LICENSE("GPL");
