/*
 *  COPYRIGHT (c) 1989-2011, 2017.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#include <bsp.h>
#include <bsp/console.h>
#include <bsp/memory.h>
#include <rtems/bspIo.h>

#define JAILHOUSE_HC_DEBUG_CONSOLE_PUTC 8
#define UART_LSR 0x14
#define  UART_LSR_RDR ( 1 << 0 )

#define UART_RBR ( 0 << 0 )

#if JAILHOUSE_ENABLE == 1
static void jailhouse_debug_console_out( char c )
{
  register uint32_t num_res asm ( "r0" ) = JAILHOUSE_HC_DEBUG_CONSOLE_PUTC;
  register uint32_t arg1 asm ( "r1" ) = c;

  asm volatile(
    ".arch_extension virt\n\t"
    "hvc #0x4a48\n\t"
    : "=r" ( num_res )
    : "r" ( num_res ), "r" ( arg1 )
    : "memory" );
}

void jailhouse_debug_console_write( const char *text )
{
  if ( text == NULL ) {
    return;
  }

  do {
    jailhouse_debug_console_out( *text );
  } while ( *text++ );
}
#else
static void ns8250_debug_console_out( char c )
{
  while ( !( mmio_read( UARTD + UART_LSR ) & UART_LSR_THRE ) ) {
    asm volatile( "nop" );
  }

  if ( c == '\n' ) {
    mmio_write( UARTD + UART_TX, '\r' );
  }

  if ( ( c >= 'a' ) && ( c <= 'z' ) ) {
    c -= 0x20;
  }

  mmio_write( UARTD + UART_TX, c );
}
#endif

static int ns8250_debug_console_in( void )
{
  int result;

  while ( !( mmio_read( UARTD + UART_LSR ) & UART_LSR_RDR ) ) {
    asm volatile ( "nop" );
  }

  /* Read from UARTD */
  result = mmio_read( UARTD + UART_RBR );
  /* Clear interrupts */
  mmio_write( UARTD + UART_LSR, 0 );

  return result;
}

#if JAILHOUSE_ENABLE == 1
BSP_output_char_function_type BSP_output_char = jailhouse_debug_console_out;
#else
BSP_output_char_function_type BSP_output_char = ns8250_debug_console_out;
#endif
BSP_polling_getchar_function_type BSP_poll_char = ns8250_debug_console_in;
