# Personal project LCD linux driver

This repository contains all sourcecode for the LCD kernel module personal project developed in semester 6. It contains the source for the driver itself, as well as some accompanying user-space programs to interact with the driver.

This project is aimed towards cross-compilation. Code is compiled on the development environment and transferred over to the production environment: A raspberry pi. (see section `Requirements` for the required hardware and software versions)

[Demo video!](https://www.youtube.com/watch?v=KRJz6AVv7Hs)

## Contents

The repository contains the following contents:

### char-dev-poc

The LCD character device driver. Controls a `16x2` LCD display via the GPIO pins.

### user-space-prog

A user-space test program to test system call interactions between user-space programs and the device driver.

### user-space-lcd

A user-space program to help write text to the LCD display via the use of the device driver.

## Requirements
The OS used for the development environment is `Ubuntu 20.04.1 LTS`, though any linux-based environment should work for development.

The Production platform used is a `Raspberry Pi model B Revision 2.0`, running `Raspberry Pi OS Lite version 11 (bullseye) (32 bit) ` which can be installed via the [Raspberry Pi Imager](https://www.raspberrypi.com/software/)

The development environment should have a fully configured and build version of the Raspberry Pi kernel on it for cross-compilation, for instructions on building your own kernel please refer to the [official documentation](https://www.raspberrypi.com/documentation/computers/linux_kernel.html#building).

The kernel version should match that of the production environment. For the production platform kernel version `5.10.92+` was used (the source of which can be bound [here](https://github.com/raspberrypi/linux/tree/rpi-5.10.y)).

> Note: If your version of Raspberry Pi OS Lite uses a later kernel version please use that one. **Do not mismatch kernel versions as this can lead to cryptic errors later in development!**

## Installing
1. Clone this repository.
2. Go into `char-dev-poc` and set the makefile fields accordingly.
3. run the `deploy` target: `$ make deploy`. This will cross-compile the kernel module for the raspberry pi, and transfer the created `.ko` file and loading/unloading bash scripts over to the raspberry.
4. Go into `user-space-lcd` and set the makefile fields accordingly.
5. For local testing, run the `build` target: `$ make build`. For cross-compilation, run the `build_pi` target: `$ make build_pi`. This will cross-compile the test program for the raspberry pi, and transfer the created executable over to the raspberry.
6. Ssh into the raspberry pi, and run the `./module_load` script to install the driver into the kernel (you need root permissions for this): `$ sudo ./module_load` (run `./module_unload` to unload the driver again).

The LCD display should now be fully initialized.

## Usage
After installing, you can use the previously cross-compiled `write_lcd` (executable from `user_space_lcd`) to write text to the lcd display (you need root permissions for this):  
`$ ./write_lcd Hello world! This is some text!`

