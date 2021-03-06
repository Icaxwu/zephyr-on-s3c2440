/*
 * Copyright (c) 2013-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Reset handler
 *
 * Reset handler that prepares the system for running C code.
 */

#include <toolchain.h>
#include <linker/sections.h>
#include <arch/cpu.h>
#include "vector_table.h"
#include <soc.h>

_ASM_FILE_PROLOGUE

GTEXT(__reset)

/**
 *
 * @brief Reset vector
 *
 * Ran when the system comes out of reset. The processor is in thread mode with
 * privileged level. At this point, the main stack pointer (MSP) is already
 * pointing to a valid area in SRAM.
 *
 * Locking interrupts prevents anything but NMIs and hard faults from
 * interrupting the CPU. A default NMI handler is already in place in the
 * vector table, and the boot code should not generate hard fault, or we're in
 * deep trouble.
 *
 * We want to use the process stack pointer (PSP) instead of the MSP, since the
 * MSP is to be set up to point to the one-and-only interrupt stack during later
 * boot. That would not be possible if in use for running C code.
 *
 * When these steps are completed, jump to _PrepC(), which will finish setting
 * up the system for running C code.
 *
 * @return N/A
 */

SECTION_SUBSEC_FUNC(TEXT,_reset_section,__reset)

/*
 * The entry point is located at the __reset symbol, which
 * is fetched by a XIP image playing the role of a bootloader, which jumps to
 * it, not through the reset vector mechanism. Such bootloaders might want to
 * search for a __start symbol instead, so create that alias here.
 */
SECTION_SUBSEC_FUNC(TEXT,_reset_section,__start)

    msr cpsr_c, #0xd3          /* 进入svc模式，屏蔽irq和frq */
	ldr sp, =0x40000000+4096   /* 假设是nor启动, s3c2440的内置4Ksram隐射到0x40000000 */


    bl _PlatformInit

    /* board-specific watchdog initialization is necessary */
    bl _WdogInit

    bl _copy2sdram

    msr cpsr_c, #0xd2       @ 进入中断模式
    ldr sp, =0x30200000     @ 设置中断模式栈指针

	msr cpsr_c, #0xd7       @ 进入数据访问终止模式
    ldr sp, =0x30201000     @ 设置数据访问终止模式栈指针

	msr cpsr_c, #0xdb       @ 进入未定义指令中止模式
    ldr sp, =0x30202000     @ 设置未定义指令中止模式栈指针

	msr cpsr_c, #0xd3		@ 进入管理模式设置I-bit=1，关IRQ中断
    ldr sp, =0x30203000     @ 设置管理模式栈指针
	//ldr fp, =0x33f00000     @ 设置栈帧指针
    /*
     * 'bl' jumps the furthest of the branch instructions that are
     * supported on all platforms. So it is used when jumping to _PrepC
     * (even though we do not intend to return).
     */
    ldr pc, =_PrepC
