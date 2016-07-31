#include "common.h"
#include <linux/kernel.h>
#include <linux/module.h>
#include "device.h"
#include "hook.h"

static int __init kmemchecker_init(void)
{
    int ret = 0;
    printk(KERN_INFO "module init \n");
    ret = memchecker_dev_init();
    if (ret < 0) {
        return -1;
    }
    ret = kmemchecker_probes_init();
    if (ret < 0) {
    	printk(KERN_INFO "kprobe plant failed\n");
    	return -1;
    }
    return 0;
}

static void __exit kmemchecker_exit(void)
{
    memchecker_dev_exit();
    kmemchecker_probes_exit();
    printk(KERN_INFO "bye\n");
}


MODULE_LICENSE("GPL");
module_init(kmemchecker_init);
module_exit(kmemchecker_exit);
