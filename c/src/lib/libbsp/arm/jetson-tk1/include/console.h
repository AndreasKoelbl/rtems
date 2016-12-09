/*
 * Copyright (c) OTH Regensburg, 2017
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifndef LIBBSP_ARM_JETSONTK1_CONSOLE_H
#define LIBBSP_ARM_JETSONTK1_CONSOLE_H

#include <rtems/termiostypes.h>
#include <bsp/memory.h>

#define UARTA ( (void *) 0x70006000 )
#define UARTD ( (void *) 0x70006300 )

#define UART_TX 0x0
#define UART_LSR 0x14
#define  UART_LSR_THRE ( 1 << 5 )

#define UARTA_IRQ ( 36 + 32 )
#define UARTD_IRQ ( 90 + 32 )

rtems_status_code console_initialize(
  rtems_device_major_number major,
  rtems_device_minor_number minor,
  void                     *arg
);
#ifdef JAILHOUSE_ENABLE
void jailhouse_debug_console_write( const char *text );
#else
static inline void ns8250_debug_console_write( const char *text )
{
  int i;

  if ( text == NULL ) {
    return;
  }

  for ( i = 0; i < strlen( text ); i++ ) {
    while ( !( mmio_read( UART_LSR ) & UART_LSR_THRE ) ) {
      asm volatile ( "nop" );
    }

    if ( text[ i ] == '\n' ) {
      mmio_write( UARTD + UART_TX, '\r' );
    }

    mmio_write( UARTD + UART_TX, text[ i ] );
  }
}

#endif
void print_hex( uint32_t num );

#endif /* LIBBSP_ARM_JETSONTK1_CONSOLE_H */
