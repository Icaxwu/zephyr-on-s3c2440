/*
 * Copyright (c) 2013-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#ifndef ZEPHYR_INCLUDE_ARCH_ARM_ARM9_MISC_H_
#define ZEPHYR_INCLUDE_ARCH_ARM_ARM9_MISC_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ASMLANGUAGE
extern void k_cpu_idle(void);

extern u32_t z_timer_cycle_get_32(void);
#define z_arch_k_cycle_get_32()	z_timer_cycle_get_32()

/**
 * @brief Explicitly nop operation.
 */
static ALWAYS_INLINE void arch_nop(void)
{
	__asm__ volatile("nop");
}

#endif

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_ARM_ARM9_MISC_H_ */
