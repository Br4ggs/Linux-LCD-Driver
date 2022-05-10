chardev-objs := main.o
obj-m        := chardev.o

KERNEL_DIR   := ~/linux

BUILD_DIR    := ~/personal-project-lcd-linux-driver/char-dev-poc
DEPLOY_DIR   := ~

RASP_IP      := 192.168.178.197
RASP_USR     := pi

build:
	make clean
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KERNEL_DIR) M=`pwd` modules

clean:
	rm -rf *.o *.mod *.mod.c .*.cmd *.ko modules.order *.symvers

deploy:
	make clean
	make build
	scp $(BUILD_DIR)/chardev.ko $(BUILD_DIR)/module_load $(BUILD_DIR)/module_unload $(RASP_USR)@$(RASP_IP):$(DEPLOY_DIR)
