/*
 * Copyright (c) 2013-2015 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Populated vector table in ROM
 *
 * Vector table at the beginning of the image for starting system. The reset
 * vector is the system entry point, ie. the first instruction executed.
 *
 * The table is populated with all the system exception handlers. The NMI vector
 * must be populated with a valid handler since it can happen at any time. The
 * rest should not be triggered until the kernel is ready to handle them.
 */

#include <toolchain.h>
#include <linker/sections.h>
#include <offsets_short.h>

_ASM_FILE_PROLOGUE

GTEXT(_isr_wrapper)

SECTION_SUBSEC_FUNC(exc_vector_table,_vector_table_section,_vector_table)
    b   __reset

@ 0x04: 未定义指令中止模式的向量地址
HandleUndef:
    b   HandleUndef 
 
@ 0x08: 管理模式的向量地址，通过SWI指令进入此模式
__handleSWI:
    b   __handleSWI

@ 0x0c: 指令预取终止导致的异常的向量地址
HandlePrefetchAbort:
    b   HandlePrefetchAbort

@ 0x10: 数据访问终止导致的异常的向量地址
HandleDataAbort:
    b   HandleDataAbort

@ 0x14: 保留
HandleNotUsed:
    b   HandleNotUsed

@ 0x18: 中断模式的向量地址  
    ldr pc, = HandleIRQ

@ 0x1c: 快中断模式的向量地址
HandleFIQ:
    b   HandleFIQ


HandleIRQ:
#if 0
    /* 保存现场 */
	/* 在irq异常处理函数中有可能会修改r0-r12, 所以先保存 */
	/* lr-4是异常处理完后的返回地址, 也要保存 */
	sub lr, lr, #4
	stmdb sp!, {r0-r12, lr}  
	
	/* 处理irq异常 */
	bl _isr_wrapper
	
	bl _IntExit

	/* 恢复现场 */
	ldmia sp!, {r0-r12, pc}^  /* ^会把spsr_irq的值恢复到cpsr里 */
#else
    /************************************
    先不考虑中断嵌套，直接保存环境到thread里
    ************************************/
    stmdb	SP, {r0-r3}
    /* load _kernel into r1 and current k_thread into r2 */
    ldr r1, =_kernel
    ldr r2, [r1, #_kernel_offset_to_current]

    /* addr of callee-saved regs in thread in r0 */
    ldr r0, =_thread_offset_to_callee_saved
    add r0, r2
    add r0, #16   // reserved for r0 ~ r3
	/* 
		Store current r4-r13, PC, CPSR.
		当前lr就是等于__do_swap执行完毕后的return address，__do_swap的上一级函数的return address
		由上一级函数自己保持到栈里，__do_swap不需要关心。
	*/
    STMIA   r0!, {r4-r12}
    MRS		r3, CPSR
	MSR     CPSR, #0xd3			// Change to SVC mode, disable irq,fiq
    STMIA   r0!, {sp, lr}       // TASK'S SP, LR
    MSR		CPSR, r3		    // BACK TO IRQ MODE
    SUB     R3,   LR, #4
    STMIA	r0!, {R3}           // TASK'S PC
    MRS		r3,  spsr		
	STMIA	r0!, {r3}           // TASK'S CPSR


	/* addr of callee-saved regs in thread in r4 */
    ldr r4, =_thread_offset_to_callee_saved
    add r4, r2

	/* Store current r0-r3,LR*/
	LDMDB sp, {r0-r3}
    STMIA r4, {r0-r3}

    /* 处理irq异常 */
	bl _isr_wrapper

    bl _IntExit

    /************************************
    _IntExit中没有切换任务，则恢复到之前被中断的任务
    ************************************/
    ldr r0, =_kernel
    ldr r1, [r0, #_kernel_offset_to_current]

	ldr r2, =_thread_offset_to_callee_saved
    adds r2, r1 

    ldr r3, [r2, #16*4]   // offset of cpsr: 16*4
    msr spsr, r3

    ldmia r2, {r0-r12, sp, lr, pc}^ 
#endif
