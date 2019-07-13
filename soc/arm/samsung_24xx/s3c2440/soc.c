#include "soc.h"

void _WdogInit(void)
{
	WTCON = 0;   // 关闭WATCHDOG很简单，往这个寄存器写0即可
}

void sdram_init(void)
{
#if 1
	BWSCON = 0x22000000;

	BANKCON6 = 0x18001;
	BANKCON7 = 0x18001;

	REFRESH  = 0x8404f5;

	BANKSIZE = 0xb1;

	MRSRB6   = 0x20;
	MRSRB7   = 0x20;
#else
	    volatile unsigned long *p = (volatile unsigned long *)MEM_CTL_BASE;

    /* 这个函数之所以这样赋值，而不是像前面的实�?(比如mmu实验)那样将配置�?
     * 写在数组中，是因为要生成”位置无关的代码”，使得这个函数可以在被复制�?
     * SDRAM之前就可以在steppingstone中运�?
     */
    /* 存储控制�?13个寄存器的�? */
    p[0] = 0x22011110;     //BWSCON
    p[1] = 0x00000700;     //BANKCON0
    p[2] = 0x00000700;     //BANKCON1
    p[3] = 0x00000700;     //BANKCON2
    p[4] = 0x00000700;     //BANKCON3  
    p[5] = 0x00000700;     //BANKCON4
    p[6] = 0x00000700;     //BANKCON5
    p[7] = 0x00018005;     //BANKCON6
    p[8] = 0x00018005;     //BANKCON7
    
                                    /* REFRESH,
                                     * HCLK=12MHz:  0x008C07A3,
                                     * HCLK=100MHz: 0x008C04F4
                                     */ 
    p[9]  = 0x008C04F4;
    p[10] = 0x000000B1;     //BANKSIZE
    p[11] = 0x00000030;     //MRSRB6
    p[12] = 0x00000030;     //MRSRB7
#endif
}

void clock_init(void)
{
	/* 设置MPLL, FCLK : HCLK : PCLK = 400m : 100m : 50m */
	LOCKTIME = 0xFFFFFFFF;      // 使用默认值即�?
	CLKDIVN  = 0x05;            // FCLK:HCLK:PCLK=1:4:8

    /* 如果HDIVN�?0，CPU的总线模式应该从“fast bus mode”变为“asynchronous bus mode�? */
	__asm__(
		"mrc    p15, 0, r1, c1, c0, 0\n"        /* 读出控制寄存�? */ 
		"orr    r1, r1, #0xc0000000\n"          /* 设置为“asynchronous bus mode�? */
		"mcr    p15, 0, r1, c1, c0, 0\n"        /* 写入控制寄存�? */
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
}
