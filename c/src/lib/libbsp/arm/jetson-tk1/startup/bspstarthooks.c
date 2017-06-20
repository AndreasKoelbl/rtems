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
#include <rtems/bspIo.h>

static void foo(void)
{
  uint32_t ctrl = arm_cp15_get_control();

  printk("MMU is%s enabled!\n", (ctrl & ARM_CP15_CTRL_M) ? "" : " not");
  printk("C is%s enabled!\n", (ctrl & ARM_CP15_CTRL_C) ? "" : " not");
  printk("I is%s enabled!\n", (ctrl & ARM_CP15_CTRL_I) ? "" : " not");
  printk("V is%s enabled!\n", (ctrl & ARM_CP15_CTRL_V) ? "" : " not");
}

void BSP_START_TEXT_SECTION bsp_start_hook_0( void )
{
}

void BSP_START_TEXT_SECTION bsp_start_hook_1( void )
{
  uint32_t sctlr;
  uint32_t i;
  /*
   * Do not use bsp_vector_table_begin == 0, since this will get optimized away.
  */
  if (bsp_vector_table_end != bsp_vector_table_size) {

    /*
     * For now we assume that every Cortex-A9 MPCore has the Security Extensions.
     * Later it might be necessary to evaluate the ID_PFR1 register.
     */
    arm_cp15_set_vector_base_address(bsp_vector_table_begin);

    sctlr = arm_cp15_get_control();
    sctlr &= ~ARM_CP15_CTRL_V;
    arm_cp15_set_control(sctlr);
  }

  //printk("Hello\n");

  /*
   * Current U-boot loader seems to start kernel image
   * with I and D caches on and MMU enabled.
   * If RTEMS application image finds that cache is on
   * during startup then disable caches.
   */
  if ( sctlr & (ARM_CP15_CTRL_I | ARM_CP15_CTRL_C | ARM_CP15_CTRL_M |
                ARM_CP15_CTRL_A | ARM_CP15_CTRL_Z ) ) {
    if ( sctlr & (ARM_CP15_CTRL_C | ARM_CP15_CTRL_M ) ) {
      /*
       * If the data cache is on then ensure that it is clean
       * before switching off to be extra carefull.
       */
      arm_cp15_data_cache_clean_all_levels();
    }
    sctlr &= ~ ( ARM_CP15_CTRL_I | ARM_CP15_CTRL_C | ARM_CP15_CTRL_M |
                 ARM_CP15_CTRL_A | ARM_CP15_CTRL_Z);

    rtems_cache_invalidate_entire_data();
    arm_cp15_instruction_cache_invalidate();
    arm_cp15_data_cache_invalidate_all_levels();
    arm_cp15_branch_predictor_invalidate_all();
    arm_cp15_tlb_invalidate();
    arm_cp15_flush_prefetch_buffer();

    arm_cp15_set_control( sctlr );
  }

#if 0
  /*
   * The care should be taken there that no shared levels
   * are invalidated by secondary CPUs in SMP case.
   * It is not problem on Zynq because level of coherency
   * is L1 only and higher level is not maintained and seen
   * by CP15. So no special care to limit levels on the secondary
   * are required there.
   */
  /* Move them above */

  printk("hello\n");
  printk("Entering sections: \n");
  for (i = 0; i < arm_cp15_start_mmu_config_table_size; i++) {
    printk(
        "%lX - %lX with %lX\n",
        arm_cp15_start_mmu_config_table[i].begin,
        arm_cp15_start_mmu_config_table[i].end,
        arm_cp15_start_mmu_config_table[i].flags
    );
  }
#endif
  bsp_start_copy_sections();
//  arm_cp15_set_translation_table_base_control_register(0);
//  bsp_memory_management_initialize();
  bsp_start_clear_bss();
}
