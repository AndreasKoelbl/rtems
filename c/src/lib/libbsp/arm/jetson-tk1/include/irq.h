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

#define TIMER_IRQ                   27

#define BSP_INTERRUPT_VECTOR_MIN    0
/* TODO: Fix random number */
#define BSP_INTERRUPT_VECTOR_MAX    64

/**
 * @defgroup raspberrypi_interrupt Interrrupt Support
 *
 * @ingroup jetsontk1_interrupt
 *
 * @brief Interrupt support.
 */

#endif /* ASM */
#endif /* LIBBSP_ARM_JETSONTK1_IRQ_H */
