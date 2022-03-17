#TODO: add target for local compilation
#TODO: add target for cross-compilation

ASSIGNMENT=readwritetest

#INC_DIRS

SRC_FILES=main.c

#INC_FILES

RASP_IP  := 192.168.178.197
RASP_USR := pi

all: main.c
	gcc -o main main.c -I.

clean:
	@rm -rf $(ASSIGNMENT) $(ASSIGNMENT)_pi

build: $(SRC_FILES)
	make clean
	gcc -o $(ASSIGNMENT) -I. $(SRC_FILES)

#not working, causes segmentation fault
#https://www.linuxtut.com/en/a6bcef67285c10edff77/ <- this seems to work, but why?
#TODO: ask Xander difference between cross-compilers
#does it have something to do with user-space and kernel-space?
#arm-linux-gnueabi-gcc -march=armv6 main.c -o main_arm
#--with-arch=armv6 --with-float=hard --with-fpu=vfp
build_pi: main.c
	make clean
	arm-linux-gnueabi-gcc -march=armv6 -o $(ASSIGNMENT)_pi -I. $(SRC_FILES)
	scp ./$(ASSIGNMENT)_pi $(RASP_USR)@$(RASP_IP):~