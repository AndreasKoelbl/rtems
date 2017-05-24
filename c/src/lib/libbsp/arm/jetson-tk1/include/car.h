/*
 * Copyright (c) OTH Regensburg, 2017
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifndef LIBBSP_ARM_JETSONTK1_CAR_H
#define LIBBSP_ARM_JETSONTK1_CAR_H

#include <bspopts.h>

#define CAR ( (void *) 0x60006000 )

/* UART clock source (fixed): PLLC_OUT0 */
#define CAR_SOURCE_DEFAULT_UART 2

#define CAR_DEV_UARTD 0 /* U */
#define CAR_DEV_UARTA 1 /* L */

#if NS8250_USE_SECONDARY_CONSOLE == 0
#define CAR_NUM_DEV 1
#else
#define CAR_NUM_DEV 2
#endif

typedef struct {
  void* enb_set;
  void* enb_clear;
} clock_dev;

typedef struct {
  void* enb_set;
  void* enb_clear;
} reset_dev;

typedef const struct {
  clock_dev clock;
  reset_dev reset;
  void* src_reg;
  uint32_t gate;
} car_dev;


#endif /* LIBBSP_ARM_JETSONTK1_CAR_H */
