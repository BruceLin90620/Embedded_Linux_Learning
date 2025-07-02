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
#include <linux/platform_device.h>
#include <linux/of.h>

#include "led_opr.h"
#include "leddrv.h"
#include "led_resource.h"

static int g_ledpins[100];
static int g_ledcnt = 0;

static int board_demo_led_init (int which) {
    printk("init gpio: group %d, pin %d\n", GROUP(g_ledpins[which]), PIN(g_ledpins[which]));
    return 0;
}

static int board_demo_led_ctl (int which, char status) {
    printk("set led %s: group %d, pin %d\n", status ? "on" : "off", GROUP(g_ledpins[which]), PIN(g_ledpins[which]));
    return 0;
}

static struct led_operations board_demo_led_opr = {
    .init = board_demo_led_init,
    .ctl  = board_demo_led_ctl,
};

struct led_operations *get_board_led_opr (void) {
    return &board_demo_led_opr;
}

static int chip_demo_gpio_probe (struct platform_device *pdev) {
    struct device_node *np;
    int err = 0;
    int led_pin;

    np = pdev->dev.of_node;
    if (!np)
        return -1;

    err = of_property_read_u32(np, "pin", &led_pin);

    g_ledpins[g_ledcnt] = led_pin;
    led_class_create_device(g_ledcnt);
    g_ledcnt++;
        
    return 0;
}

static int chip_demo_gpio_remove (struct platform_device *pdev) {
    int i = 0;
    int err;
    struct device_node *np;
    int led_pin;

    np = pdev->dev.of_node;
    if (!np)
        return -1;

    err = of_property_read_u32(np, "pin", &led_pin);

    for (i = 0; i < g_ledcnt; i++)
    {
        if (g_ledpins[i] == led_pin)
        {
            led_class_destroy_device(i);
            g_ledpins[i] = -1;
            break;
        };
    }

    for (i = 0; i < g_ledcnt; i++)
    {
        if (g_ledpins[i] != -1)
            break;
    }

    if (i == g_ledcnt)
        g_ledcnt = 0;
    
    return 0;
}

static const struct of_device_id ask100_leds[] = {
    { .compatible = "100ask,leddrv"},
    { },
};

static struct platform_driver chip_demo_gpio_driver = {
    .probe      = chip_demo_gpio_probe,
    .remove     = chip_demo_gpio_remove,
    .driver     = {
        .name   = "100ask_led",
        .of_match_table = ask100_leds,
    }, 
};

static int __init chip_demo_gpio_drv_init(void)
{
    int err;
    
    err = platform_driver_register(&chip_demo_gpio_driver); 
    register_led_operations(&board_demo_led_opr);
    
    return 0;
}

static void __exit lchip_demo_gpio_drv_exit(void)
{
    platform_driver_unregister(&chip_demo_gpio_driver);
}

module_init(chip_demo_gpio_drv_init);
module_exit(lchip_demo_gpio_drv_exit);

MODULE_LICENSE("GPL");