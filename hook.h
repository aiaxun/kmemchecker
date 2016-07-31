#ifndef _MEMCHECKER_HOOK_H
#define _MEMCHECKER_HOOK_H
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/ktime.h>
#include <linux/limits.h>
#include <linux/sched.h>

#define MAX_PROBES 			16
#define __KMALLOC_PROBE 	0
#define KFREE_PROBE 		1
#define KMEM_CACHE_ALLOC 	2
#define KMEM_CACHE_FREE		3
#define __SLAB_ALLOC		4
#define __SLAB_FREE			5



/*
 * TODO:
 * Need to probe some kernel functions:
 * __kmalloc -> kretprobe, get retval
 * kfree -> kjprobe, get parameter
 * __sys_sendmsg -> jprobe: get parameter value ? ___sys_sendmsg
 * __sys_recvmsg -> jprobe: .* ? ___sys_recvmsg
 * sys_ioctl
 * sys_fcntl
 * sys_chroot
 * sys_ssetmask
 * sys_socketcall
 * sys_mprotect
 * sys_setresuid16
 * sys_chown16
 * sys_vfork
 * sys_setuid
 * sys_setgid
 * sys_futex
 * It is better to hook all of those sys calls. 
 * And we'd like to know that the syscall number when doing system calls.
*/

int kmemchecker_probes_init(void);
void kmemchecker_probes_exit(void); 

#endif
