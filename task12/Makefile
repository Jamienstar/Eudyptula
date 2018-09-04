ifneq ($(KERNELRELEASE),)
ccflags-y := -DDEBUG
obj-m := helloworld.o
else
ifndef KDIR
KDIR := /usr/src/kernels/$(shell uname -r)
endif

PWD := $(shell /bin/pwd)
all:
	$(MAKE) -C $(KDIR) M=$(PWD)
module_clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers *.order
endif
