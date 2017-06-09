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

void BSP_START_TEXT_SECTION bsp_start_hook_0( void )
{
}

void BSP_START_TEXT_SECTION bsp_start_hook_1( void )
{
  uint32_t sctlr;

  sctlr = arm_cp15_get_control();

  /*
   * Current U-boot loader seems to start kernel image
   * with I and D caches on and MMU enabled.
   * If RTEMS application image finds that cache is on
   * during startup then disable caches.
   */
  if ( sctlr & (ARM_CP15_CTRL_I | ARM_CP15_CTRL_C | ARM_CP15_CTRL_M | ARM_CP15_CTRL_A ) ) {
    if ( sctlr & (ARM_CP15_CTRL_C | ARM_CP15_CTRL_M ) ) {
      /*
       * If the data cache is on then ensure that it is clean
       * before switching off to be extra carefull.
       */
      arm_cp15_data_cache_clean_all_levels();
    }
    arm_cp15_flush_prefetch_buffer();
    sctlr &= ~ ( ARM_CP15_CTRL_I | ARM_CP15_CTRL_C | ARM_CP15_CTRL_M | ARM_CP15_CTRL_A );
    arm_cp15_set_control( sctlr );
  }

  arm_cp15_instruction_cache_invalidate();
  /*
   * The care should be taken there that no shared levels
   * are invalidated by secondary CPUs in SMP case.
   * It is not problem on Zynq because level of coherency
   * is L1 only and higher level is not maintained and seen
   * by CP15. So no special care to limit levels on the secondary
   * are required there.
   */
  arm_cp15_data_cache_invalidate_all_levels();
  arm_cp15_branch_predictor_invalidate_all();
  arm_cp15_tlb_invalidate();
  arm_cp15_flush_prefetch_buffer();

//  printk("Before init\n");
//  arm_a9mpcore_start_hook_1();
  bsp_start_copy_sections();
//  zynq_setup_mmu_and_cache();

#if !defined(RTEMS_SMP) \
  && (defined(BSP_DATA_CACHE_ENABLED) \
    || defined(BSP_INSTRUCTION_CACHE_ENABLED))
  /* Enable unified L2 cache */
  rtems_cache_enable_data();
#endif

  sctlr = arm_cp15_get_control();
  bsp_start_clear_bss();
  sctlr |= ARM_CP15_CTRL_I | ARM_CP15_CTRL_C | ARM_CP15_CTRL_M | ARM_CP15_CTRL_A;
  arm_cp15_set_control( sctlr );
}
