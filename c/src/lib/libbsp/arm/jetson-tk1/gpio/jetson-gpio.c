/*
 * Copyright (c) OTH Regensburg, 2017
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#include <bsp.h>
#include <bsp/gpio.h>
#include <bsp/irq-generic.h>
#include <bsp/jetson-gpio.h>
#include <bsp/memory.h>

#define GPIO_1_REG 0x0
#define GPIO_2_REG 0x100
#define GPIO_3_REG 0x200
#define GPIO_4_REG 0x300
#define GPIO_5_REG 0x400
#define GPIO_6_REG 0x500
#define GPIO_7_REG 0x600
#define GPIO_8_REG 0x700

#define PU0 0x3184
#define PU1 0x3188
#define PU2 0x318c
#define PU3 0x3190
#define PU4 0x3194
#define PU5 0x3198
#define PU6 0x319c
#define PH1 0x3214

#define GPIO_CNF      0x00
#define GPIO_OE       0x10
#define GPIO_OUT      0x20
#define GPIO_IN       0x30
#define GPIO_INT_STA  0x40
#define GPIO_INT_ENB  0x50
#define GPIO_INT_LVL  0x60
#define GPIO_INT_CLR  0x70

#define GPIO_1_IRQ (32 + 32)
#define GPIO_2_IRQ (33 + 32)
#define GPIO_3_IRQ (34 + 32)
#define GPIO_4_IRQ (35 + 32)
#define GPIO_5_IRQ (55 + 32)
#define GPIO_6_IRQ (87 + 32)
#define GPIO_7_IRQ (89 + 32)
#define GPIO_8_IRQ (125 + 32)

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

typedef struct {
  void *regs;
  uint32_t pinmux;
  rtems_vector_number irq;
} jetson_gpio_context;

static jetson_gpio_context jetson_gpio_instances[] = {
  {
    .regs = GPIO_BASE + GPIO_1_REG,
    .pinmux = PINMUX_AUX + PU0,
    .irq = GPIO_1_IRQ,
  },
  {
    .regs = GPIO_BASE + GPIO_2_REG,
    .pinmux = PINMUX_AUX + PU1,
    .irq = GPIO_2_IRQ,
  },
  {
    .regs = GPIO_BASE + GPIO_3_REG,
    .pinmux = PINMUX_AUX + PU2,
    .irq = GPIO_3_IRQ,
  },
  {
    .regs = GPIO_BASE + GPIO_4_REG,
    .pinmux = PINMUX_AUX + PU3,
    .irq = GPIO_4_IRQ,
  },
  {
    .regs = GPIO_BASE + GPIO_5_REG,
    .pinmux = PINMUX_AUX + PU4,
    .irq = GPIO_5_IRQ,
  },
  {
    .regs = GPIO_BASE + GPIO_6_REG,
    .pinmux = PINMUX_AUX + PU5,
    .irq = GPIO_6_IRQ,
  },
  {
    .regs = GPIO_BASE + GPIO_7_REG,
    .pinmux = PINMUX_AUX + PU6,
    .irq = GPIO_8_IRQ,
  },
  {
    .regs = GPIO_BASE + GPIO_8_REG,
    .pinmux = PINMUX_AUX + PH1,
    .irq = GPIO_8_IRQ,
  },
};


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
  mmio_write32(jetson_gpio_instances[bank].regs + GPIO_OUT, 1 << bank);
  return RTEMS_SUCCESSFUL;
}

rtems_status_code rtems_gpio_bsp_clear(uint32_t bank, uint32_t pin)
{
  mmio_write32(jetson_gpio_instances[bank].regs + GPIO_INT_CLR, 0xff);
  return RTEMS_SUCCESSFUL;
}

uint32_t rtems_gpio_bsp_get_value(uint32_t bank, uint32_t pin)
{
  uint32_t result = (1 << bank);
  result &= mmio_read32(jetson_gpio_instances[bank].regs + GPIO_IN);

	return !(!result);
}

rtems_status_code rtems_gpio_bsp_select_input(
  uint32_t bank,
  uint32_t pin,
  void *bsp_specific
) {
  if (bank >= RTEMS_ARRAY_SIZE(jetson_gpio_instances)) {
    return RTEMS_UNSATISFIED;
  }
  mmio_write32(PINMUX_AUX + jetson_gpio_instances[bank].pinmux, MUX_E_INPUT |
    MUX_TRISTATE);

  return RTEMS_SUCCESSFUL;
}

rtems_status_code rtems_gpio_bsp_select_output(
  uint32_t bank,
  uint32_t pin,
  void *bsp_specific
) {
  if (bank >= RTEMS_ARRAY_SIZE(jetson_gpio_instances)) {
    return RTEMS_UNSATISFIED;
  }
  mmio_write32(PINMUX_AUX + jetson_gpio_instances[bank].pinmux, 0);
#ifndef JAILHOUSE_ENABLE
  /* Configures pin to be in GPIO mode */
  mmio_write32(jetson_gpio_instances[bank].regs + GPIO_CNF, 1 << bank);
#endif
  /* Output Enable */
  mmio_write32(jetson_gpio_instances[bank].regs + GPIO_OE, 1 << bank);

  return RTEMS_SUCCESSFUL;
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
      mmio_write32(jetson_gpio_instances[bank].pinmux, MUX_PULL_UP);
      break;
    case PULL_DOWN:
      mmio_write32(jetson_gpio_instances[bank].pinmux, MUX_PULL_DOWN);
      break;
    case NO_PULL_RESISTOR:
      mmio_write32(jetson_gpio_instances[bank].pinmux, MUX_TRISTATE);
      break;
    default:
      return RTEMS_UNSATISFIED;
  }
  return RTEMS_NOT_DEFINED;
}

rtems_vector_number rtems_gpio_bsp_get_vector(uint32_t bank)
{
  if (bank >= RTEMS_ARRAY_SIZE(jetson_gpio_instances)) {
    return RTEMS_UNSATISFIED;
  } else {
    return jetson_gpio_instances[bank].irq;
  }
}

uint32_t rtems_gpio_bsp_interrupt_line(rtems_vector_number vector)
{
  int i;
  uint32_t result = RTEMS_UNSATISFIED;

  for (i = 0; i < RTEMS_ARRAY_SIZE(jetson_gpio_instances); i++) {
    if (jetson_gpio_instances[i].irq == vector) {
      result = mmio_read32(jetson_gpio_instances[i].regs + GPIO_INT_STA);
      mmio_write32(jetson_gpio_instances[i].regs + GPIO_INT_CLR, 0xff);
      return result;
    }
  }

  return result;
}

rtems_status_code rtems_gpio_bsp_enable_interrupt(
  uint32_t bank,
  uint32_t pin,
  rtems_gpio_interrupt interrupt
) {
  uint32_t old;
  if (bank >= RTEMS_ARRAY_SIZE(jetson_gpio_instances)) {
    return RTEMS_UNSATISFIED;
  }

  /* Enable Interrupt */
  mmio_write32(jetson_gpio_instances[bank].regs + GPIO_INT_ENB, 1 << bank);
  old = mmio_read32(jetson_gpio_instances[bank].regs + GPIO_INT_LVL);
  switch (interrupt) {
    case FALLING_EDGE:
      mmio_write32(jetson_gpio_instances[bank].regs + GPIO_INT_LVL, (old &
        ~(1 << bank)) | ((1 << bank) << 8));
      break;
    case RISING_EDGE:
      mmio_write32(jetson_gpio_instances[bank].regs + GPIO_INT_LVL, old |
        (1 << bank) | ((1 << bank) << 8));
      break;
    case BOTH_EDGES:
      mmio_write32(jetson_gpio_instances[bank].regs + GPIO_INT_LVL, old |
        ((1 << bank) << 16));
      break;
    case LOW_LEVEL:
      mmio_write32(jetson_gpio_instances[bank].regs + GPIO_INT_LVL, old &
        ~(1 << bank));
      break;
    case HIGH_LEVEL:
      mmio_write32(jetson_gpio_instances[bank].regs + GPIO_INT_LVL, old |
        (1 << bank));
      break;
    case BOTH_LEVELS:
    case NONE:
    default:
      return RTEMS_UNSATISFIED;
  }
  /* Clear Interrupt */
// TODO: check if needed
  mmio_write32(jetson_gpio_instances[bank].regs + GPIO_INT_STA, 0);
  mmio_write32(jetson_gpio_instances[bank].regs + GPIO_INT_CLR, 0xff);
	mmio_write32(jetson_gpio_instances[bank].regs + GPIO_OUT, 0);

  return RTEMS_SUCCESSFUL;
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
