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
#include "constants.h"

MODULE_AUTHOR("Emiel vd Brink");
MODULE_LICENSE("Dual BSD/GPL");

#ifdef ENABLE_WORD_WRAP

static int current_cursor_pos = 0;

#endif

static uint8_t gpioMap[11] = {RS, RW, E, D0, D1, D2, D3, D4, D5, D6, D7};

static void *gpio = NULL;

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

void trigger_enable_pulse(void)
{
    clear_display_pins(LCD_ENABLE_PIN);
    mdelay(1);
    set_display_pins(LCD_ENABLE_PIN);
    mdelay(1);
    clear_display_pins(LCD_ENABLE_PIN);
}

void clear_display(void)
{
    clear_display_pins(LCD_ALL_PINS);
    set_display_pins(LCD_CLEAR_DISPLAY);
    trigger_enable_pulse();

#ifdef ENABLE_WORD_WRAP

    current_cursor_pos = 0;

#endif
}

void write_character(char a)
{
    clear_display_pins(LCD_ALL_PINS);
    mdelay(1);
    set_display_pins(LCD_REGISTER_SET_PIN | (uint16_t)a << 3);
    trigger_enable_pulse();

#ifdef ENABLE_WORD_WRAP

    current_cursor_pos++;
    if (current_cursor_pos == 16)
    {
        clear_display_pins(LCD_ALL_PINS);
        set_display_pins(LCD_SET_DDRAM_ADDRESS | 41 << 3);
        trigger_enable_pulse();
    }

#endif
}

void write_string(char *str, int length)
{
    int i;
    for(i = 0; i < length; i++)
    {
        write_character(str[i]);
    }
}

int chardev_open(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "chardev: open called\n");
    clear_display();

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

ssize_t chardev_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp)
{
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

    write_string(buffer, count);

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

    clear_display_pins(LCD_ALL_PINS);

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

    //See datasheet page 34 on info for initialization sequence
    //function set
    clear_display_pins(LCD_ALL_PINS);
    set_display_pins(LCD_FUNCTION_SET | LCD_8BIT | LCD_2LINE | LCD_5x8);
    trigger_enable_pulse();

    //display set
    clear_display_pins(LCD_ALL_PINS);
    set_display_pins(LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON | LCD_CURSOR_ON | LCD_CURSOR_BLINK);
    trigger_enable_pulse();

    //clear display
    clear_display();

    mdelay(2000); //TODO: required?

    //entry mode set
    clear_display_pins(LCD_ALL_PINS);
    mdelay(1);
    set_display_pins(LCD_ENTRY_MODE_SET | LCD_INCREMENT);
    trigger_enable_pulse();

    // mdelay(500); //TODO: required?
    mdelay(1000);

    write_string("Hello world!", 12);

    clear_display_pins(LCD_ALL_PINS);
    set_display_pins(LCD_SET_DDRAM_ADDRESS | 41 << 3);
    trigger_enable_pulse();

#ifdef ENABLE_WORD_WRAP

    current_cursor_pos = 41;

#endif

    mdelay(1000);

    write_string("Hello world!", 12);

    mdelay(2000);

    clear_display();

    return 0;

    fail:
        chardev_exit_module();
        return result;
}

module_init(chardev_init_module);
module_exit(chardev_exit_module);
