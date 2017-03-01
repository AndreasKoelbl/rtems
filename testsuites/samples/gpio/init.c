/*
 *  COPYRIGHT (c) 1989-2012.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/printer.h>
#include <rtems/test.h>
#include <rtems/irq-extension.h>

#include <bsp.h> /* for device driver prototypes */
#include <bsp/gpio.h>

#include <stdio.h>
#include <stdlib.h>

/* Soft error handling */
#undef rtems_directive_failed
#undef posix_directive_failed

#define rtems_directive_failed(status, msg) \
  if (status != RTEMS_SUCCESSFUL) { \
    printf("\n%s FAILED -- expected (%s) got (%s)\n", msg, \
           rtems_status_text(RTEMS_SUCCESSFUL), rtems_status_text(status)); \
    fflush(stdout); \
  }

#define posix_directive_failed(status, msg) \
    if (status) { \
    perror(#msg); \
  }

rtems_task Init(rtems_task_argument argument);
void irq_handler(void *arg);

rtems_task Init(rtems_task_argument ignored)
{
  rtems_status_code status;
  rtems_vector_number vector;

  vector = rtems_gpio_bsp_get_vector(5);
  status = rtems_interrupt_handler_install(
    vector,
    "GPIO6",
    RTEMS_INTERRUPT_UNIQUE,
    irq_handler,
    (char*) vector/* TODO: transmit context */
  );
  rtems_directive_failed(status, "interrupt_handler_install");

  status = rtems_gpio_bsp_select_output(5, 0, NULL);
  status = rtems_gpio_bsp_enable_interrupt(5, 0, vector);
  status = rtems_gpio_bsp_set(5, 0);
  while (true) {
    asm volatile("" ::: "memory");
  }
}

void irq_handler(void *arg)
{
  printf("irq received\n");
  rtems_gpio_bsp_interrupt_line((rtems_vector_number) arg);
}

/* NOTICE: the clock driver is explicitly disabled */
#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_MAXIMUM_TASKS            1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
