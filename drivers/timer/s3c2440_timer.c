#include <soc.h>
#include <clock_control.h>
#include <system_timer.h>
#include <sys_clock.h>
#include <spinlock.h>

#define COUNTER_MAX  (31250)
#define CYC_PER_TICK (312)

static u32_t last_count;

void clock_isr(void *arg)
{
	//(*(volatile unsigned long *)0x56000054) ^= (4<<4);
	z_clock_announce(1);
}

int z_clock_driver_init(struct device *device)
{
	ARG_UNUSED(device);

	/* 设置TIMER0的时�?*/
	/* Timer clk = PCLK / {prescaler value+1} / {divider value} 
	             = 50000000/(99+1)/16
	             = 31250
	 */
	TCFG0 = 99;  /* Prescaler 0 = 99, 用于timer0,1 */
	TCFG1 &= ~0xf;
	TCFG1 |= 3;  /* MUX0 : 1/16 */

	/* 设置TIMER0的初�?*/
	TCNTB0 = 312;  /* 10ms中断一�?*/

	/* 加载初�? 启动timer0 */
	TCON |= (1<<1);   /* Update from TCNTB0 & TCMPB0 */

	/* 设置为自动加载并启动 */
	TCON &= ~(1<<1);
	TCON |= (1<<0) | (1<<3);  /* bit0: start, bit3: auto reload */

	IRQ_CONNECT(10, 1, clock_isr, 0, 0);
	irq_enable(10);

	return 0;
}

u32_t z_clock_elapsed(void)
{
    return 0;
}

u32_t z_timer_cycle_get_32(void)
{

	return 0;
}

void z_clock_set_timeout(s32_t ticks, bool idle)
{
	ARG_UNUSED(idle);
}
