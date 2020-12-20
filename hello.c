#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/bug.h>

#include <linux/ktime.h>

struct personal_list_head {
struct personal_list_head *next;
ktime_t time;
};

MODULE_AUTHOR("Kramar Viktor IO-83");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static struct personal_list_head *header;


static uint count = 1;
module_param(count, uint, 0444);
MODULE_PARM_DESC(count, "Number of messages Hello, world");


static int __init hello_init(void)
{
	uint i = 0;
	struct personal_list_head *first, *second;

	if (count == 0) {
		pr_warn("WARNING!\nCounter = 0");
	} else if (count >= 5 && count <= 10) {
		pr_warn("WARNING!\nCounter between 5 and 10");
	} else if (count > 10) {
		pr_err("Invalid value!");
		BUG_ON(1);
	}

	header = kmalloc(sizeof(struct personal_list_head *), GFP_KERNEL);
	first = header;
	for (i = 0; i < count; i++) {
		if (i == 2)
			first->next = kmalloc(0, GFP_KERNEL);
		else 
			first->next = kmalloc(sizeof(struct personal_list_head), GFP_KERNEL);
		first->time = ktime_get();
		pr_info("Hello World!");
		second = first;
		first = first->next;
	}
	if (count != 0) {
		kfree(second->next);
		second->next = NULL;
	}
	pr_info("count: %d\n", count);
	return 0;
}


static void __exit hello_exit(void)
{
	struct personal_list_head *variable;
	while (header != NULL && count != 0) {
		variable = header;
		pr_info("Time : %lld", variable->time);
		header = variable->next;
		kfree(variable);
	}
	if (count == 0)
		pr_info("Time is 0 (print never used)");
		pr_info("");
}

module_init(hello_init);
module_exit(hello_exit);
