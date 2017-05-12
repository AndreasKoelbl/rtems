/*
 * Copyright (c) OTH Regensburg, 2017
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#include <bsp.h>
#include <bsp/car.h>
#include <bsp/tegra124-gpio.h>
#include <bsp/console.h>
#include <bsp/timer.h>
#include <bsp/arm-cp15-start.h>

/*
 * Pagetable initialization data
 *
 * Keep all read-only sections before read-write ones.
 * This ensures that write is allowed if one page/region
 * is partially filled by read-only section content
 * and rest is used for writeable section
 */

const arm_cp15_start_section_config arm_cp15_start_mmu_config_table[] = {
  {
    .begin = (uint32_t) bsp_section_fast_text_begin,
    .end = (uint32_t) bsp_section_fast_text_end,
    .flags = ARMV7_MMU_CODE_CACHED
  }, {
    .begin = (uint32_t) bsp_section_start_begin,
    .end = (uint32_t) bsp_section_start_end,
    .flags = ARMV7_MMU_CODE_CACHED
  }, {
    .begin = (uint32_t) bsp_section_text_begin,
    .end = (uint32_t) bsp_section_text_end,
    .flags = ARMV7_MMU_CODE_CACHED
  }, {
    .begin = (uint32_t) bsp_section_rodata_begin,
    .end = (uint32_t) bsp_section_rodata_end,
    .flags = ARMV7_MMU_DATA_READ_ONLY_CACHED
  }, {
    .begin = (uint32_t) bsp_translation_table_base,
    .end = (uint32_t) bsp_translation_table_base + 0x4000,
    .flags = ARMV7_MMU_DATA_READ_WRITE_CACHED
  }, {
    .begin = (uint32_t) bsp_section_fast_data_begin,
    .end = (uint32_t) bsp_section_fast_data_end,
    .flags = ARMV7_MMU_DATA_READ_WRITE_CACHED
  }, {
    .begin = (uint32_t) bsp_section_vector_begin,
    .end = (uint32_t) bsp_section_vector_end,
    .flags = ARMV7_MMU_DATA_READ_WRITE_CACHED
  }, {
    .begin = (uint32_t) bsp_section_data_begin,
    .end = (uint32_t) bsp_section_data_end,
    .flags = ARMV7_MMU_DATA_READ_WRITE_CACHED
  }, {
    .begin = (uint32_t) bsp_section_bss_begin,
    .end = (uint32_t) bsp_section_bss_end,
    .flags = ARMV7_MMU_DATA_READ_WRITE_CACHED
  }, {
    .begin = (uint32_t) bsp_section_work_begin,
    .end = (uint32_t) bsp_section_work_end,
    .flags = ARMV7_MMU_DATA_READ_WRITE_CACHED
  }, {
    .begin = (uint32_t) bsp_section_stack_begin,
    .end = (uint32_t) bsp_section_stack_end,
    .flags = ARMV7_MMU_DATA_READ_WRITE_CACHED
  }, {
#if JAILHOUSE_ENABLE == 0
    .begin = (uint32_t) CAR,
    .end = (uint32_t) (CAR + 0x1000),
    .flags = ARMV7_MMU_DEVICE
  }, {
#endif
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
    .begin = (uint32_t) UARTA,
    .end = (uint32_t) UARTA + 0x40,
    .flags = ARMV7_MMU_DEVICE
  }, {
    .begin = (uint32_t) UARTD,
    .end = (uint32_t) UARTD + 0x80,
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
