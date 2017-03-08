/*
 * Copyright (c) OTH Regensburg, 2017
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifndef LIBBSP_ARM_JETSONTK1_GPIO_H
#define LIBBSP_ARM_JETSONTK1_GPIO_H

#define GPIO_BASE   ((void*)0x6000d000)
#define PINMUX_AUX  ((void*)0x70003000)

#define GPIO1_IRQ                   (32 + 32)
#define GPIO2_IRQ                   (33 + 32)
#define GPIO3_IRQ                   (34 + 32)
#define GPIO4_IRQ                   (35 + 32)
#define GPIO5_IRQ                   (55 + 32)
#define GPIO6_IRQ                   (87 + 32)
#define GPIO7_IRQ                   (89 + 32)
#define GPIO8_IRQ                   (125 + 32)

#endif /* LIBBSP_ARM_JETSONTK1_GPIO_H */
