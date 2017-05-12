/*
 * Copyright (c) OTH Regensburg, 2017
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#include <bspopts.h>
#include <stdint.h>
#include <bsp/console.h>
#include <bsp/tegra124-gpio.h>

#define PRIMARY_CPU 0x8

const uint8_t jailhouse_cpu_list[ 1 ] = {
  PRIMARY_CPU,
};

const uint32_t jailhouse_irq_list[
  1 + NS8250_CONSOLE_USE_INTERRUPTS + NS8250_USE_SECONDARY_CONSOLE ] = {
#if NS8250_CONSOLE_USE_INTERRUPTS == 1
  UARTD_IRQ,
#if NS8250_USE_SECONDARY_CONSOLE == 1
  UARTA_IRQ,
#endif
#endif
  GPIO6_IRQ,
};
