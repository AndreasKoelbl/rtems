/**
 * @file
 *
 * @ingroup arm_jetsontk1
 *
 * @brief This include file contains all board IO definitions of the Jetson TK1.
 */

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
#include <bsp/default-initial-extension.h>

#include <rtems.h>
#include <rtems/console.h>
#include <rtems/clockdrv.h>

#ifdef __cplusplus
extern "C" {
#endif

#define INMATE_BASE 0x00008000

/* FIXME values copied from altera cyclone, verify it */
#define BSP_ARM_A9MPCORE_SCU_BASE 0xf8f00000
#define BSP_ARM_GIC_CPUIF_BASE ( BSP_ARM_A9MPCORE_SCU_BASE + 0x00000100 )

#define BSP_ARM_A9MPCORE_GT_BASE ( BSP_ARM_A9MPCORE_SCU_BASE + 0x00000200 )

#define BSP_ARM_GIC_DIST_BASE ( BSP_ARM_A9MPCORE_SCU_BASE + 0x00001000 )

#define BSP_ARM_L2C_310_BASE 0xfffef000

#define BSP_ARM_L2C_310_ID 0x410000c9


/* functions */

rtems_isr_entry set_vector(                     /* returns old vector */
  rtems_isr_entry     handler,                  /* isr routine        */
  rtems_vector_number vector,                   /* vector number      */
  int                 type                      /* RTEMS or RAW intr  */
);

#ifdef __cplusplus
}
#endif
#endif /* LIBBSP_ARM_JETSONTK1_BSP_H */
