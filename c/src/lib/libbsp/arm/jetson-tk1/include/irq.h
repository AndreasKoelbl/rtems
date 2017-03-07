/*
 * Copyright (c) OTH Regensburg, 2017
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
#include <bsp.h>

#define TIMER_IRQ                   27

#define UARTA_IRQ                   (36 + 32)
#define UARTD_IRQ                   (90 + 32)

#define GPIO1_IRQ                   (32 + 32)
#define GPIO2_IRQ                   (33 + 32)
#define GPIO3_IRQ                   (34 + 32)
#define GPIO4_IRQ                   (35 + 32)
#define GPIO5_IRQ                   (55 + 32)
#define GPIO6_IRQ                   (87 + 32)
#define GPIO7_IRQ                   (89 + 32)
#define GPIO8_IRQ                   (125 + 32)


#define BSP_INTERRUPT_VECTOR_MIN    0
#define BSP_INTERRUPT_VECTOR_MAX    128

#endif /* ASM */
#endif /* LIBBSP_ARM_JETSONTK1_IRQ_H */
