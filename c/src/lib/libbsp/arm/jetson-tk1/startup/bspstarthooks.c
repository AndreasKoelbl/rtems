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
#include <bsp/cache_.h>
#include <rtems.h>

static void foo(void)
{
  uint32_t ctrl = arm_cp15_get_control();

  printk("MMU is %s enabled!\n", (ctrl & ARM_CP15_CTRL_M) ? "" : "not");
  printk("C is %s enabled!\n", (ctrl & ARM_CP15_CTRL_C) ? "" : "not");
  printk("I is %s enabled!\n", (ctrl & ARM_CP15_CTRL_I) ? "" : "not");
  printk("V is %s enabled!\n", (ctrl & ARM_CP15_CTRL_V) ? "" : "not");
}

/* Invalidate TLB */
static void v7_inval_tlb(void)
{
	/* Invalidate entire unified TLB */
	asm volatile ("mcr p15, 0, %0, c8, c7, 0" : : "r" (0));
	/* Invalidate entire data TLB */
	asm volatile ("mcr p15, 0, %0, c8, c6, 0" : : "r" (0));
	/* Invalidate entire instruction TLB */
	asm volatile ("mcr p15, 0, %0, c8, c5, 0" : : "r" (0));
	/* Full system DSB - make sure that the invalidation is complete */
	dsb();
	/* Full system ISB - make sure the instruction stream sees it */
	isb();
}

static void inline config_cache()
{
  uint32_t reg = 0;
	/*
	 * Systems with an architectural L2 cache must not use the PL310.
	 * Config L2CTLR here for a data RAM latency of 3 cycles.
	 */
	asm("mrc p15, 1, %0, c9, c0, 2" : : "r" (reg));
	reg &= ~7;
	reg |= 2;
	asm("mcr p15, 1, %0, c9, c0, 2" : : "r" (reg));
}

static void inline invalidate_dcache_all(void) {
  /* No idea, what happens in this linux asm routine */
	//v7_invalidate_dcache_all(); //ASM, TODO
  _CPU_cache_flush_entire_data();
}

static void inline arm_init_before_mmu(void)
{
  config_cache();
	//v7_outer_cache_enable(); not implemented
	invalidate_dcache_all();
	v7_inval_tlb();
}

void BSP_START_TEXT_SECTION bsp_start_hook_0( void )
{
}

void BSP_START_TEXT_SECTION bsp_start_hook_1( void )
{
  uint32_t sctlr;

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

  bsp_start_copy_sections();
  bsp_start_clear_bss();
  arm_init_before_mmu();

  bsp_memory_management_initialize();
  sctlr = arm_cp15_get_control();
  /* Enable Cache, MMU, Alignment check, Branch predictor and Access Flag */
  sctlr |= ARM_CP15_CTRL_I | ARM_CP15_CTRL_C | ARM_CP15_CTRL_M | ARM_CP15_CTRL_A
        | ARM_CP15_CTRL_Z | ARM_CP15_CTRL_AFE;
  arm_cp15_set_control( sctlr );

}

#if 0
void old_code( void ) {
  uint32_t sctlr;

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
    rtems_cache_invalidate_entire_data();
    arm_cp15_instruction_cache_invalidate();
    sctlr &= ~ ( ARM_CP15_CTRL_I | ARM_CP15_CTRL_C | ARM_CP15_CTRL_M | ARM_CP15_CTRL_A );
    arm_cp15_set_control( sctlr );
  }

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

  arm_cp15_set_translation_table_base_control_register(0);
  bsp_start_copy_sections();
  bsp_memory_management_initialize();

  sctlr = arm_cp15_get_control();
  /* Enable Cache, MMU, Alignment check, Branch predictor and Access Flag */
  sctlr |= ARM_CP15_CTRL_I | ARM_CP15_CTRL_C | ARM_CP15_CTRL_M | ARM_CP15_CTRL_A
        | ARM_CP15_CTRL_Z | ARM_CP15_CTRL_AFE;
  arm_cp15_set_control( sctlr );
  foo();
  printk("Hello\n");
}
#endif
