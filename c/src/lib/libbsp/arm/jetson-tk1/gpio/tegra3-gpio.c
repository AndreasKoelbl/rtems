/*
 * Copyright (c) OTH Regensburg, 2017
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#include <bsp.h>
#include <bsp/gpio.h>
#include <bsp/tegra3-gpio.h>

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

#define GPIO_BANK(x)  ((x >> 2) * 0x100)
#define GPIO_PORT(x)  (x - GPIO_BANK(x))

/*
 * To achieve a performant implementation the bank has also to include the
 * port
 */

static inline void tegra_gpio_write(
  uintptr_t gpio_reg,
  uint32_t bank,
  uint32_t pin,
  uint32_t value
)
{
  mmio_write32(GPIO_BASE + gpio_reg + bank*0x100 + pin, value);
}

static uint32_t tegra_gpio_read(
  uintptr_t gpio_reg,
  uint32_t bank,
  uint32_t pin
)
{
  return mmio_read32(GPIO_BASE + gpio_reg + bank*0x100 + pin);
}

static inline void pinmux_write(
    uint32_t port,
    uint32_t pin,
    tegra_pinmux_property property
)
{
	mmio_write32(PINMUX_AUX + port + 4*pin, property);
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
	return 0;
}

rtems_status_code rtems_gpio_bsp_select_input(
  uint32_t bank,
  uint32_t pin,
  void *bsp_specific
)
{
  return RTEMS_NOT_DEFINED;
}

rtems_status_code rtems_gpio_bsp_select_output(
  uint32_t bank,
  uint32_t pin,
  void *bsp_specific
)
{
  return RTEMS_NOT_DEFINED;
}

rtems_status_code rtems_gpio_bsp_select_specific_io(
  uint32_t bank,
  uint32_t pin,
  uint32_t function,
  void *pin_data
)
{
  return RTEMS_NOT_DEFINED;
}

rtems_status_code rtems_gpio_bsp_set_resistor_mode(
  uint32_t bank,
  uint32_t pin,
  rtems_gpio_pull_mode mode
)
{
  return RTEMS_NOT_DEFINED;
}

rtems_vector_number rtems_gpio_bsp_get_vector(uint32_t bank)
{
  return RTEMS_NOT_DEFINED;
}

uint32_t rtems_gpio_bsp_interrupt_line(rtems_vector_number vector)
{
  return RTEMS_NOT_DEFINED;
}

rtems_status_code rtems_gpio_bsp_enable_interrupt(
  uint32_t bank,
  uint32_t pin,
  rtems_gpio_interrupt interrupt
)
{
  return RTEMS_NOT_DEFINED;
}

rtems_status_code rtems_gpio_bsp_disable_interrupt(
  uint32_t bank,
  uint32_t pin,
  rtems_gpio_interrupt interrupt
)
{
  return RTEMS_NOT_DEFINED;
}

rtems_status_code rtems_gpio_bsp_multi_select(
  rtems_gpio_multiple_pin_select *pins,
  uint32_t pin_count,
  uint32_t select_bank
)
{
  return RTEMS_NOT_DEFINED;
}

rtems_status_code rtems_gpio_bsp_specific_group_operation(
  uint32_t bank,
  uint32_t *pins,
  uint32_t pin_count,
  void *arg
)
{
  return RTEMS_NOT_DEFINED;
}
