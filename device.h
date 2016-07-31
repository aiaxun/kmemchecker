#ifndef _HEAPCHECKER_DEVICE_H
#define _HEAPCHECKER_DEVICE_H

#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/sched.h>

/* ioctl cmd */
 
/* 1. attach task
 * 2. dump object from one address with its size 
 * 3. dump kernel stack 
 * 4. arbitrarily write on one address 
 * 5. dump slab page to show slab layout
*/
#define ATTACH_TASK    1
#define DUMP_OBJECT    3
#define DUMP_STACK     5
#define WRITE_ADDR     7
#define DUMP_SLAB_PAGE 9

/* MMAP_SIZE 64KB */
#define MMAP_SIZE 64*1024

/* Module device initialize
 * 
 * */

int memchecker_dev_init(void);
void memchecker_dev_exit(void);

#define for_each_object(__p, __s, __addr, __objects) \
        for (__p = (__addr); __p < (__addr) + (__objects)*(__s)->size,\
             __p += (__s)->size)

#endif /* _HEAPCHECKER_DEVICE_H */
