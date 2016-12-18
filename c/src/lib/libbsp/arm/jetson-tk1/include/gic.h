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


#define GICD_V2_BASE	((void *)0x50041000)
#define GICC_V2_BASE	((void *)0x50042000)

#define GICD_CTLR			0x0000
# define GICD_CTLR_ARE_NS		(1 << 4)
#define GICD_TYPER			0x0004
#define GICD_IIDR			0x0008
#define GICD_IGROUPR			0x0080
#define GICD_ISENABLER			0x0100
#define GICD_ICENABLER			0x0180
#define GICD_ISPENDR			0x0200
#define GICD_ICPENDR			0x0280
#define GICD_ISACTIVER			0x0300
#define GICD_ICACTIVER			0x0380
#define GICD_IPRIORITYR			0x0400
#define GICD_ITARGETSR			0x0800
#define GICD_ICFGR			0x0c00
#define GICD_NSACR			0x0e00
#define GICD_SGIR			0x0f00
#define GICD_CPENDSGIR			0x0f10
#define GICD_SPENDSGIR			0x0f20
#define GICD_IROUTER			0x6000

#define GICD_PIDR2_ARCH(pidr)		(((pidr) & 0xf0) >> 4)


/* GICv2 specific */
#define GICC_CTLR		0x0000
#define GICC_PMR		0x0004
#define GICC_IAR		0x000c
#define GICC_EOIR		0x0010

#define GICC_CTLR_GRPEN1	(1 << 0)

#define GICC_PMR_DEFAULT	0xf0

#endif /* ASM */
#endif /* LIBBSP_ARM_JETSONTK1_GIC_H */
