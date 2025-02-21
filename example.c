#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <errno.h>
#include <assert.h>

#include "rpi5_gpio_ioctl.h"

#define FILE_PATH "/dev/rpi5_gpio_ioctl"
#define GPIO_NUMBER 21

int main(){
  
  int fd;
  rpi5_gpio_t gpio_led;
  
  gpio_led.number = 21;
  gpio_led.direction = GPIO_OUTPUT;
  gpio_led.value = 0;

  chmod(FILE_PATH, 0666);
  fd = open(FILE_PATH, O_WRONLY);

  if (fd < 0){
    printf("Error opening file descriptor\n");
    return -1;
  }

  ioctl(fd, GPIO_DIRECTION, &gpio_led);
  assert(gpio_led.value == 0);
  ioctl(fd, GPIO_GET_VALUE, &gpio_led);
  assert(gpio_led.value == 0);

  sleep(5);

  gpio_led.value = 1;
  ioctl(fd, GPIO_SET_VALUE, &gpio_led);
  assert(gpio_led.value == 1);
  ioctl(fd, GPIO_GET_VALUE, &gpio_led);
  assert(gpio_led.value == 1);

  sleep(5);

  gpio_led.value = 0;
  ioctl(fd, GPIO_SET_VALUE, &gpio_led);
  assert(gpio_led.value == 0);
  ioctl(fd, GPIO_GET_VALUE, &gpio_led);
  assert(gpio_led.value == 0);

  printf("All tests passed\n");

  return 0;
    
}
