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
#include <bsp/jetson-tk1.h>
#include <rtems/bspIo.h>
#include <rtems/sysinit.h>

#define JAILHOUSE_HC_DEBUG_CONSOLE_PUTC 8

void hypervisor_putc(char c)
{
	register uint32_t num_res asm("r0") = JAILHOUSE_HC_DEBUG_CONSOLE_PUTC;
	register uint32_t arg1 asm("r1") = c;

	asm volatile(
		".arch_extension virt\n\t"
		"hvc #0x4a48\n\t"
		: "=r" (num_res)
		: "r" (num_res), "r" (arg1)
		: "memory");
}

void jailhouse_dbgcon_write(
  rtems_termios_device_context *base,
  const char                   *buf,
  size_t                        len
)
{
  size_t i;
  jetsontk1_driver_context *ctx = (jetsontk1_driver_context *) base;

	for (i = 0; i < len; i++) {
		hypervisor_putc(buf[i]);
	}
}

static void jetsontk1_debug_console_out(char c)
{
  /* Do we need this?
  if (c == '\n') {
    DBG_PutChar('\r');
  }
  */

  hypervisor_putc(c);
}

static void __attribute__((aligned(32))) jetsontk1_debug_console_init(void)
{
  //DBG_Configure(115200, BOARD_MCK);
  BSP_output_char = jetsontk1_debug_console_out;
  hypervisor_putc('X');
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
  RTEMS_SYSINIT_ORDER_FIRST
);
/*
 *
  #define RTEMS_STRING( _x ) #_x
  #define RTEMS_XSTRING( _x ) RTEMS_STRING( _x )
  #define _RTEMS_SYSINIT_ITEM( handler, module, order ) \
    _RTEMS_SYSINIT_INDEX_ITEM( handler, 0x##module##order )
  #define RTEMS_USED __attribute__((__used__))
  #define RTEMS_SECTION( _section ) __attribute__((aligned(32))) __attribute__((__section__(_section)))

  #define RTEMS_LINKER_ROSET_ITEM_ORDERED( set, type, item, order ) \
    type const _Linker_set_##set##_##item \
    RTEMS_SECTION( ".rtemsroset." #set ".content.0." RTEMS_XSTRING( order ) ) \
    RTEMS_USED

  #define _RTEMS_SYSINIT_INDEX_ITEM( handler, index ) \
  enum { _Sysinit_##handler = index }; \
  RTEMS_LINKER_ROSET_ITEM_ORDERED( \
    _Sysinit, \
    rtems_sysinit_item, \
    handler, \
    index \
  ) = { handler }
  #define _RTEMS_SYSINIT_ITEM( handler, module, order ) \
    _RTEMS_SYSINIT_INDEX_ITEM( handler, 0x##module##order )
  #define RTEMS_SYSINIT_ITEM( handler, module, order ) \
    _RTEMS_SYSINIT_ITEM( handler, module, order )
  ---------------------------------------------------------

RTEMS_SYSINIT_ITEM(jetsontk1_debug_console_init, RTEMS_SYSINIT_BSP_START, RTEMS_SYSINIT_ORDER_LAST);
_RTEMS_SYSINIT_INDEX_ITEM( handler, 0x##module##order )

enum { _Sysinit_jetsontk1_debug_console_init = 0x000200ff }; 
rtems_sysinit_item const _Linker_set__Sysinit_jetsontk1_debug_console_init __attribute__((aligned(32))) __attribute__((__section__(".rtemsroset." "_Sysinit" ".content.0." "0x000200ff"))) __attribute__((__used__)) = { jetsontk1_debug_console_init }
*/
