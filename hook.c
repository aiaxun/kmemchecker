#include "hook.h"
#include "trackmem.h"

/*
 * kretprobe : __kmalloc
 * jprobe    : kfree
 */

static char func_name[NAME_MAX] = "__kmalloc";
extern struct task_struct *attach;

static unsigned char probes[MAX_PROBES];

struct my_data {
	ktime_t entry_stamp;
};

static int entry_handler(struct kretprobe_instance *ri, 
	struct pt_regs *regs)
{
	return 0;
}

static int ret_handler(struct kretprobe_instance *ri, 
	struct pt_regs *regs)
{
	int retval = regs_return_value(regs);
	struct kmem_cache *s;
	if (current == attach)
	{
		s = get_kmem_cache_by_addr( (void* )retval);
		printk(KERN_INFO "%s [%d] malloc %008x , %s\n", 
			current->comm, current->pid, retval, s->name);
	}
	return 0;
}

static struct kretprobe my_kmalloc_kretprobe = {
	.handler		= ret_handler,
	.entry_handler		= entry_handler,
	.data_size		= sizeof(struct my_data),
	/* Probe up to 20 instances concurrently. */
	.maxactive		= 20,
};

static struct kretprobe my_kmem_cache_alloc_kretprobe = {
	.handler = ret_handler,
	.entry_handler = entry_handler,
	.data_size = sizeof(struct my_data),
	.maxactive = 20,
};
// kfree function
void kmemchecker_do_kfree(const void *x) 
{
	if (attach == current) 
	{
		printk(KERN_INFO "%s free %008x\n", current->comm, x);
	}	
	jprobe_return();
	return 0;
}
static struct jprobe my_kfree_jprobe = {
	.entry			= kmemchecker_do_kfree,
	.kp = {
		.symbol_name	= "kfree",
	},
};

int kmemchecker_probes_init(void)
{
	int ret;

	my_kmalloc_kretprobe.kp.symbol_name = func_name;
	ret = register_kretprobe(&my_kmalloc_kretprobe);
	if (ret < 0) {
		printk(KERN_INFO "kretprobe failed, returned %d\n",
			ret);
		return -1;
	}

	printk(KERN_INFO "Planted return probe at %s: %p\n",
			my_kmalloc_kretprobe.kp.symbol_name, 
			my_kmalloc_kretprobe.kp.addr);
	ret = register_jprobe(&my_kfree_jprobe);
	if (ret < 0) {
		unregister_kretprobe(&my_kmalloc_kretprobe);
		return -1;
	}
	printk(KERN_INFO "hook kfree\n");

	my_kmem_cache_alloc_kretprobe.kp.symbol_name = "__slab_alloc";
	ret = register_kretprobe(&my_kmem_cache_alloc_kretprobe);
	if (ret < 0) {
		unregister_kretprobe(&my_kmalloc_kretprobe);
		unregister_jprobe(&my_kfree_jprobe);
		return -1;
	}
	printk(KERN_INFO "hook __slab_alloc\n");
	return 0;
}
void kmemchecker_probes_exit(void)
{
	unregister_kretprobe(&my_kmalloc_kretprobe);
	printk(KERN_INFO "kretprobe at %p unregistered\n",
			my_kmalloc_kretprobe.kp.addr);
	unregister_jprobe(&my_kfree_jprobe);
	unregister_kretprobe(&my_kmem_cache_alloc_kretprobe);
}
