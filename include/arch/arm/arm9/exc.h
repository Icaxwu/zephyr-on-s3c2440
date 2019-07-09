/*
 * Copyright (c) 2013-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief ARM9 public exception handling
 *
 * ARM-specific kernel exception handling interface. Included by arm/arch.h.
 */

#ifndef ZEPHYR_INCLUDE_ARCH_ARM_ARM9_EXC_H_
#define ZEPHYR_INCLUDE_ARCH_ARM_ARM9_EXC_H_

#ifdef __cplusplus
extern "C" {
#endif

/* for assembler, only works with constants */
#define Z_EXC_PRIO(pri) (((pri) << (8 - DT_NUM_IRQ_PRIO_BITS)) & 0xff)

#if defined(CONFIG_CPU_ARM9_HAS_PROGRAMMABLE_FAULT_PRIOS)
#define _EXCEPTION_RESERVED_PRIO 1
#else
#define _EXCEPTION_RESERVED_PRIO 0
#endif

#define _EXC_FAULT_PRIO 0
#ifdef CONFIG_ZERO_LATENCY_IRQS
#define _EXC_ZERO_LATENCY_IRQS_PRIO 0
#define _EXC_SVC_PRIO 1
#define _IRQ_PRIO_OFFSET (_EXCEPTION_RESERVED_PRIO + 1)
#else
#define _EXC_SVC_PRIO 0
#define _IRQ_PRIO_OFFSET (_EXCEPTION_RESERVED_PRIO)
#endif

#define _EXC_IRQ_DEFAULT_PRIO Z_EXC_PRIO(_IRQ_PRIO_OFFSET)

#ifdef _ASMLANGUAGE
GTEXT(z_ExcExit);
#else
#include <zephyr/types.h>

struct __esf {
	u32_t r0; 
	u32_t r1; 
	u32_t r2; 
	u32_t r3; 
	u32_t r4; 
	u32_t r5; 
	u32_t r6; 
	u32_t r7; 
	u32_t r8; 
	u32_t r9; 
	u32_t r10;
	u32_t r11;
	u32_t r12;
#ifdef CONFIG_FLOAT
#endif
};

typedef struct __esf NANO_ESF;

extern void z_ExcExit(void);

/**
 * @brief display the contents of a exception stack frame
 *
 * @return N/A
 */

extern void sys_exc_esf_dump(NANO_ESF *esf);

#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_ARM_ARM9_EXC_H_ */
