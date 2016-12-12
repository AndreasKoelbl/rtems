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

static void hypervisor_putc(char c);
static void jailhouse_dbgcon_write(
  rtems_termios_device_context *base,
  const char                   *buf,
  size_t                        len
);

static bool jetsontk1_driver_set_attributes(
  rtems_termios_device_context  *base,
  const struct termios          *term
);

static bool jetsontk1_driver_first_open(
  rtems_termios_tty             *tty,
  rtems_termios_device_context  *base,
  struct termios                *term,
  rtems_libio_open_close_args_t *args
);

static void jetsontk1_driver_last_close(
  rtems_termios_tty             *tty,
  rtems_termios_device_context  *base,
  rtems_libio_open_close_args_t *args
);

static int jetsontk1_driver_poll_read(rtems_termios_device_context *base);

rtems_status_code console_initialize(
  rtems_device_major_number  major,
  rtems_device_minor_number  minor,
  void                      *arg
);

#endif /* LIBBSP_ARM_JETSONTK1_CONSOLE_H */
