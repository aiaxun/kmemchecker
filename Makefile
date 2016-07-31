KERNEL_VER=$(shell uname -r)
KERNEL_DIR=/lib/modules/$(shell uname -r)/build
obj-m += kmemchecker.o

kmemchecker-objs= device.o hook.o trackmem.o main.o

all:
		make -C $(KERNEL_DIR) M=$(shell pwd) modules
clean:
		make -C $(KERNEL_DIR) M=$(shell pwd) modules clean
