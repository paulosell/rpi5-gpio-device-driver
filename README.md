# Raspberry Pi 5 GPIO IOCTL Driver

## Description
This is a Linux kernel module that provides an IOCTL-based interface to control GPIO pins on a Raspberry Pi 5. It allows userspace applications to set the direction of GPIO pins and read/write their values.

## Features
- Configure GPIO pins as input or output.
- Set and get GPIO values using IOCTL system calls.
- Simple character device interface for userspace communication.

## Requirements
- Raspberry Pi 5
- Linux kernel with GPIO support
- Kernel headers installed (`linux-headers-$(uname -r)`) for module compilation

## Building and Installing the Driver

### Step 1: Compile the Kernel Module
Run the following command in the project directory:

```sh
make device_driver
```

### Step 2: Load the Kernel Module
After compiling, load the module into the kernel using:

```sh
sudo insmod rpi5_gpio_ioctl.ko
```

### Step 3: Verify Module Loading
Check if the module is loaded successfully:

```sh
dmesg | tail
lsmod | grep rpi5_gpio_ioctl
```

### Step 4: Create Device Node
If the module is loaded but the device node is not created automatically, create it manually:

```sh
sudo mknod /dev/rpi5_gpio_ioctl c <major_number> 0
sudo chmod 666 /dev/rpi5_gpio_ioctl
```

To find `<major_number>`, run:

```sh
dmesg | grep "rpi5_gpio_ioctl - Device major"
```

## Running the Userspace Test Program

### Step 1: Compile the Test Program
Compile the test program using:

```sh
make test_gpio
```

### Step 2: Run the Test
Execute the test program:

```sh
./test_gpio
```

If successful, the program will configure GPIO 21 as an output, toggle its value, and verify it.

## Unloading the Driver
To remove the kernel module, run:

```sh
sudo rmmod rpi5_gpio_ioctl
```

## Cleaning Up
To remove compiled files, use:

```sh
make clean
```

## License
This project is licensed under the GPL.

## Author
pfs
