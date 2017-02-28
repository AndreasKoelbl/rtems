/*
 * Copyright (c) OTH Regensburg, 2017
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifndef LIBBSP_ARM_JETSONTK1_JAILHOUSE_H
#define LIBBSP_ARM_JETSONTK1_JAILHOUSE_H

#include <bspopts.h>
#include <stdint.h>
#include <rtems/termiostypes.h>

#define PRIMARY_CPU       0x8

static uint8_t jailhouse_cpu_list[1] = {
  PRIMARY_CPU,
};
static uint32_t jailhouse_irq_list[2] = {
  (32 + 36) % 32,
  (90 + 32) % 32,
};

#endif /* LIBBSP_ARM_JETSONTK1_JAILHOUSE_H */
