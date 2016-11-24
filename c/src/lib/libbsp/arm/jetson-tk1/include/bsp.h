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

/* FIXME value copied from altera cyclone, verify it */
#define BSP_ARM_A9MPCORE_SCU_BASE 0xf8f00000

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
