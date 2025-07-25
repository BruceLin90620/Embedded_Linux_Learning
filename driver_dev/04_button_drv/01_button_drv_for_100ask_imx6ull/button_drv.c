#include <linux/module.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>

#include "button_drv.h"

static int major = 0;

static struct class *button_class;
struct button_operations *p_button_opr;

static int button_drv_open (struct inode *node, struct file *file) {
    int minor = iminor(node);

    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

    p_button_opr->init(minor);

    return 0;
}

static ssize_t button_drv_read (struct file *file, char __user *buf, size_t size, loff_t *offset) {
    int err;
    char level;
    struct inode *inode = file_inode(file);
    int minor = iminor(inode);
    
    level = p_button_opr->read(minor);
    err = copy_to_user(buf, &level, 1);
	return 1;
}

static struct file_operations button_fops = {
	.open    = button_drv_open,
	.read    = button_drv_read,
};


void register_button_operations (struct button_operations *opr) {
    int i;

    p_button_opr = opr;
    for (i = 0; i < p_button_opr->count; i++) {
        device_create(button_class, NULL, MKDEV(major, i), NULL, "100ask_button%d", i);
    }
    p_button_opr = opr;
}

void unregister_button_operations (void) {
    int i;

    for (i = 0; i < p_button_opr->count; i++) {
        device_destroy(button_class, MKDEV(major, i));
    }
}

EXPORT_SYMBOL(register_button_operations);
EXPORT_SYMBOL(unregister_button_operations);


static int __init button_init (void) {
    int err;

    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
    major = register_chrdev(0, "100ask_button", &button_fops);

    button_class = class_create(THIS_MODULE, "100ask_button_class");
	err = PTR_ERR(button_class);
	if (IS_ERR(button_class)) {
		printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
		unregister_chrdev(major, "button");
		return -1;
	}
	
	return 0;
}

static void __exit button_exit(void)
{
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

	class_destroy(button_class);
	unregister_chrdev(major, "100ask_button");
}

module_init(button_init);
module_exit(button_exit);

MODULE_LICENSE("GPL");