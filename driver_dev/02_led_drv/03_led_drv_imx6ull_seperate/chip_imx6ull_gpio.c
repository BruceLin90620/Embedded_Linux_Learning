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
#include <asm/io.h>
#include "led_opr.h"
#include "led_resource.h"

static volatile unsigned int *IOMUXC_MUX_CTL_PAD;
static volatile unsigned int *GPIO5_GDIR;
static volatile unsigned int *GPIO5_DR;

static struct led_resource *led_rsc;

static int chip_imx6ull_led_init (int which)  
{	
	//printk("%s %s line %d, led %d\n", __FILE__, __FUNCTION__, __LINE__, which);
	if (!led_rsc)
	{
		led_rsc = get_led_resouce();
	}

    IOMUXC_MUX_CTL_PAD = ioremap(led_rsc->iomuxc_mux_ctl_pad_phys, 4);
    GPIO5_GDIR = ioremap(led_rsc->gpio_gdir_phys, 4);
    GPIO5_DR = ioremap(led_rsc->gpio_dr_phys, 4);	
	
	*IOMUXC_MUX_CTL_PAD &= ~0xf;
	*IOMUXC_MUX_CTL_PAD |= led_rsc->iomuxc_mux_ctl_pad_val;
	
	*GPIO5_GDIR |= (1 << led_rsc->gpio_pin_num);
	return 0;
}

static int chip_imx6ull_led_ctl (int which, char status) /* 控制LED, which-哪个LED, status:1-亮,0-灭 */
{
	if (status)
	{
		*GPIO5_DR &= ~(1 << led_rsc->gpio_pin_num);
	}
	else
	{
		*GPIO5_DR |= (1 << led_rsc->gpio_pin_num);
	}

	return 0;
}

static struct led_operations board_imx6ull_led_opr = {
	.init = chip_imx6ull_led_init,
	.ctl  = chip_imx6ull_led_ctl,
};

struct led_operations *get_board_led_opr(void)
{
	return &board_imx6ull_led_opr;
}

