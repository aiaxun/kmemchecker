#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>

#define ATTACH_TASK    1
#define DUMP_OBJECT    3
#define DUMP_STACK     5
#define WRITE_ADDR     7
#define DUMP_SLAB_PAGE 9

#define MMAP_SIZE 16*1024*4

struct slab_object {
     void *start;
     unsigned int length;
     unsigned char *buffer;
};

int main(int argc, char **argv) 
{
    int fd = open("/dev/memchecker", O_RDWR);
    int i;
    struct slab_object *obj;
    if (fd < 0) {
        printf("open device error\n");
        return -1;
    }
    ioctl(fd, 1, 2709);
    printf("attach task 1\n");
    i = 0;
    while (1) { 
        i++;
        if (i==0xffffffff) {
            break;
        }
    }
    close(fd);
    return 0;
    
}
