/*
 * Copyright (c) OTH Regensburg, 2017
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#include <stdint.h>
#include <bsp/car.h>

#define CLK_ENB_L_SET (CAR + 0x320)
#define CLK_ENB_L_CLR (CAR + 0x324)
#define CLK_ENB_U_SET (CAR + 0x330)
#define CLK_ENB_U_CLR (CAR + 0x334)
#define CLK_SOURCE_UARTA (CAR + 0x178)
#define CLK_SOURCE_UARTD (CAR + 0x1c0)

#define RST_DEV_L_SET (CAR + 0x300)
#define RST_DEV_L_CLR (CAR + 0x304)
#define RST_DEV_U_SET (CAR + 0x310)
#define RST_DEV_U_CLR (CAR + 0x314)

#define GATE_UARTA (1 << 6)
#define GATE_UARTD (1 << 0)

car_dev tegra124_car_dev[ CAR_NUM_DEV ] = {
  {
    .clock = {
      .enb_set = CLK_ENB_U_SET,
      .enb_clear = CLK_ENB_U_CLR,
    },
    .reset = {
      .enb_set = RST_DEV_U_SET,
      .enb_clear = RST_DEV_U_CLR,
    },
    .src_reg = CLK_SOURCE_UARTD,
    .gate = GATE_UARTD,
  },
#if NS8250_USE_SECONDARY_CONSOLE == 1
  {
    .clock = {
      .enb_set = CLK_ENB_L_SET,
      .enb_clear = CLK_ENB_L_CLR,
    },
    .reset = {
      .enb_set = RST_DEV_L_SET,
      .enb_clear = RST_DEV_L_CLR,
    },
    .src_reg = (void*) CLK_SOURCE_UARTA,
    .gate = GATE_UARTA,
  },
#endif
};
