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
#include <bsp/arm-gic-irq.h>

#define TIMER_IRQ                   27
#define UARTA_IRQ                   (36 + 32)
#define UARTD_IRQ                   (90 + 32)

#define BSP_INTERRUPT_VECTOR_MIN    0
#define BSP_INTERRUPT_VECTOR_MAX    128

#endif /* ASM */
#endif /* LIBBSP_ARM_JETSONTK1_IRQ_H */
