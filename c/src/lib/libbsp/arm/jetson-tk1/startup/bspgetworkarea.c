/**
 * @file
 *
 * @ingroup arm_start
 *
 * @brief Raspberry pi workarea initialization.
 */

/*
 * COPYRIGHT (c) 1989-2008.
 * On-Line Applications Research Corporation (OAR).
 *
 * Copyright (c) 2011-2012 embedded brains GmbH.
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 *
 * Copyright (c) 2015 YANG Qiao
 *
 * Code is based on c/src/lib/libbsp/shared/bspgetworkarea.c
 */

#include <string.h>
#include <bsp.h>
#include <bsp/bootcard.h>
#ifdef BSP_INTERRUPT_STACK_AT_WORK_AREA_BEGIN
  #include <rtems/config.h>
#endif

#if defined(HAS_UBOOT) && !defined(BSP_DISABLE_UBOOT_WORK_AREA_CONFIG)
  #define USE_UBOOT
#endif

/*
 *  These are provided by the linkcmds for ALL of the BSPs which use this file.
 */
extern char WorkAreaBase[];

/*
 *  We may get the size information from U-Boot or the linker scripts.
 */
#ifdef USE_UBOOT
  #include <bsp/u-boot.h>
#else
  extern char RamBase[];
  extern char RamSize[];
#endif

void bsp_work_area_initialize(void)
{
  uintptr_t work_base = (uintptr_t) WorkAreaBase;
  uintptr_t ram_end;

  #ifdef USE_UBOOT
    ram_end = (uintptr_t) bsp_uboot_board_info.bi_memstart +
                          bsp_uboot_board_info.bi_memsize;
  #else
    ram_end = (uintptr_t)RamBase + (uintptr_t)RamSize;
  #endif

  #ifdef BSP_INTERRUPT_STACK_AT_WORK_AREA_BEGIN
    work_base += rtems_configuration_get_interrupt_stack_size();
  #endif

  bsp_work_area_initialize_default( (void *) work_base, ram_end - work_base );
}
