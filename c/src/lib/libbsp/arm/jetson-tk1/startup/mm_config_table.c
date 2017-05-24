/*
 * Copyright (c) OTH Regensburg, 2017
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#include <bsp.h>
#include <bspopts.h>
#include <bsp/car.h>
#include <bsp/tegra124-gpio.h>
#include <bsp/console.h>
#include <bsp/timer.h>
#include <bsp/arm-cp15-start.h>

const arm_cp15_start_section_config arm_cp15_start_mmu_config_table[] = {
  ARMV7_CP15_START_DEFAULT_SECTIONS,
  {
    .begin = (uint32_t) CAR,
    .end = (uint32_t) (CAR + 0x1000),
    .flags = ARMV7_MMU_DEVICE
  }, {
    .begin = (uint32_t) TIMER_BASE,
    .end = (uint32_t) (TIMER_BASE + 0x1000),
    .flags = ARMV7_MMU_DEVICE
  }, {
    .begin = (uint32_t) GPIO_BASE,
    .end = (uint32_t) (GPIO_BASE + 0x1000),
    .flags = ARMV7_MMU_DEVICE
  }, {
    .begin = (uint32_t) PINMUX_AUX,
    .end = (uint32_t) (PINMUX_AUX + 0x1000),
    .flags = ARMV7_MMU_DEVICE
  }, {
#if NS8250_USE_SECONDARY_CONSOLE == 1
    .begin = (uint32_t) UARTA,
    .end = (uint32_t) UARTA + 0x40,
    .flags = ARMV7_MMU_DEVICE
  }, {
#endif
    .begin = (uint32_t) UARTD,
    .end = (uint32_t) UARTD + 0xff,
    .flags = ARMV7_MMU_DEVICE
  }, {
    .begin = (uint32_t) BSP_ARM_GIC_DIST_BASE,
    .end = (uint32_t) (BSP_ARM_GIC_DIST_BASE + 0x1000),
    .flags = ARMV7_MMU_DEVICE
  }, {
    .begin = (uint32_t) BSP_ARM_GIC_CPUIF_BASE,
    .end = (uint32_t) (BSP_ARM_GIC_CPUIF_BASE + 0x1000),
    .flags = ARMV7_MMU_DEVICE
  }
};

const size_t arm_cp15_start_mmu_config_table_size =
  RTEMS_ARRAY_SIZE(arm_cp15_start_mmu_config_table);
