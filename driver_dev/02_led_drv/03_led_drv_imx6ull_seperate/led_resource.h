#ifndef _LED_RESOURCE_H
#define _LED_RESOURCE_H


struct led_resource {
	unsigned int iomuxc_mux_ctl_pad_phys;
	unsigned int iomuxc_mux_ctl_pad_val;
	unsigned int gpio_gdir_phys;
	unsigned int gpio_dr_phys;
	unsigned int gpio_pin_num;

};

struct led_resource *get_led_resouce(void);

#endif

