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

#define UARTA  ((void*) 0x70006000)
#define UARTD  ((void*) 0x70006300)
#define JETSONTK1_CONSOLE_USE_INTERRUPTS 1

rtems_status_code console_initialize(
  rtems_device_major_number  major,
  rtems_device_minor_number  minor,
  void                      *arg
);
void jailhouse_debug_console_write(char *text);
void print_hex(uint32_t num);

#endif /* LIBBSP_ARM_JETSONTK1_CONSOLE_H */
