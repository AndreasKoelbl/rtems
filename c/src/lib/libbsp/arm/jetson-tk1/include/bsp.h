/*
 * Copyright (c) OTH Regensburg, 2017
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifndef LIBBSP_ARM_JETSONTK1_BSP_H
#define LIBBSP_ARM_JETSONTK1_BSP_H

#include <bspopts.h>
#include <rtems.h>
#include <bsp/default-initial-extension.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define BSP_ARM_GIC_DIST_BASE  ((void *)0x50041000)
#define BSP_ARM_GIC_CPUIF_BASE ((void *)0x50042000)

#define BSP_GPIO_PIN_COUNT      162
#define BSP_GPIO_PINS_PER_BANK  20

#define BSP_GPIO_PIN_COUNT      162
#define BSP_GPIO_PINS_PER_BANK  20

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LIBBSP_ARM_JETSONTK1_BSP_H */
