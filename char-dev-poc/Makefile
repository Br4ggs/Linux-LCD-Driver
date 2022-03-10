# TODO: concaternate calls to scp into one
# TODO: figure out if we can set execute permissions from this machine on raspberry

chardev-objs := main.o
obj-m        := chardev.o

BUILD_DIR  := ~/personal-project-lcd-linux-driver/char-dev-poc
DEPLOY_DIR := ~

KERNEL_DIR    := ~/linux
RASPIP_HOME   := 192.168.178.197
RASPIP_STRIJP := 192.168.0.50
RASP_USR      := pi

build:
	make clean
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KERNEL_DIR) M=`pwd` modules

clean:
	rm -rf *.o *.mod *.mod.c .*.cmd *.ko modules.order *.symvers

deploy_home:
	make clean
	make build
	scp $(BUILD_DIR)/chardev.ko    $(RASP_USR)@$(RASPIP_HOME):$(DEPLOY_DIR)
	scp $(BUILD_DIR)/module_load   $(RASP_USR)@$(RASPIP_HOME):$(DEPLOY_DIR)
	scp $(BUILD_DIR)/module_unload $(RASP_USR)@$(RASPIP_HOME):$(DEPLOY_DIR)

deploy_school:
	make clean
	make build
	scp $(BUILD_DIR)/chardev.ko    $(RASP_USR)@$(RASPIP_STRIJP):$(DEPLOY_DIR)
	scp $(BUILD_DIR)/module_load   $(RASP_USR)@$(RASPIP_STRIJP):$(DEPLOY_DIR)
	scp $(BUILD_DIR)/module_unload $(RASP_USR)@$(RASPIP_STRIJP):$(DEPLOY_DIR)
