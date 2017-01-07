/*
 * Copyright (c) OTH Regensburg, 2016
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifndef LIBBSP_ARM_JETSONTK1_BSP_H
#define LIBBSP_ARM_JETSONTK1_BSP_H

#include <bspopts.h>
#include <rtems.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define BSP_ARM_GIC_DIST_BASE  ((void *)0x50041000)
#define BSP_ARM_GIC_CPUIF_BASE ((void *)0x50042000)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LIBBSP_ARM_JETSONTK1_BSP_H */
