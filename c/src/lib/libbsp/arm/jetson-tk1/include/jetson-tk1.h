/**
 * @file
 *
 * @ingroup jetson-tk1_reg
 *
 * @brief Jetson-TK1 register definitions
 */

/*
 * Copyright (c) OTH Regensburg, 2016
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifndef LIBBSP_ARM_JETSONTK1_H
#define LIBBSP_ARM_JETSONTK1_H

#include <bspopts.h>
#include <stdint.h>
#include <rtems/termiostypes.h>

#define TSC_BASE 0x60005000

#define FREQUENCY_TSC 1000000L
#define FREQUENCY_GIC 12000000L

typedef struct {
  rtems_termios_device_context base;
  const char *device_name;
} jetsontk1_driver_context;

static inline void cpu_relax(void)
{
	asm volatile("" : : : "memory");
}

/**
 * @defgroup jetson-tk1_reg Register Definitions
 *
 * @ingroup arm_jetson-tk1
 *
 * @brief Register Definitions
 *
 * @{
 */

/**
 * @name Register Macros
 *
 * @{
 */

#define TEGRAK1_REG(x)           (*(volatile uint32_t *)(x))
#define TEGRAK1_BIT(n)           (1 << (n))

/** @} */

/**
 * @name Peripheral Base Register Address
 *
 * @{
 */

#define TEGRAK1_PERIPHERAL_BASE      0x01001000
#define TEGRAK1_PERIPHERAL_SIZE      0x7C00B004

#define TEGRAK1_BSP_FATAL_CONSOLE_DEVICE_INSTALL 0x55

/** @} */

#endif /* LIBBSP_ARM_JETSONTK1_H */
