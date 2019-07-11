/*
 * Copyright (c) 2013-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Stack helpers for Cortex-M CPUs
 *
 * Stack helper functions.
 */

#ifndef ZEPHYR_ARCH_ARM_INCLUDE_ARM9_STACK_H_
#define ZEPHYR_ARCH_ARM_INCLUDE_ARM9_STACK_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _ASMLANGUAGE

/* nothing */

#else


extern K_THREAD_STACK_DEFINE(_interrupt_stack, CONFIG_ISR_STACK_SIZE);

/**
 *
 * @brief Setup interrupt stack
 *
 * On Cortex-M, the interrupt stack is registered in the MSP (main stack
 * pointer) register, and switched to automatically when taking an exception.
 *
 * @return N/A
 */
static ALWAYS_INLINE void z_InterruptStackSetup(void)
{

}

#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_ARCH_ARM_INCLUDE_ARM9_STACK_H_ */
