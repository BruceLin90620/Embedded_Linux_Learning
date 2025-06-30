
#include "led_resource.h"

static struct led_resource board_imx6ull_led = {
	.iomuxc_mux_ctl_pad_phys = 0x02290000 + 0x14,
	.iomuxc_mux_ctl_pad_val = 0x5,
	.gpio_gdir_phys = 0x020AC004,
	.gpio_dr_phys = 0x020AC000,
	.gpio_pin_num = 3,
};

struct led_resource *get_led_resouce(void)
{
	return &board_imx6ull_led;
}


