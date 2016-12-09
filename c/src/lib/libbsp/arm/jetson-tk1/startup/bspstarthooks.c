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

void BSP_START_TEXT_SECTION bsp_start_hook_0( void )
{
  arm_cp15_set_translation_table_base_control_register( 0x0 );
#if JAILHOUSE_ENABLE == 1
  arm_cp15_set_vector_base_address( (void *) 0x00000000 );
#else
  arm_cp15_set_vector_base_address( (void *) 0x90000000 );
#endif
}

void BSP_START_TEXT_SECTION bsp_start_hook_1( void )
{
  bsp_start_copy_sections();
  bsp_start_clear_bss();
}
