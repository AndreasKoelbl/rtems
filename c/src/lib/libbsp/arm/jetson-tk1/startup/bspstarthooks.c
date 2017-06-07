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
#include <bsp/console.h>
#include <rtems.h>

static void foo(void)
{
  uint32_t ctrl = arm_cp15_get_control();

  printk("MMU is %s enabled!\n", (ctrl & ARM_CP15_CTRL_M) ? "" : "not");
  printk("C is %s enabled!\n", (ctrl & ARM_CP15_CTRL_C) ? "" : "not");
  printk("I is %s enabled!\n", (ctrl & ARM_CP15_CTRL_I) ? "" : "not");
  printk("V is %s enabled!\n", (ctrl & ARM_CP15_CTRL_V) ? "" : "not");
}

static void cp_delay (void)
{
	volatile int i;

	/* copro seems to need some delay between reading and writing */
	for (i = 0; i < 100; i++) {
		asm volatile ("nop");
  }
	asm volatile("" : : : "memory");
}

void BSP_START_TEXT_SECTION bsp_start_hook_0( void )
{
  uint32_t ctrl;

  arm_cp15_set_vector_base_address(bsp_vector_table_begin);

  ctrl = arm_cp15_get_control();
  ctrl &= ~ARM_CP15_CTRL_V;
  arm_cp15_set_control(ctrl);

  printk("Before rpi\n");
  ctrl = arm_cp15_get_control();

  if (ctrl & (ARM_CP15_CTRL_I | ARM_CP15_CTRL_C | ARM_CP15_CTRL_M | ARM_CP15_CTRL_A)) {
    if (ctrl & (ARM_CP15_CTRL_C | ARM_CP15_CTRL_M)) {
      rtems_cache_flush_entire_data();
      rtems_cache_invalidate_entire_data();
      /*
      arm_cp15_data_cache_clean_all_levels();
      arm_cp15_data_cache_invalidate_all_levels();
      arm_cp15_instruction_cache_invalidate();
      arm_cp15_flush_prefetch_buffer();
      */
    }
    //arm_cp15_tlb_invalidate();
    arm_cp15_flush_prefetch_buffer();
    ctrl &= ~(ARM_CP15_CTRL_I | ARM_CP15_CTRL_C | ARM_CP15_CTRL_M | ARM_CP15_CTRL_A);
    arm_cp15_set_control(ctrl);
    rtems_cache_invalidate_entire_data();

    rtems_cache_invalidate_entire_instruction();
    arm_cp15_branch_predictor_invalidate_all();
    arm_cp15_tlb_invalidate();
    arm_cp15_flush_prefetch_buffer();

    arm_cp15_set_translation_table_base_control_register(0);
  }
}

void BSP_START_TEXT_SECTION bsp_start_hook_1( void )
{
  printk("Before init\n");
  bsp_start_copy_sections();
  /* Access is to ttb is too far, > 4kB -> not turning mmu off */
  bsp_memory_management_initialize();
  bsp_start_clear_bss();
}
