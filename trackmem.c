#include "trackmem.h"
#include "common.h"
struct list_head *get_slab_caches_head(void)
{
    struct list_head *p;
    unsigned long addr;
    addr = kallsyms_lookup_name("slab_caches");
    if (!addr) {
        printk(KERN_INFO "do not find slab caches\n");
        return NULL;
    }
    p = (struct list_head*) addr;
    return p;
}

struct task_struct *get_task_by_pid(pid_t pid)
{
    struct task_struct *p;
    for_each_process(p)
    {
        if (p->pid == pid) {
            return p;
        }
    }
    return NULL;
}

struct kmem_cache *get_kmem_cache_by_addr(void *addr)
{
    struct page *page = virt_to_head_page(addr);
    // 先找到该块所在的页表
    if (!page) {
        return NULL;
    }
    // struct page中该字段指向其承载的slab的kmem_cache
    if (PageSlab(page))
        return page->slab_cache;
    else {
        return NULL;
    }
}
