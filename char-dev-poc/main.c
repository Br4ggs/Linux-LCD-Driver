/* This small Proof Of Concept demonstrates how a char device kernel module
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

MODULE_AUTHOR("Emiel vd Brink");
MODULE_LICENSE("Dual BSD/GPL");

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

ssize_t chardev_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp)
{
    printk(KERN_ALERT "chardev: write called\n");
    return 0;
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

    if (chardev_cdev) cdev_del(chardev_cdev);

    dev_t dev = MKDEV(15, 0);

    unregister_chrdev_region(dev, 1);
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

    return 0;

    fail:
        chardev_exit_module();
        return result;
}

module_init(chardev_init_module);
module_exit(chardev_exit_module);
