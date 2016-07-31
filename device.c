#include "device.h"
#include "trackmem.h"
#include "common.h"
struct task_struct *using = NULL;
struct task_struct *attach = NULL;

static int memchecker_open(struct inode *inode, struct file *filep)
{
    if (using == NULL) {
        using = current;
        printk(KERN_INFO "task %s is using kmemchecker\n", current->comm);
        return 0;
    } else {
        printk(KERN_INFO "kmemchecker has been using now\n");
        return -1;
    }
}

static int memchecker_release(struct inode *inode, struct file* filep)
{
    if (using) {
        using = NULL;
    }
    if (attach) {
        attach = NULL;
    }
    return 0;
}

static ssize_t memchecker_read(struct file *fp, char __user *buf, 
        size_t size, loff_t *pos)
{
    return 0;
}

static ssize_t memchecker_write(struct file *fp, const char __user *buf, 
        size_t size, loff_t *pos)
{
    return 0;
}

static int memchecker_mmap(struct file *fp, struct vm_area_struct *vma) 
{
    return 0;
}

static int memchecker_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
    int retval = 0;
    void __user *argp = (void __user *)arg;
    struct task_struct *p;
    struct kmem_cache *s;
    struct slab_object *so; 
    int size;
    switch (cmd) {
        case ATTACH_TASK:
            if (!attach) {
                p = get_task_by_pid((pid_t)arg);
                if (p) {
                    attach = p;
                    retval = 0;
                    printk(KERN_INFO "attach task %d %s\n",
                                p->pid, p->comm);

                } else {
                    printk(KERN_INFO "do not find task\n");
                    retval = -1;
                }
            } else {
                printk(KERN_INFO "has attach task\n");
                return -1;
            }
            break;
        /*
         * use struct slab_object to copy object to user space
         * argp -> struct slab_object
         */
        case DUMP_OBJECT:
            so = (struct slab_object *) argp;       
            s = get_kmem_cache_by_addr((void *)so->start);
            if (so->length == 0) {
                size = s->size;
            } else {
                size = so->length;
            }
            if (!so->buffer) {
                printk(KERN_INFO "Please init user buffer now\n");
                return -1;
            }
            if (so->buffer < attach->mm->start_data || 
                        so->buffer > attach->mm->end_data) 
            {
                printk(KERN_INFO "buffer address illegal\n");
                return -1;
            }
            copy_to_user(so->buffer, so->start, size);
            return 0;

        default:
            break;
    }
    return retval;
}

static const struct file_operations memchecker_ops = {
    .owner = THIS_MODULE,
    .read = memchecker_read,
    .write = memchecker_write,
    .open = memchecker_open,
    .mmap = memchecker_mmap,
    .unlocked_ioctl = memchecker_ioctl,
    .release = memchecker_release,
};

static struct miscdevice memchecker_dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "memchecker",
    .fops = &memchecker_ops,
    .mode = 0775,
};

int memchecker_dev_init(void) 
{
    int ret;
    printk(KERN_INFO "registering memchecker device \n");
    ret = misc_register(&memchecker_dev);
    if (ret < 0) {
        printk(KERN_INFO "register failed\n");
        return ret;
    } else {
        printk(KERN_INFO "register success\n");
    }
    return 0;
}

void memchecker_dev_exit(void)
{
    printk(KERN_INFO "device is leaving now\n");
    misc_deregister(&memchecker_dev);
}
