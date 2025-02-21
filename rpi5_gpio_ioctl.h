#ifndef RPI5_GPIO_IOCTL_H
#define RPI5_GPIO_IOCTL_H

#define GPIO_OFFSET 571
#define GPIO_MAX 28

#define GPIO_INPUT 0
#define GPIO_OUTPUT 1

#define DRIVER_NAME "rpi5_gpio_ioctl"

#define RPI5_GPIO_MAGIC '='
#define GPIO_DIRECTION _IOW(RPI5_GPIO_MAGIC, 1, rpi5_gpio_t *)
#define GPIO_SET_VALUE _IOW(RPI5_GPIO_MAGIC, 2, rpi5_gpio_t *)
#define GPIO_GET_VALUE _IOW(RPI5_GPIO_MAGIC, 3, rpi5_gpio_t *)

typedef struct rpi5_gpio {
  unsigned int number;
  unsigned int direction;
  unsigned int value;
} rpi5_gpio_t;

#endif
