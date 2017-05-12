/*
 * Copyright (c) OTH Regensburg, 2017
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#include <bsp.h>
#include <bsp/gpio.h>
#include <bsp/memory.h>
#include <bsp/tegra124-gpio.h>

#define GPIO_CNF 0x00
#define GPIO_OE 0x10
#define GPIO_OUT 0x20
#define GPIO_IN 0x30
#define GPIO_INT_STA 0x40
#define GPIO_INT_ENB 0x50
#define GPIO_INT_LVL 0x60
#define GPIO_INT_CLR 0x70

#define GPIO_MSK_CNF 0x80
#define GPIO_MSK_OE 0x90
#define GPIO_MSK_OUT 0xa0
#define GPIO_MSK_INT_STA 0xc0
#define GPIO_MSK_INT_ENB 0xd0
#define GPIO_MSK_INT_LVL 0xe0

/*
 * To achieve a performant and compatible implementation the bank parameter has
 * also to include the gpio port address, Tegra K1 TRM, p. 278
 */

static inline void tegra_gpio_write(
  uintptr_t gpio_reg,
  uint32_t  bank,
  uint32_t  value
)
{
  mmio_write( GPIO_BASE + gpio_reg + bank, value );
}

static uint32_t tegra_gpio_read(
  uintptr_t gpio_reg,
  uint32_t  bank
)
{
  return mmio_read( GPIO_BASE + gpio_reg + bank );
}

static inline void pinmux_write(
  uint32_t              pinmux_reg,
  uint32_t              pin,
  tegra_pinmux_property property
)
{
  mmio_write( PINMUX_AUX + pinmux_reg + 4 * pin, property );
}

static inline uint32_t pinmux_read(
  uint32_t pinmux_reg,
  uint32_t pin
)
{
  return mmio_read( PINMUX_AUX + pinmux_reg + 4 * pin );
}

static inline void gpio_set_level(
  uint32_t             bank,
  uint32_t             pin,
  rtems_gpio_interrupt sens
)
{
  uint32_t value;

  value = tegra_gpio_read( GPIO_INT_LVL, bank );

  value &= ~( ( 1 << pin ) | ( ( 1 << 8 ) << pin ) | ( ( 1 << 16 ) << pin ) );

  if ( sens != NONE ) {
    value |= ( 1 << 8 ) << pin;
  }

  if ( sens == RISING_EDGE || sens == HIGH_LEVEL ) {
    value |= 1 << pin;
  } else if ( sens == BOTH_EDGES ) {
    value |= ( 1 << 16 ) << pin;
  }

  tegra_gpio_write( GPIO_INT_LVL, bank, value );
}

rtems_status_code rtems_gpio_bsp_multi_set(uint32_t bank, uint32_t bitmask)
{
  tegra_gpio_write( GPIO_OUT, bank, bitmask & 0xff );

  return RTEMS_SUCCESSFUL;
}

rtems_status_code rtems_gpio_bsp_multi_clear(uint32_t bank, uint32_t bitmask)
{
  tegra_gpio_write( GPIO_MSK_OUT, bank, bitmask << 8 );

  return RTEMS_SUCCESSFUL;
}

uint32_t rtems_gpio_bsp_multi_read(uint32_t bank, uint32_t bitmask)
{
  return tegra_gpio_read( GPIO_IN, bank ) & bitmask;
}

rtems_status_code rtems_gpio_bsp_set(uint32_t bank, uint32_t pin)
{
  tegra_gpio_write( GPIO_MSK_OUT, bank, ( ( 1 << 8 ) << pin ) | ( 1 << pin ) );

  return RTEMS_SUCCESSFUL;
}

rtems_status_code rtems_gpio_bsp_clear(uint32_t bank, uint32_t pin)
{
  tegra_gpio_write( GPIO_MSK_OUT, bank, ( 1 << 8 ) << pin );

  return RTEMS_SUCCESSFUL;
}

uint32_t rtems_gpio_bsp_get_value(uint32_t bank, uint32_t pin)
{
  return ( ( tegra_gpio_read( GPIO_IN, bank ) & ( 1 << pin ) ) != 0 );
}

rtems_status_code rtems_gpio_bsp_select_input(
  uint32_t bank,
  uint32_t pin,
  void    *pinmux_reg
)
{
  pinmux_write(
    *( (uint32_t *) ( pinmux_reg ) ),
    pin,
    TEGRA_E_INPUT | TEGRA_TRISTATE
  );
  tegra_gpio_write( GPIO_MSK_CNF, bank, 1 << pin );
  tegra_gpio_write( GPIO_MSK_OE, bank, 1 << pin );

  return RTEMS_SUCCESSFUL;
}

rtems_status_code rtems_gpio_bsp_select_output(
  uint32_t bank,
  uint32_t pin,
  void    *pinmux_reg
)
{
  pinmux_write( *( (uint32_t *) ( pinmux_reg ) ), pin, 0 );
  tegra_gpio_write( GPIO_MSK_CNF, bank, 1 << pin );
  tegra_gpio_write( GPIO_MSK_OE, bank, ( ( 1 << 8 ) << pin ) | ( 1 << pin ) );

  return RTEMS_SUCCESSFUL;
}

rtems_status_code rtems_gpio_bsp_select_specific_io(
  uint32_t bank,
  uint32_t pin,
  uint32_t function,
  void    *pin_data
)
{
  pinmux_write( bank, pin, (tegra_pinmux_property) function );

  return RTEMS_SUCCESSFUL;
}

rtems_status_code rtems_gpio_bsp_set_resistor_mode(
  uint32_t             bank,
  uint32_t             pin,
  rtems_gpio_pull_mode mode
)
{
  switch ( mode ) {
    case PULL_UP:
      pinmux_write( bank, pin, TEGRA_PULL_UP );
      break;
    case PULL_DOWN:
      pinmux_write( bank, pin, TEGRA_PULL_DOWN );
      break;
    case NO_PULL_RESISTOR:
      pinmux_write( bank, pin, TEGRA_PULL_DISABLED );
      break;
    default:

      return RTEMS_INVALID_NUMBER;
  };

  return RTEMS_SUCCESSFUL;
}

rtems_vector_number rtems_gpio_bsp_get_vector( uint32_t bank )
{
  uint32_t i;

  if ( ( bank / 0x100 ) > sizeof( gpio_vector_table ) ) {
    return -1;
  }

  return gpio_vector_table[ bank / 0x100 ];
}

uint32_t rtems_gpio_bsp_interrupt_line( rtems_vector_number vector )
{
  uint32_t bank;

  for ( bank = 0; bank < sizeof( gpio_vector_table ); bank++ ) {
    if ( gpio_vector_table[ bank ] == vector ) {
      tegra_gpio_write( GPIO_INT_CLR, bank * 0x100, 0xff );

      return tegra_gpio_read( GPIO_INT_STA, bank * 0x100 );
    }
  }

  return RTEMS_INVALID_NUMBER;
}

rtems_status_code rtems_gpio_bsp_enable_interrupt(
  uint32_t             bank,
  uint32_t             pin,
  rtems_gpio_interrupt interrupt
)
{
  tegra_gpio_write( GPIO_INT_CLR, bank, 1 << pin );
  gpio_set_level( bank, pin, interrupt );
  tegra_gpio_write( GPIO_MSK_INT_ENB,
    bank,
    ( ( 1 << 8 ) << pin ) | ( 1 << pin ) );

  return RTEMS_SUCCESSFUL;
}

rtems_status_code rtems_gpio_bsp_disable_interrupt(
  uint32_t             bank,
  uint32_t             pin,
  rtems_gpio_interrupt interrupt
)
{
  uint32_t level;

  tegra_gpio_write( GPIO_INT_CLR, bank, 1 << pin );
  level = tegra_gpio_read( GPIO_INT_LVL, bank );
  tegra_gpio_write( GPIO_MSK_INT_ENB, bank, ( ( 1 << 8 ) << pin ) );
  tegra_gpio_write( GPIO_INT_LVL, bank, level & ~interrupt );

  return RTEMS_SUCCESSFUL;
}

rtems_status_code rtems_gpio_bsp_multi_select(
  rtems_gpio_multiple_pin_select *pins,
  uint32_t                        pin_count,
  uint32_t                        select_bank
)
{
  return RTEMS_NOT_DEFINED;
}

rtems_status_code rtems_gpio_bsp_specific_group_operation(
  uint32_t  bank,
  uint32_t *pins,
  uint32_t  pin_count,
  void     *arg
)
{
  return RTEMS_NOT_DEFINED;
}
