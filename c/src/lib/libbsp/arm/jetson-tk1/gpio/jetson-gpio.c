/*
 * Copyright (c) OTH Regensburg, 2017
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#include <bsp.h>
#include <bsp/irq-generic.h>
#include <bsp/jetson-gpio.h>
#include <bsp/gpio.h>

#define GPIO_1_IRQ (32 + 32)
#define GPIO_2_IRQ (33 + 32)
#define GPIO_3_IRQ (34 + 32)
#define GPIO_4_IRQ (35 + 32)
#define GPIO_5_IRQ (55 + 32)
#define GPIO_6_IRQ (87 + 32)
#define GPIO_7_IRQ (89 + 32)
#define GPIO_8_IRQ (125 + 32)

#define PU0 0x3184
#define PU1 0x3188
#define PU2 0x318c
#define PU3 0x3190
#define PU4 0x3194
#define PU5 0x3198
#define PU6 0x319c
#define PH1 0x3214

#define P_LOCK             (1 << 7)
#define P_E_INPUT          (1 << 5)
#define P_TRISTATE         (1 << 4)
#define P_RVSD             ((1 << 3)|(1 << 2))
#define P_PULL_UP          ((1 << 3)|(0 << 2))
#define P_PULL_DOWN        ((0 << 3)|(1 << 2))
#define P_PULL_DOWN_NORMAL ((0 << 3)|(0 << 2))
#define P_GMI_DTV          ((1 << 1)|(1 << 0))
#define P_GMI_GMI          ((1 << 1)|(0 << 0))
#define P_GMI_TRACE        ((0 << 1)|(0 << 0))
#define P_GMI_PWM          ((0 << 1)|(0 << 0))

RTEMS_INTERRUPT_LOCK_DEFINE(static, rtems_gpio_bsp_lock, "rtems_gpio_bsp_lock");

static rtems_status_code jetson_select_pin_function(
  uint32_t bank,
  uint32_t pin,
  uint32_t type
) {
  return RTEMS_NOT_DEFINED;
}

rtems_status_code rtems_gpio_bsp_multi_set(uint32_t bank, uint32_t bitmask)
{
  return RTEMS_NOT_DEFINED;
}

rtems_status_code rtems_gpio_bsp_multi_clear(uint32_t bank, uint32_t bitmask)
{
  return RTEMS_NOT_DEFINED;
}

uint32_t rtems_gpio_bsp_multi_read(uint32_t bank, uint32_t bitmask)
{
  return RTEMS_NOT_DEFINED;
}

rtems_status_code rtems_gpio_bsp_set(uint32_t bank, uint32_t pin)
{
  return RTEMS_NOT_DEFINED;
}

rtems_status_code rtems_gpio_bsp_clear(uint32_t bank, uint32_t pin)
{
  return RTEMS_NOT_DEFINED;
}

uint32_t rtems_gpio_bsp_get_value(uint32_t bank, uint32_t pin)
{
  return -1;
}

rtems_status_code rtems_gpio_bsp_select_input(
  uint32_t bank,
  uint32_t pin,
  void *bsp_specific
) {
  return RTEMS_NOT_DEFINED;
}

rtems_status_code rtems_gpio_bsp_select_output(
  uint32_t bank,
  uint32_t pin,
  void *bsp_specific
) {
  return RTEMS_NOT_DEFINED;
}

rtems_status_code rtems_gpio_bsp_select_specific_io(
  uint32_t bank,
  uint32_t pin,
  uint32_t function,
  void *pin_data
) {
  return RTEMS_NOT_DEFINED;
}

rtems_status_code rtems_gpio_bsp_set_resistor_mode(
  uint32_t bank,
  uint32_t pin,
  rtems_gpio_pull_mode mode
) {
  switch (mode) {
    case PULL_UP:
      break;
    case PULL_DOWN:
      break;
    case NO_PULL_RESISTOR:
      break;
    default:
      return RTEMS_UNSATISFIED;
  }
  return RTEMS_NOT_DEFINED;
}

rtems_vector_number rtems_gpio_bsp_get_vector(uint32_t bank)
{
  switch(bank) {
    case 1:
      return GPIO_1_IRQ;
    case 2:
      return GPIO_2_IRQ;
    case 3:
      return GPIO_3_IRQ;
    case 4:
      return GPIO_4_IRQ;
    case 5:
      return GPIO_5_IRQ;
    case 6:
      return GPIO_6_IRQ;
    case 7:
      return GPIO_7_IRQ;
    case 8:
      return GPIO_8_IRQ;
    default:
      return -1;
  }
}

uint32_t rtems_gpio_bsp_interrupt_line(rtems_vector_number vector)
{
  return 0;
}

rtems_status_code rtems_gpio_bsp_enable_interrupt(
  uint32_t bank,
  uint32_t pin,
  rtems_gpio_interrupt interrupt
) {
  return RTEMS_NOT_DEFINED;
}

rtems_status_code rtems_gpio_bsp_disable_interrupt(
  uint32_t bank,
  uint32_t pin,
  rtems_gpio_interrupt interrupt
) {
  return RTEMS_NOT_DEFINED;
}

rtems_status_code rtems_gpio_bsp_multi_select(
  rtems_gpio_multiple_pin_select *pins,
  uint32_t pin_count,
  uint32_t select_bank
) {
  return RTEMS_NOT_DEFINED;
}

rtems_status_code rtems_gpio_bsp_specific_group_operation(
  uint32_t bank,
  uint32_t *pins,
  uint32_t pin_count,
  void *arg
) {
  return RTEMS_NOT_DEFINED;
}
