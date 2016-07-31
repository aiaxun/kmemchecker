#ifndef _KMEMCHECKER_TRACKMEM_H
#define _KMEMCHECKER_TRACKMEM_H
#include <linux/kernel.h>
#include <linux/kallsyms.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/slub_def.h>


struct list_head *get_slab_caches_head(void);
struct task_struct *get_task_by_pid(pid_t pid);
struct kmem_cache *get_kmem_cache_by_addr(void *addr);


#endif
