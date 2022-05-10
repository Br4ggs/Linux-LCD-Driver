ASSIGNMENT=write_lcd

#INC_DIRS

SRC_FILES=main.c

#INC_FILES

RASP_IP  := 192.168.178.197
RASP_USR := pi

all: build

clean:
	@rm -rf $(ASSIGNMENT) $(ASSIGNMENT)_pi

build: $(SRC_FILES) clean
	gcc -o $(ASSIGNMENT) -I. $(SRC_FILES)

build_pi: $(SRC_FILES) clean
	arm-linux-gnueabi-gcc -march=armv6 -o $(ASSIGNMENT)_pi -I. $(SRC_FILES)
	scp ./$(ASSIGNMENT)_pi $(RASP_USR)@$(RASP_IP):~