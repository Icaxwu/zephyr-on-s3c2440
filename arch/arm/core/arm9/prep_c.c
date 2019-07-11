/*
 * Copyright (c) 2013-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Full C support initialization
 *
 *
 * Initialization of full C support: zero the .bss, copy the .data if XIP,
 * call z_cstart().
 *
 * Stack is available in this module, but not the global data/bss until their
 * initialization is performed.
 */

#include <kernel.h>
#include <zephyr/types.h>
#include <toolchain.h>
#include <linker/linker-defs.h>
#include <kernel_internal.h>
#include <arm9/stack.h>

#if defined(__GNUC__)
/*
 * GCC can detect if memcpy is passed a NULL argument, however one of
 * the cases of relocate_vector_table() it is valid to pass NULL, so we
 * supress the warning for this case.  We need to do this before
 * string.h is included to get the declaration of memcpy.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnonnull"
#endif

#include <string.h>

#ifdef CONFIG_INIT_STACKS
static inline void init_stacks(void)
{
	memset(&_interrupt_stack, 0xAA, CONFIG_ISR_STACK_SIZE);
}
#endif

#define	GPFCON		(*(volatile unsigned long *)0x56000050)
#define	GPFDAT		(*(volatile unsigned long *)0x56000054)

#define	GPF4_out	(1<<(4*2))
#define	GPF5_out	(1<<(5*2))
#define	GPF6_out	(1<<(6*2))
#define	GPF7_out	(1<<(7*2))
void wait(int dly)
{
	volatile int loop = dly;
	for(; loop > 0; loop--);
}

extern FUNC_NORETURN void z_cstart(void);
/**
 *
 * @brief Prepare to and run C code
 *
 * This routine prepares for the execution of and runs C code.
 *
 * @return N/A
 */

extern void z_IntLibInit(void);

#ifdef CONFIG_BOOT_TIME_MEASUREMENT
	extern u64_t __start_time_stamp;
#endif
void _PrepC(void)
{
#ifdef CONFIG_INIT_STACKS
	init_stacks();
#endif
	{
		// 将LED1-3对应的GPF4/5/6三个引脚设为输出
		GPFCON = GPF4_out|GPF5_out|GPF6_out;

		GPFDAT &= ~(1<<4);
	}

	z_bss_zero();
	z_data_copy();
#ifdef CONFIG_BOOT_TIME_MEASUREMENT
	__start_time_stamp = 0U;
#endif
	z_IntLibInit();
	z_cstart();
	CODE_UNREACHABLE;
}
