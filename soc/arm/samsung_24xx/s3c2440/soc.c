#include "soc.h"

void _WdogInit(void)
{
	WTCON = 0;   // 关闭WATCHDOG很简单，往这个寄存器写0即可
}

/*
 * 启动ICACHE
 */
void enable_ICACNE(void)
{
    __asm__ volatile (
		"mrc    p15, 0, r0, c1, c0, 0\n"		/* 读出控制寄存器 */ 
		"orr    r0, r0, #(1<<12)\n"
		"mcr    p15, 0, r0, c1, c0, 0\n"	/* 写入控制寄存器 */
		:::"r0"
    );
}

/*
 * 启动ICACHE
 */
void enable_DCACNE(void)
{
    __asm__ volatile (
		"mrc    p15, 0, r0, c1, c0, 0\n"		/* 读出控制寄存器 */ 
		"orr    r0, r0, #(1<<4)\n"
		"mcr    p15, 0, r0, c1, c0, 0\n"	/* 写入控制寄存器 */
		:::"r0"
    );
}

void sdram_init(void)
{
	BWSCON = 0x22000000;

	BANKCON6 = 0x18001;
	BANKCON7 = 0x18001;

	REFRESH  = 0x8404f5;

	BANKSIZE = 0xb1;

	MRSRB6   = 0x20;
	MRSRB7   = 0x20;
}

/* 被系统接口k_busy_wait调用,  在enable icache的情况下测量
   time_us = 500000, 457ms
   time_us = 1000, 0.9125ms */
void z_arch_busy_wait(unsigned int time_us)
{
    for(volatile unsigned int i = 0; i < time_us*3; i++)
        ;
}

void clock_init(void)
{
	/* 设置MPLL, FCLK : HCLK : PCLK = 400m : 100m : 50m */
	LOCKTIME = 0xFFFFFFFF;      // 使用默认值
	CLKDIVN  = 0x05;            // FCLK:HCLK:PCLK=1:4:8

    /* If HDIVN is not 0, the CPU bus mode has to be changed from the fast bus mode to 
       the asynchronous bus mode  */
	__asm__(
		"mrc    p15, 0, r1, c1, c0, 0\n"        /* 读出控制寄存 */ 
		"orr    r1, r1, #0xc0000000\n"          /* 设置为“asynchronous bus mode */
		"mcr    p15, 0, r1, c1, c0, 0\n"        /* 写入控制寄存 */
    );

    MPLLCON = (92<<12)|(1<<4)|(1<<0); 
}

void _copy2sdram(void)
{
	extern unsigned int _image_rom_start;
	extern unsigned int _flash_used;

	volatile unsigned int *dest = (volatile unsigned int *)&_image_rom_start;
	volatile unsigned int *end = (volatile unsigned int *)((volatile unsigned char *)&_image_rom_start + (unsigned int)&_flash_used);
	volatile unsigned int *src = (volatile unsigned int *)0;

	while (dest < end)
	{
		*dest++ = *src++;
	}
}

void _PlatformInit(void)
{
	clock_init();
	
	sdram_init();

    enable_ICACNE();
}
