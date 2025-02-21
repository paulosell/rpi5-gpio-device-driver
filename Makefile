obj-m += rpi5_gpio_ioctl.o

all: clean device_driver test_gpio

device_driver:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f test_gpio

test_gpio:
	rm -f test_gpio
	gcc -o test_gpio test_gpio.c

.PHONY: all clean device_driver test_gpio
