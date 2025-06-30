

KERN_DIR = /home/book/100ask_roc-rk3399-pc/linux-4.4

all:
	make -C $(KERN_DIR) M=`pwd` modules 
	$(CROSS_COMPILE)gcc -o ledtest ledtest.c 

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order
	rm -f ledtest

100ask_led-y := leddrv.o chip_demo_gpio.o board_A_led.o
obj-m	+= 100ask_led.o
