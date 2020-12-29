#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/slab.h>

#include <linux/ktime.h>

struct personal_list_head {
struct personal_list_head *next;
ktime_t time;
};

MODULE_AUTHOR("Yuliia Lykhatska IV-82");
MODULE_DESCRIPTION("Writing Hello, World in Kernel");
MODULE_LICENSE("Dual BSD/GPL");

static struct personal_list_head *header;


static uint count = 1;
module_param(count, uint, S_IRUGO);
MODULE_PARM_DESC(count, "How many times to write Hello, world");


static int __init initter(void)
{
    uint i = 0;
    struct personal_list_head *first_variable, *second_variable, *variable;
    BUG_ON(count > 10);
    if (count == 0) {
        printk(KERN_WARNING "WARNING! \nCounter is 0");
    } else if (count >= 5 && count <= 10) {
        printk(KERN_WARNING "WARNING! \nCounter is between 5 and 10");
    }

    header = kmalloc(sizeof(struct personal_list_head *), GFP_KERNEL);
    first_variable = header;
    for (i = 0; i < count; i++) {
        first_variable->next = kmalloc(sizeof(struct personal_list_head), GFP_KERNEL);
        if (i == 6) {
            first_variable = NULL;
        }
        if (ZERO_OR_NULL_PTR(first_variable)) {
            goto error;
        }
        first_variable->time = ktime_get();
        pr_info("Hello World!");
        second_variable = first_variable;
        first_variable = first_variable->next;
    }
    if (count != 0) {
        kfree(second_variable->next);
        second_variable->next = NULL;
    }
    printk(KERN_INFO "count: %d\n", count);
    return 0;
error:
    pr_err("Out of memory");
    while (header != NULL && count != 0) {
    	variable = header;
    	header = variable->next;
    	kfree(variable);
    }
    BUG_ON(count > 6);
    return -ENOMEM;
}
static void __exit exitter(void)
{
    struct personal_list_head *variable;
    while (header != NULL && count != 0) {
        variable = header;
    	pr_info("Time : %lld", variable->time);
    	header = variable->next;
    	kfree(variable);
    }
    if (count == 0) {
    	pr_info("Time is 0 because print wasn't used");
    	kfree(variable);
    }
    pr_info("");
}

module_init(initter);
module_exit(exitter);
