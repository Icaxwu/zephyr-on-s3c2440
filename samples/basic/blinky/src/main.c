/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <gpio.h>

#define LED_PORT LED0_GPIO_CONTROLLER
#define LED	LED0_GPIO_PIN

/* 1000 msec = 1 sec */
#define SLEEP_TIME 	1000

void  wait(volatile unsigned long dly)
{
	for(; dly > 0; dly--);
}

void main(void)
{
	#if 0
	int cnt = 0;
	struct device *dev;

	dev = device_get_binding(LED_PORT);
	/* Set LED pin as output */
	gpio_pin_configure(dev, LED, GPIO_DIR_OUT);

	while (1) {
		/* Set pin to HIGH/LOW every 1 second */
		gpio_pin_write(dev, LED, cnt % 2);
		cnt++;
		k_sleep(SLEEP_TIME);
	}
	#else

	// 将LED1-3对应的GPF4/5/6三个引脚设为输出
	(*(volatile unsigned long *)0x56000050) = (1<<(4*2))|(1<<(5*2))|(1<<(6*2));
	// 默认全部关闭
	(*(volatile unsigned long *)0x56000054) = (7<<4);

	while(1){
		//wait(30000);
		
		(*(volatile unsigned long *)0x56000054) ^= (3<<4);

		k_sleep(1000);
	}
	
	#endif
}
