#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/mutex.h>
#include <linux/wait.h>
#include <asm/uaccess.h>
#include <linux/device.h>
#include <asm/io.h>

static int major;
static struct class *led_class;

/* registers */
// IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 address：0x02290000 + 0x14
static volatile unsigned int *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3;

// GPIO5_GDIR address：0x020AC004
static volatile unsigned int *GPIO5_GDIR;

//GPIO5_DR address：0x020AC000
static volatile unsigned int *GPIO5_DR;




static ssize_t led_write(struct file *filp, const char __user *buf,
			 size_t count, loff_t *ppos)
{
    char val;
    int ret; 

    ret = copy_from_user(&val, buf, 1);

    if (val)
    {
        *GPIO5_DR &= !(1<<3);
    }
    else
    {
        *GPIO5_DR |= (1<<3);
    }
    return 1;
}

static int led_open(struct inode *inode, struct file *filp)
{
    *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 &= ~0xf;
    *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 |= 0x5;

    *GPIO5_GDIR |= (1<<3);

    return 0;
}

static const struct file_operations led_fops = {
	.owner		= THIS_MODULE,
	.write		= led_write,
	.open		= led_open,
};

static int __init led_init(void)
{
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);

	major = register_chrdev(0, "imx6ull_led", &led_fops);

    IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 = ioremap(0x02290000 + 0x14, 4);

    GPIO5_GDIR = ioremap(0x020AC004, 4);

    GPIO5_DR = ioremap(0x020AC000, 4);

    led_class = class_create(THIS_MODULE, "myled");

    device_create(led_class, NULL, MKDEV(major, 0), NULL, "myled");

	return 0;
}

static void __exit led_exit(void)
{
    iounmap(IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3);
    iounmap(GPIO5_GDIR);
    iounmap(GPIO5_DR);

    device_destroy(led_class, MKDEV(major, 0));
    class_destroy(led_class);
	unregister_chrdev(major, "imx6ull_led");
}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");