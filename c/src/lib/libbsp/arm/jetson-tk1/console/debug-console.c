/**
 * @file
 *
 * @ingroup arm_jetson-tk1
 *
 * @brief Jetson-TK1 console driver
 *
 */

/*
 * Copyright (c) OTH Regensburg, 2016
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#include <bsp.h>
#include <console.h>
#include <rtems/bspIo.h>
#include <rtems/sysinit.h>

static void jetsontk1_debug_console_out(char c)
{
  /* Do we need this?
  if (c == '\n') {
    DBG_PutChar('\r');
  }
  */

  hypervisor_putc(c);
}

static void jetsontk1_debug_console_init(void)
{
  //DBG_Configure(115200, BOARD_MCK);
  BSP_output_char = jetsontk1_debug_console_out;
}

static void jetsontk1_debug_console_early_init(char c)
{
  jetsontk1_debug_console_init();
  jetsontk1_debug_console_out(c);
}

static int jetsontk1_debug_console_in(void)
{
  /* we do not read yet... */
  return (int)'Y';
}

BSP_output_char_function_type BSP_output_char = jetsontk1_debug_console_early_init;

BSP_polling_getchar_function_type BSP_poll_char = jetsontk1_debug_console_in;

RTEMS_SYSINIT_ITEM(
  jetsontk1_debug_console_init,
  RTEMS_SYSINIT_BSP_START,
  RTEMS_SYSINIT_ORDER_LAST
);
