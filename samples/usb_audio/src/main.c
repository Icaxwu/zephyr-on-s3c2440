/*
 * Copyright (c) 2017 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <gpio.h>
#include <misc/printk.h>
#include <misc/__assert.h>
#include <string.h>

#include "WM8976.h"

#define PORT0	 LED0_GPIO_CONTROLLER
#define PORT1	 LED1_GPIO_CONTROLLER

/* Change this if you have an LED connected to a custom pin */
#define LED0    LED0_GPIO_PIN
#define LED1    LED1_GPIO_PIN

void main(void)
{
    struct device *gpio_dev;
    static int cnt = 0;

    printk("boot main\n");

	gpio_dev = device_get_binding(PORT0);

	gpio_pin_configure(gpio_dev, LED0, GPIO_DIR_OUT);

    init_wm8976();

	while (1) {
		printk("k_sleep(500);\n");
        gpio_pin_write(gpio_dev, LED0, cnt++ % 2);
        k_busy_wait(500*1000);
	}
}
