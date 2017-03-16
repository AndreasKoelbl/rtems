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
#define PU0 0x184
#define PU1 0x188
#define PU2 0x18c
#define PU3 0x190
#define PU4 0x194

#define PU5 0x198
#define PU6 0x19c

#define PH1 0x214

#define GPIO_CNF      0x00
#define GPIO_OE       0x10
#define GPIO_OUT      0x20
#define GPIO_IN       0x30
#define GPIO_INT_STA  0x40
#define GPIO_INT_ENB  0x50
#define GPIO_INT_LVL  0x60
#define GPIO_INT_CLR  0x70

#define MUX_LOCK             (1 << 7)
#define MUX_E_INPUT          (1 << 5)
#define MUX_TRISTATE         (1 << 4)
#define MUX_RVSD             ((1 << 3)|(1 << 2))
#define MUX_PULL_UP          ((1 << 3)|(0 << 2))
#define MUX_PULL_DOWN        ((0 << 3)|(1 << 2))
#define MUX_PULL_NORMAL      ((0 << 3)|(0 << 2))
#define MUX_GMI_DTV          ((1 << 1)|(1 << 0))
#define MUX_GMI_GMI          ((1 << 1)|(0 << 0))
#define MUX_GMI_TRACE        ((0 << 1)|(0 << 0))
#define MUX_GMI_PWM          ((0 << 1)|(0 << 0))


#define GPIO_BASE   ((void*)0x6000d000)
#define PINMUX_AUX  ((void*)0x70003000)
//#define DEBUG
#define bank 6

rtems_task Init(rtems_task_argument argument);
void irq_handler(void *arg);

rtems_task Init(rtems_task_argument ignored)
{
  rtems_status_code status;
  rtems_vector_number vector;

  printf("I am ALIVE\n");

  vector = rtems_gpio_bsp_get_vector(6);
  /* Clear all interrupts */
  mmio_write32(GPIO_BASE + (6 - 1) * 0x100 + GPIO_INT_CLR, 0xff);
  /* Disable all interrupts */
  mmio_write8(GPIO_BASE + (bank - 1) * 0x100 + GPIO_INT_ENB, 0);
  status = rtems_interrupt_handler_install(
    vector,
    "GPIO6",
    RTEMS_INTERRUPT_UNIQUE,
    irq_handler,
    (char*) vector
  );

  status = rtems_gpio_bsp_select_output(6, 6, NULL);
  rtems_directive_failed(status, "select output");
  status = rtems_gpio_bsp_select_input(6, 5, NULL);
  rtems_directive_failed(status, "select input");
  status = rtems_gpio_bsp_select_input(6, 4, NULL);
  rtems_directive_failed(status, "select input");
  status = rtems_gpio_bsp_enable_interrupt(6, 5, FALLING_EDGE);
  rtems_directive_failed(status, "enable interrupt");
  status = rtems_gpio_bsp_set(6, 6);
  rtems_directive_failed(status, "set");

  while (true) {
    asm volatile("" : : : "memory");
  }
  rtems_task_delete(rtems_task_self());
}

void irq_handler(void *arg)
{
  uint32_t pin_value;
  mmio_write32(GPIO_BASE + (6 - 1) * 0x100 + GPIO_OUT, 0);
  do {
    pin_value = rtems_gpio_bsp_get_value(6, 4);
  } while (!pin_value);
  mmio_write32(GPIO_BASE + (6 - 1) * 0x100 + GPIO_INT_CLR, 0xff);
  mmio_write32(GPIO_BASE + (6 - 1) * 0x100 + GPIO_OUT, 1 << 6);
}

/* NOTICE: the clock driver is explicitly disabled */
#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_MAXIMUM_TASKS            1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
