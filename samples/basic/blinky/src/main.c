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

#define TEST_THREAD1_STACK_SIZE         512
#define TEST_THREAD1_PRIIO              1
struct k_thread test_thread1;
k_thread_stack_t test_thread1_stack[TEST_THREAD1_STACK_SIZE];

#define TEST_THREAD2_STACK_SIZE         512
#define TEST_THREAD2_PRIIO              -1
struct k_thread test_thread2;
k_thread_stack_t test_thread2_stack[TEST_THREAD2_STACK_SIZE];


void  wait(volatile unsigned long dly)
{
	for(; dly > 0; dly--);
}

void test_thread_loop1(void *p1, void *p2, void *p3)
{
	while(1)
	{
		(*(volatile unsigned long *)0x56000054) ^= (2<<4);
		k_sleep(500);
	}
}

void test_thread_loop2(void *p1, void *p2, void *p3)
{
	while(1)
	{
		(*(volatile unsigned long *)0x56000054) ^= (1<<4);
		k_sleep(1000);
	}
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
	k_thread_create(&test_thread1, test_thread1_stack, 
			TEST_THREAD1_STACK_SIZE, test_thread_loop1,
			NULL, NULL, NULL,
			TEST_THREAD1_PRIIO, 0, 0);
	
	k_thread_create(&test_thread2, test_thread2_stack, 
			TEST_THREAD2_STACK_SIZE, test_thread_loop2,
			NULL, NULL, NULL,
			TEST_THREAD2_PRIIO, 0, 0);
	// 将LED1-3对应的GPF4/5/6三个引脚设为输出
	(*(volatile unsigned long *)0x56000050) = (1<<(4*2))|(1<<(5*2))|(1<<(6*2));
	// 默认全部关闭
	(*(volatile unsigned long *)0x56000054) = (7<<4);

	while(1){
	
		(*(volatile unsigned long *)0x56000054) ^= (4<<4);
		k_sleep(250);
	}
	
	#endif
}
