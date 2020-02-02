obj-m += hook_syscall.o

EXTRA_CFLAGS := -Wno-int-conversion

hello:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
