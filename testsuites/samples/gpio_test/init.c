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
#define GPIO_CNF      0x00
#define GPIO_OE       0x10
#define GPIO_OUT      0x20
#define GPIO_IN       0x30
#define GPIO_INT_STA  0x40
#define GPIO_INT_ENB  0x50
#define GPIO_INT_LVL  0x60
#define GPIO_INT_CLR  0x70
#define GPIO_MSK_CNF      0x80
#define GPIO_MSK_OE       0x90
#define GPIO_MSK_OUT      0xa0
#define GPIO_MSK_INT_STA  0xc0
#define GPIO_MSK_INT_ENB  0xd0
#define GPIO_MSK_INT_LVL  0xe0

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
#define BANK 0x500
#define PINMUX_PORT_U 0x184
#define OUTPUT 6
#define INPUT 0

rtems_task Init(rtems_task_argument argument);
void irq_handler(void *arg);

rtems_task Init(rtems_task_argument ignored)
{
  rtems_status_code status;
  rtems_vector_number vector;
  uint32_t port = PINMUX_PORT_U;
  uint32_t pin_value = 0, i;

  printf("I am ALIVE\n");
  rtems_gpio_bsp_select_output(BANK, OUTPUT, &port);
  rtems_gpio_bsp_select_output(BANK, INPUT, &port);

  rtems_gpio_bsp_multi_set(BANK, (1 << INPUT) | (1 << OUTPUT));
  rtems_gpio_bsp_select_input(BANK, OUTPUT, &port);
  rtems_gpio_bsp_select_input(BANK, INPUT, &port);
  printf("values: %lx\n", rtems_gpio_bsp_multi_read(BANK, 0xff));
  rtems_gpio_bsp_select_output(BANK, OUTPUT, &port);
  rtems_gpio_bsp_select_output(BANK, INPUT, &port);
  rtems_gpio_bsp_multi_clear(BANK, 0xff);
  rtems_gpio_bsp_select_input(BANK, OUTPUT, &port);
  rtems_gpio_bsp_select_input(BANK, INPUT, &port);
  printf("values: %lx\n", rtems_gpio_bsp_multi_read(BANK, 0xff));


#if 0
  rtems_gpio_bsp_set(BANK, OUTPUT);
  printf("Waiting for input\n");
  while (!pin_value) {
    pin_value = rtems_gpio_bsp_get_value(BANK, INPUT);
    printf("pin value: %lu", pin_value);
  }

  rtems_gpio_bsp_clear(BANK, OUTPUT);
  rtems_task_wake_after(10);

  vector = rtems_gpio_bsp_get_vector(BANK);
  status = rtems_gpio_bsp_enable_interrupt(BANK, INPUT, FALLING_EDGE);
  rtems_directive_failed(status, "enable interrupt");
  status = rtems_interrupt_handler_install(
    vector,
    "GPIO6",
    RTEMS_INTERRUPT_UNIQUE,
    irq_handler,
    &vector
  );
  printf("Entering wfi\n");
#endif
  while (true) {
    asm volatile("wfi");
  }
  rtems_task_delete(rtems_task_self());
}

void irq_handler(void *num)
{
  printf("irq\n");
  rtems_gpio_bsp_interrupt_line(*((uint32_t*)num));
  rtems_gpio_bsp_disable_interrupt(BANK, INPUT, FALLING_EDGE);
}

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_MICROSECONDS_PER_TICK 100000

#define CONFIGURE_MAXIMUM_TASKS 2

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
