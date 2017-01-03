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

#define UART0  (0x70006300)
/* FIXME: wrong IRQ number, this is the timer */
#define UART0_IRQn 27

#define UART_TX			      0x0
#define UART_DLL          0x0
#define UART_RBR          0x0
#define UART_DLM          0x4
#define UART_LCR          0xc
#define  UART_LCR_8N1     0x03
#define  UART_LCR_DLAB    0x80
#define UART_LSR          0x14
#define  UART_LSR_THRE    0x20


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
void printHex(uint32_t num);
void writeChar(char out);
void myItoa(uint32_t num, char* str, uint32_t base);

#endif
