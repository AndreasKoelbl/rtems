/*
 * Copyright (c) OTH Regensburg, 2017
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#include <bspopts.h>
#include <bsp/start.h>
#include <bsp/mm.h>
#include <bsp.h>
#include <libcpu/arm-cp15.h>
#include <bsp/arm-cp15-start.h>

static void BSP_START_TEXT_SECTION setup_mmu_and_cache(void)
{
  uint32_t ctrl = arm_cp15_start_setup_mmu_and_cache(
    ARM_CP15_CTRL_A | ARM_CP15_CTRL_M,
    ARM_CP15_CTRL_AFE | ARM_CP15_CTRL_Z
  );

  arm_cp15_start_setup_translation_table_and_enable_mmu_and_cache(
    ctrl,
    (uint32_t *) bsp_translation_table_base,
    ARM_MMU_DEFAULT_CLIENT_DOMAIN,
    &arm_cp15_start_mmu_config_table[0],
    arm_cp15_start_mmu_config_table_size
  );
}

void BSP_START_TEXT_SECTION bsp_start_hook_0( void )
{
  uint32_t ctrl;

  /*
   * Do not use bsp_vector_table_begin == 0, since this will get optimized away.
   */
  if (bsp_vector_table_end != bsp_vector_table_size) {
    arm_cp15_set_vector_base_address(bsp_vector_table_begin);

    ctrl = arm_cp15_get_control();
    ctrl &= ~ARM_CP15_CTRL_V;
    arm_cp15_set_control(ctrl);
  }
  printk("before invalidate\n");
  arm_cp15_instruction_cache_invalidate();
  arm_cp15_data_cache_invalidate_all_levels();
  printk("after invalidate\n");
}

void BSP_START_TEXT_SECTION bsp_start_hook_1( void )
{
  bsp_start_copy_sections();
  printk("before mmu\n");
  setup_mmu_and_cache();
  printk("after mmu\n");
#ifndef RTEMS_SMP
  /* Enable unified L2 cache */
  rtems_cache_enable_data();
#endif
  bsp_start_clear_bss();

}
