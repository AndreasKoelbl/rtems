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

#include <rtems/irq-extension.h>

#include <bsp.h> /* for device driver prototypes */
#include <bsp/memory.h>
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

#define GPIO_INT_ENB 0x50
#define GPIO_BASE    0x70003000

rtems_task Init(rtems_task_argument argument);
void irq_handler(void *arg);

rtems_task Init(rtems_task_argument ignored)
{
  rtems_status_code status;
  rtems_vector_number vector;

  vector = rtems_gpio_bsp_get_vector(6);
  rtems_gpio_bsp_clear(6, 6);
  rtems_gpio_bsp_disable_interrupt(6, 6, RISING_EDGE);

  status = rtems_interrupt_handler_install(
    vector,
    "GPIO6",
    RTEMS_INTERRUPT_UNIQUE,
    irq_handler,
    (char*) vector
  );
  printf("after installing\n");
  status = rtems_gpio_bsp_select_output(6, 6, NULL);
  rtems_directive_failed(status, "select output");
//  mmio_write32(GPIO_BASE + (6 - 1) * 0x100 + GPIO_INT_ENB, 1 << 4);
  status = rtems_gpio_bsp_select_input(6, 4, NULL);
  rtems_directive_failed(status, "select input");
  status = rtems_gpio_bsp_enable_interrupt(6, 4, RISING_EDGE);
  rtems_directive_failed(status, "enable interrupt");

  status = rtems_gpio_bsp_set(6, 6);
  rtems_directive_failed(status, "gpio set");
  rtems_task_wake_after(50);
  rtems_gpio_bsp_clear(6, 6);
  printf("entering endless loop\n");

  while (true) {
    asm volatile("" ::: "memory");
  }
}

void irq_handler(void *arg)
{
  rtems_gpio_bsp_clear(6, 6);
  printf("irqn: %u\n", (rtems_vector_number) arg);
}

/* NOTICE: the clock driver is explicitly disabled */
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_MICROSECONDS_PER_TICK 100000

#define CONFIGURE_MAXIMUM_TASKS            1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
