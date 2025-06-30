

KERN_DIR = /home/book/100ask_imx6ull-sdk/Linux-4.9.88

all:
	make -C $(KERN_DIR) M=`pwd` modules 
	$(CROSS_COMPILE)gcc -o ledtest ledtest.c 

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order
	rm -f ledtest

100ask_led-y := leddrv.o chip_imx6ull_gpio.o board_imx6ull_led.o
obj-m	+= 100ask_led.o
