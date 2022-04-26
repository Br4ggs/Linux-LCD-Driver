/* 
 * Author: Emiel van den Brink
 * 
 * Description:
 * This small Proof Of Concept demonstrates how a char device kernel module
 * can be created and registered.
 *
 * It statically allocates major number 15 and registers one minor number (0)
 * Availability of major number 15 for char devices has been checked beforehand in /proc/devices
 * 
 * If successful, it will create and add a character device struct to the kernel.
 * 
 * System calls open, release, read and write are stumped and send a message to the kernel system log
 * You can test this by trying the following bash command on the char device's file node:
 * $: < /path/to/device/node
 * 
 * The above should trigger the open and release handlers defined below
 */

//TODO: add and remove device numbers
//use major number 15 for now

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/io.h>

#include <linux/delay.h>

MODULE_AUTHOR("Emiel vd Brink");
MODULE_LICENSE("Dual BSD/GPL");


//TODO: move to constants header file
//see BCM2835 Peripheral manual
#define BCM2835_PERI_BASE 0x20000000
#define GPIO_BASE         (BCM2835_PERI_BASE + 0x200000)

#define GPFSEL0           0x0
#define GPFSEL1           0x4
#define GPFSEL2           0x8

#define GPSET0            0x1C
#define GPCLR0            0x28

#define FSEL2             6
#define FSEL3             9
#define FSEL4             12
#define FSEL9             27
#define FSEL10            0
#define FSEL11            3
#define FSEL17            21
#define FSEL22            6
#define FSEL23            9
#define FSEL24            12
#define FSEL27            21

#define RS 2
#define RW 3
#define E  4
#define D0 9
#define D1 10
#define D2 11
#define D3 17
#define D4 22
#define D5 23
#define D6 24
#define D7 27

#define CHAR_A {D6, D0}             //01000001
#define CHAR_B {D6, D1}             //01000010
#define CHAR_C {D6, D1, D0}         //01000011
#define CHAR_D {D6, D2}             //01000100
#define CHAR_E {D6, D2, D0}         //01000101
#define CHAR_F {D6, D2, D1}         //01000110
#define CHAR_G {D6, D2, D1, D0}     //01000111
#define CHAR_H {D6, D3}             //01001000
#define CHAR_I {D6, D3, D0}         //01001001
#define CHAR_J {D6, D3, D1}         //01001010
#define CHAR_K {D6, D3, D1, D0}     //01001011
#define CHAR_L {D6, D3, D2}         //01001100
#define CHAR_M {D6, D3, D2, D0}     //01001101
#define CHAR_N {D6, D3, D2, D1}     //01001110
#define CHAR_O {D6, D3, D2, D1, D0} //01001111

//todo: create 12 bit lcd representation
//todo: create mapper function to translate 12 bit lcd value to gpio set register value (see picture)
//todo: do this with a switch function

//needs array count
//maybe make entire function call a macro?
//test if this works:
#define PRINT_CHAR_A 

static uint8_t gpioMap[11] = {RS, RW, E, D0, D1, D2, D3, D4, D5, D6, D7};

void *gpio = NULL;

void set_display_pins(uint16_t data)
{
    printk(KERN_ALERT "data is: %04X\n", data);

    uint32_t output = 0x0;

    int i;
    for(i = 0; i < 11; i++)
    {
        uint32_t value = ((data & (1 << i)) > 0);

        printk(KERN_ALERT "value is: %02X\n", value);

        output |= value << gpioMap[i];
    }

    printk(KERN_ALERT "output is: %08X\n", output);
    iowrite32(output, gpio + GPSET0);
}

void clear_display_pins(uint16_t data)
{
    uint32_t output = 0x0;

    int i;
    for(i = 0; i < 11; i++)
    {
        uint32_t value = ((data & (1 << i)) > 0);
        output |= value << gpioMap[i];
    }

    iowrite32(output, gpio + GPCLR0);
}

void write_to_register(int *bits, int count, void *address)
{
    unsigned int output = 0x0;

    int i;
    for (i = 0; i < count; i++)
    {
        output |= (1 << bits[i]);
    }

    iowrite32(output, address);
}

void set_output_pins_high(int *pins, int count)
{
    write_to_register(pins, count, gpio + GPSET0);
}

void set_output_pins_low(int *pins, int count)
{
    write_to_register(pins, count, gpio + GPCLR0);
}

int chardev_open(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "chardev: open called\n");
    return 0;
}

int chardev_release(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "chardev: release called\n");
    return 0;
}

ssize_t chardev_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
    printk(KERN_ALERT "chardev: read called\n");
    return 0;
}

//valid commands are:
//GPHI
//GPLO
ssize_t chardev_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp)
{
    if (count < 4)
    {
        printk(KERN_ALERT "chardev: incorrect buffer size from user space\n");
        return -EINVAL;
    }

    char buffer[count];
    memset(buffer, '\0', count * sizeof(buffer[0]));

    //access_ok?

    if (copy_from_user(buffer, buff, count))
    {
        //not all data was copied; fully try again?
        printk(KERN_ALERT "chardev: not all data was copied from user space buffer\n");
        return 0;
    }

    printk(KERN_ALERT "chardev: write called\n");
    printk(KERN_ALERT "chardev: received: %s\n", buffer);

    if      (strcmp(buffer, "GPHI") == 0)
    {
        //set pin 4 to HIGH
        printk(KERN_ALERT "chardev: setting io pin to HIGH\n");
        iowrite32(1 << 4, gpio + 0x1C);
    }
    else if (strcmp(buffer, "GPLO") == 0)
    {
        //set pin 4 to LOW
        printk(KERN_ALERT "chardev: setting io pin to LOW\n");
        iowrite32(1 << 4, gpio + 0x28);
    }
    else
    {
        printk(KERN_ALERT "chardev: incorrect command specified\n");
        return -EINVAL;
    }

    return count;
}

struct file_operations chardev_fops =
{
    .owner          = THIS_MODULE,
    .llseek         = NULL,
    .read           = chardev_read,
    .write          = chardev_write,
    .unlocked_ioctl = NULL,
    .open           = chardev_open,
    .release        = chardev_release,
};

struct cdev *chardev_cdev = NULL;

static void chardev_exit_module(void)
{
    printk(KERN_ALERT "chardev: exit\n");

    clear_display_pins(0b11111111111);

    if (chardev_cdev) cdev_del(chardev_cdev);

    dev_t dev = MKDEV(15, 0);

    unregister_chrdev_region(dev, 1);

    iounmap(gpio);
}

static int chardev_init_module(void)
{
    printk(KERN_ALERT "chardev: init\n");

    //TODO: use dynamic allocation of major number
    dev_t dev  = MKDEV(15, 0);
    int result = register_chrdev_region(dev, 1, "chardev");

    if (result < 0)
    {
        printk(KERN_WARNING "chardev: device number registration unsuccessful\n");
        goto fail;
    }
    
    printk(KERN_WARNING "chardev: device number registration successful\n");

    chardev_cdev      = cdev_alloc();
    chardev_cdev->ops = &chardev_fops;
    result            = cdev_add(chardev_cdev, dev, 1);

    if (result < 0)
    {
        printk(KERN_WARNING "chardev: char device registration unsuccessful\n");
        goto fail;
    }
    
    printk(KERN_WARNING "chardev: char device registration successful\n");

    if (request_mem_region(GPIO_BASE, 0x30, "chardev") == NULL)
        printk(KERN_WARNING "chardev: GPIO region already reserved\n");

    //todo: check if region is available
    //todo: check /proc/iomem
    //TODO: reserve gpio region
    gpio = ioremap(GPIO_BASE, 0x30);
    if (gpio == NULL)
    {
        printk(KERN_WARNING "chardev: ioremap fail\n");
        result = -1;
        goto fail;
    }

    printk(KERN_WARNING "chardev: ioremap successful\n");
    printk(KERN_WARNING "chardev: gpio address is %px\n", gpio);

    //set all used gpio pins to output
    unsigned int funcSelect = ioread32(gpio + GPFSEL0);
    funcSelect |= (1 << FSEL2) | (1 << FSEL3) | (1 << FSEL4) | (1 << FSEL9);
    iowrite32(funcSelect, gpio + GPFSEL0);

    funcSelect = ioread32(gpio + GPFSEL1);
    funcSelect |= (1 << FSEL10) | (1 << FSEL11) | (1 << FSEL17);
    iowrite32(funcSelect, gpio + GPFSEL1);

    funcSelect = ioread32(gpio + GPFSEL2);
    funcSelect |= (1 << FSEL22) | (1 << FSEL23) | (1 << FSEL24) | (1 << FSEL27);
    iowrite32(funcSelect, gpio + GPFSEL2);

    //set all used gpio pins to low
    unsigned int outputSet = 0x0;
    outputSet |= (1 << 2) | (1 << 3) | (1 << 4) | (1 << 9) | (1 << 10) | (1 << 11) | (1 << 17) | (1 << 22) | (1 << 23) | (1 << 24) | (1 << 27);
    iowrite32(outputSet, gpio + GPCLR0);

    //Initializing by Instruction
    set_display_pins(0b00111000000);
    mdelay(100);
    set_display_pins(0b00111000000);
    mdelay(50);
    set_display_pins(0b00111000000);
    set_display_pins(0b00111000000);

    //set display
    clear_display_pins(0b11111111111);
    set_display_pins(0b00001111000);
    clear_display_pins(0b00000000100);
    mdelay(1);
    set_display_pins(0b00000000100);
    mdelay(1);
    clear_display_pins(0b00000000100);

    //clear display
    clear_display_pins(0b11111111111);
    mdelay(1);
    set_display_pins(0b00000001000);
    clear_display_pins(0b00000000100);
    mdelay(1);
    set_display_pins(0b00000000100);
    mdelay(1);
    clear_display_pins(0b00000000100);

    mdelay(2000);

    //entry mode set
    clear_display_pins(0b11111111111);
    mdelay(1);
    set_display_pins(0b00000110000);
    clear_display_pins(0b00000000100);
    mdelay(1);
    set_display_pins(0b00000000100);
    mdelay(1);
    clear_display_pins(0b00000000100);

    mdelay(500);

    //write 'a' to screen?
    clear_display_pins(0b11111111111);
    mdelay(1);
    set_display_pins(0b01000001001);
    clear_display_pins(0b00000000100);
    mdelay(1);
    set_display_pins(0b00000000100);
    mdelay(1);
    clear_display_pins(0b00000000100);

    // set_display_pins(0b00000000000);
    // mdelay(10);
    // set_display_pins(0b00001111000);
    // mdelay(10);

    //int pins[11] = {RS, RW, E, D0, D1, D2, D3, D4, D5, D6, D7};
    //set_output_pins_low(pins, 11);

    //read: https://stackoverflow.com/questions/32632877/initialize-integer-array-inline-when-passing-arguments-to-a-method
    //TODO: test if this works
    //set_output_pins_low((int[])CHAR_O, 5);

    return 0;

    fail:
        chardev_exit_module();
        return result;
}

module_init(chardev_init_module);
module_exit(chardev_exit_module);
