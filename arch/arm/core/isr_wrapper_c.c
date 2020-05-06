#include <soc.h>
#include <sw_isr_table.h>

#if defined(CONFIG_ARMV4T)

void _isr_wrapper(void)
{
    /* 分辨中断源 */
	int bit = INTOFFSET;

	/* 调用对应的处理函数 */
	_sw_isr_table[bit].isr((void*)bit);
	
	/* 清中断 : 从源头开始清 */
	SRCPND = (1<<bit);
	INTPND = (1<<bit);	
}

#endif
