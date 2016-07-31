#ifndef _MEMCHECKER_COMMON_H
#define _MEMCHECKER_COMMON_H

#define TRACK_ADDRS_COUNT 16
/* module base */
#include <linux/module.h>
#include <linux/kernel.h>
/* Slab */
#include <linux/slab.h>
#include <linux/slub_def.h>
#include <linux/socket.h>
#include <linux/bitops.h>
#include <linux/bit_spinlock.h>
#include <net/sock.h>
/*page & object*/
#include <linux/mm_types.h>
#include <linux/mm.h>
#include <asm/uaccess.h>
#include <asm/page.h>
#include <linux/gfp.h>
#include <linux/skbuff.h>
#include <linux/net.h>
#include <linux/numa.h>
#include <linux/nodemask.h>
#include <linux/kmemleak.h>

//#include <asm/bitops.h>
/* file system*/
#include <linux/fdtable.h>
#include <linux/fs.h>
#include <linux/path.h>
#include <linux/dcache.h>
//devices
#include <linux/cdev.h>
#include <linux/miscdevice.h>
//task
#include <linux/sched.h>
#include <linux/pid.h>
//probe
#include <linux/kallsyms.h>
#include <linux/kprobes.h>
//string ops
#include <linux/string.h>
#include <linux/printk.h>
//mutex
#include <linux/mutex.h>
// SMP
#include <linux/preempt.h>


/*
 * prepared for ioctl interface 
 * copy size of length from addr 
 */
struct slab_object {
    void *start;
    unsigned int length;
    unsigned char *buffer;
};

/*
 * The slab lists for all objects. copy from mm/slab.h
 */
 /*
 * struct array_cache
 *
 * Purpose:
 * - LIFO ordering, to hand out cache-warm objects from _alloc
 * - reduce the number of linked list operations
 * - reduce spinlock operations
 *
 * The limit is stored in the per-cpu structure to reduce the data cache
 * footprint.
 *
 */

struct array_cache {
    unsigned int avail;
    unsigned int limit;
    unsigned int batchcount;
    unsigned int touched;
    spinlock_t lock;
    void *entry[];  /*
             * Must have this definition in here for the proper
             * alignment of array_cache. Also simplifies accessing
             * the entries.
             *
             * Entries should not be directly dereferenced as
             * entries belonging to slabs marked pfmemalloc will
             * have the lower bits set SLAB_OBJ_PFMEMALLOC
             */
};
struct kmem_cache_node {
    spinlock_t list_lock;

#ifdef CONFIG_SLAB
    struct list_head slabs_partial; /* partial list first, better asm code */
    struct list_head slabs_full;
    struct list_head slabs_free;
    unsigned long free_objects;
    unsigned int free_limit;
    unsigned int colour_next;   /* Per-node cache coloring */
    struct array_cache *shared; /* shared per node */
    struct array_cache **alien; /* on other nodes */
    unsigned long next_reap;    /* updated without locking */
    int free_touched;       /* updated without locking */
#endif

#ifdef CONFIG_SLUB
    unsigned long nr_partial;
    struct list_head partial;
#ifdef CONFIG_SLUB_DEBUG
    atomic_long_t nr_slabs;
    atomic_long_t total_objects;
    struct list_head full;
#endif
#endif

};

struct track {
    unsigned long addr; // called from address
#ifdef CONFIG_STACKTRACE
    unsigned long addrs[TRACK_ADDRS_COUNT];
#endif
    int cpu;            // was running on cpu
    int pid;            // pid context
    unsigned long when; // when did the operation occur
};

enum track_item { TRACK_ALLOC, TRACK_FREE };

#endif

