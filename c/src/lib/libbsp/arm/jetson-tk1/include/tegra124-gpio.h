/*
 * Copyright (c) OTH Regensburg, 2017
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifndef LIBBSP_ARM_JETSON_GPIO_H
#define LIBBSP_ARM_JETSON_GPIO_H

#include <bsp.h>

#define GPIO_BASE ( (void *) 0x6000d000 )
#define PINMUX_AUX ( (void *) 0x70003000 )

#define GPIO1_IRQ ( 32 + 32 )
#define GPIO2_IRQ ( 33 + 32 )
#define GPIO3_IRQ ( 34 + 32 )
#define GPIO4_IRQ ( 35 + 32 )
#define GPIO5_IRQ ( 55 + 32 )
#define GPIO6_IRQ ( 87 + 32 )
#define GPIO7_IRQ ( 89 + 32 )
#define GPIO8_IRQ ( 125 + 32 )

static const uint8_t gpio_vector_table[ BSP_GPIO_BANK_COUNT ] = {
  GPIO1_IRQ,
  GPIO2_IRQ,
  GPIO3_IRQ,
  GPIO4_IRQ,
  GPIO5_IRQ,
  GPIO6_IRQ,
  GPIO7_IRQ,
  GPIO8_IRQ,
};

typedef enum {
  TEGRA_SFIO0 = ( ( 0 << 1 ) | ( 0 << 0 ) ),
  TEGRA_SFIO1 = ( ( 0 << 1 ) | ( 0 << 0 ) ),
  TEGRA_SFIO2 = ( ( 1 << 1 ) | ( 0 << 0 ) ),
  TEGRA_SFIO3 = ( ( 1 << 1 ) | ( 1 << 0 ) ),
  TEGRA_PULL_DISABLED = ( ( 0 << 3 ) | ( 0 << 2 ) ),
  TEGRA_PULL_DOWN = ( ( 0 << 3 ) | ( 1 << 2 ) ),
  TEGRA_PULL_UP = ( ( 1 << 3 ) | ( 0 << 2 ) ),
  TEGRA_TRISTATE = ( 1 << 4 ),
  TEGRA_E_INPUT = ( 1 << 5 ),
  TEGRA_E_OPEN_DRAIN = ( 1 << 6 ), /* Matters only for DD pads */
  TEGRA_LOCK = ( 1 << 7 ),
  TEGRA_IO_RESET = ( 1 << 8 ),    /* Matters only for LV pads */
} tegra_pinmux_property;

typedef enum {
  GPIO_PU0 = 0x184,
  GMI_AD8 = 0x210,
} pinmux_ball;

typedef enum {
  GPIO_DIR_IN,
  GPIO_DIR_OUT,
} gpio_direction;

#endif /* LIBBSP_ARM_JETSON_GPIO_H */
