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

#ifndef LIBBSP_ARM_JETSONTK1_IRQ_H
#define LIBBSP_ARM_JETSONTK1_IRQ_H

#ifndef ASM

#include <rtems.h>
#include <rtems/irq.h>
#include <rtems/irq-extension.h>

/* architecture specific (tegra)*/
#define TIMER_IRQ                   27

#define GICD_V2_BASE	((void *)0x50041000)
#define GICC_V2_BASE	((void *)0x50042000)

/* GICv2 specific */
#define GICC_CTLR		0x0000
#define GICC_PMR		0x0004
#define GICC_IAR		0x000c
#define GICC_EOIR		0x0010

#define GICC_CTLR_GRPEN1	(1 << 0)

#define GICC_PMR_DEFAULT	0xf0


#define BSP_INTERRUPT_VECTOR_MIN    0
/* TODO: Fix random number */
#define BSP_INTERRUPT_VECTOR_MAX    64

#endif /* ASM */
#endif /* LIBBSP_ARM_JETSONTK1_IRQ_H */
