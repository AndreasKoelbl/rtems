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

#ifndef LIBBSP_ARM_JETSONTK1_CONSOLE_H
#define LIBBSP_ARM_JETSONTK1_CONSOLE_H

#include <rtems/termiostypes.h>
#include <bsp/jetson-tk1.h>

#define UART0  (0x70006300)
#define JETSONTK1_CONSOLE_USE_INTERRUPTS 1

#define UART_TX			      0x0 // Also known as THR
#define UART_DLL          0x0
#define UART_RBR          0x0 //Receive Buffer/Holding Register
#define UART_DLM          0x4
#define UART_IER          0x4
#define UART_LCR          0xc
#define  UART_LCR_8N1     0x03
#define  UART_LCR_DLAB    (1 << 7)
#define UART_LSR          0x14

#define  UART_LSR_RDR     _BV(0)
/* TODO: Check if errors have to be checked */
#define UART_LSR_RECEIVER_OVERRUN_ERROR _BV(1)
#define UART_LSR_PARITY_ERROR           _BV(2)
#define UART_LSR_FRAMING_ERROR          _BV(3)
#define UART_LSR_BREAK_CONDITION        _BV(4)
#define UART_LSR_THRE                   _BV(5)

void jetsontk1_driver_write(
  rtems_termios_device_context *base,
  const char                   *buf,
  size_t                        len
);
rtems_status_code console_initialize(
  rtems_device_major_number  major,
  rtems_device_minor_number  minor,
  void                      *arg
);
void print_hex(uint32_t num);

#endif /* LIBBSP_ARM_JETSONTK1_CONSOLE_H */
