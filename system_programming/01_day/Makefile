obj-m=module_1.o

KERNEL=/root/linux
PWD=$(shell pwd)

default:
	$(MAKE) -C $(KERNEL) SUBDIR=$(PWD) modules
clean:
	$(MAKE) -C $(KERNEL) SUBDIR=$(PWD) clean
