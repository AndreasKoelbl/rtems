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
#include <bsp/tegra3-gpio.h>
#include <bsp/console.h>
#include <rtems/termiostypes.h>

#define PRIMARY_CPU       0x8

static uint8_t jailhouse_cpu_list[1] = {
  PRIMARY_CPU,
};
static uint32_t jailhouse_irq_list[1] = {
#if NS8250_CONSOLE_USE_INTERRUPTS == 1
  UARTD_IRQ,
#if NS8250_USE_SECONDARY_CONSOLE == 1
  UARTA_IRQ,
#endif
#endif
  GPIO6_IRQ,
};

#endif /* LIBBSP_ARM_JETSONTK1_JAILHOUSE_H */
