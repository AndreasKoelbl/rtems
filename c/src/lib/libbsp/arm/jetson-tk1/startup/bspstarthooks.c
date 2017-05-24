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
#include <bsp/bootcard.h>

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
}

void BSP_START_TEXT_SECTION bsp_start_hook_1( void )
{
  bsp_start_copy_sections();
  bsp_start_clear_bss();
}
