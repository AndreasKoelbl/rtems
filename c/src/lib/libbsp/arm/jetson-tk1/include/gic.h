/**
 * @file
 *
 * @ingroup jetsontk1_interrupt
 *
 * @brief Interrupt definitions.
 */

/*
 * Copyright (c) OTH Regensburg, 2016
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifndef LIBBSP_ARM_JETSONTK1_GIC_H
#define LIBBSP_ARM_JETSONTK1_GIC_H

#ifndef ASM

#include <rtems.h>
#include <rtems/irq-extension.h>

static inline void gic_setup_irq_stack(void)
{
	static __attribute__((aligned(0x1000))) uint32_t irq_stack[1024];

	asm volatile (".arch_extension virt\n");
	asm volatile ("msr	SP_irq, %0\n" : : "r" (irq_stack));
	asm volatile ("cpsie	i\n");
}

#endif /* ASM */
#endif /* LIBBSP_ARM_JETSONTK1_GIC_H */
