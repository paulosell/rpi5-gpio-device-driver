#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/gpio/consumer.h>
#include <linux/ioctl.h>

#include "rpi5_gpio_ioctl.h"

static dev_t device;
static struct class * class;
static struct cdev class_device;

#define DRIVER_CLASS "rpi5_gpio_class"

static int rpi5_gpio_direction(rpi5_gpio_t * gpio){
  int status = -1;
  struct gpio_desc * gpiod;

  gpiod = gpio_to_desc(gpio->number + GPIO_OFFSET);

   if (gpio->direction == GPIO_INPUT){
    status = gpiod_direction_input(gpiod);
  } else if (gpio->direction == GPIO_OUTPUT) {
    status = gpiod_direction_output(gpiod, gpio->value);
  }

  return status;
}

static int rpi5_gpio_set_value(rpi5_gpio_t * gpio){
  struct gpio_desc * gpiod;

  gpiod = gpio_to_desc(gpio->number + GPIO_OFFSET);

  gpiod_set_value(gpiod, gpio->value);

  return gpio->value;
}

static int rpi5_gpio_get_value(rpi5_gpio_t * gpio){ 
  struct gpio_desc * gpiod;

  gpiod = gpio_to_desc(gpio->number + GPIO_OFFSET);

  gpio->value = gpiod_get_value(gpiod);

  return gpio->value;
}

static long int rpi5_gpio_ioctl(struct file *f, unsigned cmd, unsigned long arg){
  int copied, status = 0;
  rpi5_gpio_t gpio;
  
  copied = copy_from_user(&gpio, (rpi5_gpio_t *) arg, sizeof(rpi5_gpio_t));

  switch(cmd){
    case GPIO_DIRECTION: {
      status = rpi5_gpio_direction(&gpio);
      break;
    }
    case GPIO_SET_VALUE: {
      status = rpi5_gpio_set_value(&gpio);
      break;
    }
    case GPIO_GET_VALUE: {
      status = rpi5_gpio_get_value(&gpio);
      copied = copy_to_user((void __user *) arg, &gpio, sizeof(gpio));
      break;
    }
    default:
      status = -1;
      break;
  }

  if (status < 0) {
    pr_err("rpi5_gpio_ioctl - An error occurred %d", status);
  }

  return status;

}

static int open_dev(struct inode *dev, struct file *f){
  return 0;
}

static int close_dev(struct inode *dev, struct file *f){
  return 0;
}


static struct file_operations fops = {
  .owner = THIS_MODULE,
  .open = open_dev,
  .release = close_dev,
  .unlocked_ioctl = rpi5_gpio_ioctl,
};

static int __init init_dev(void){

  pr_info("Starting rpi5_gpio_ioctl driver");

  // allocate device number 
  if (alloc_chrdev_region(&device, 0, 1, DRIVER_NAME) < 0){
    pr_err("rpi5_gpio_ioctl - Error trying to allocate device number");
    return -1;
  }

  pr_debug("rpi5_gpio_ioctl - Device major %d", device >> 20);
  pr_debug("rpi5_gpio_ioctl - Device minor %d", device & 0xfffff);

  // create device class
  class = class_create(DRIVER_CLASS);
  if (class == NULL) {
    pr_err("rpi5_gpio_ioctl - Error creating device class");
    goto ClassError;
  }

  // create device file
  if (device_create(class, NULL, device, NULL, DRIVER_NAME) == NULL) {
    pr_err("rpi5_gpio_ioctl - Error creating device file");
    goto FileError;
  }

  // initialize and register device file
  cdev_init(&class_device, &fops);
  if (cdev_add(&class_device, device, 1) == -1) {
    pr_err("rpi5_gpio_ioctl - Error registering device to Kernel");
    goto AddError;
  }

  return 0;

AddError:
  device_destroy(class, device);
FileError:
  class_destroy(class);  
ClassError:
  unregister_chrdev(device, DRIVER_NAME);
  return -1;
}

static void __exit exit_dev(void){
  pr_info("rpi5_gpio_ioctl - Removing rpi5_gpio_ioctl driver");

  cdev_del(&class_device);
  device_destroy(class, device);
  class_destroy(class);
  unregister_chrdev(device, DRIVER_NAME);
}

module_init(init_dev);
module_exit(exit_dev);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("pfs");
MODULE_DESCRIPTION("A device driver to control GPIO over ioctl calls");
