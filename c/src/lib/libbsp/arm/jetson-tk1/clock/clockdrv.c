/**
 * @file
 *
 * @ingroup bsp_clock
 *
 * @brief Raspberry Pi clock support.
 */

/*
 * BCM2835 Clock driver
 *
 * Copyright (c) 2013 Alan Cudmore
 * Copyright (c) 2016 Pavel Pisa
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *
 *  http://www.rtems.org/license/LICENSE
 *
*/

#include <rtems.h>
#include <bsp.h>
#include <bsp/irq.h>
#include <bsp/irq-generic.h>
#include <bsp/raspberrypi.h>
#include <rtems/timecounter.h>

/* This is defined in ../../../shared/clockdrv_shell.h */
void Clock_isr(rtems_irq_hdl_param arg);
static uint32_t counter = 0;

static uint32_t jetson_clock_get_timecount(struct timecounter *tc)
{
  writeText("clock get timecount");
  return counter++;
}

static void jetson_clock_at_tick(void)
{
  writeText("clock at tick");
}

static void jetson_clock_handler_install_isr(
  rtems_isr_entry clock_isr
)
{
  writeText("clock handler install isr");
  printHex((uint32_t) clock_isr);
  writeText("Itoa end");
  rtems_fatal_error_occurred(0xdeadbeef);
}

static void jetson_clock_initialize_hardware(void)
{
  writeText("clock init hardware");
  rtems_fatal_error_occurred(0xdeadbeef);
}

static void jetson_clock_cleanup(void)
{
  writeText("clock_cleanup");
}

#define Clock_driver_support_at_tick() jetson_clock_at_tick()

#define Clock_driver_support_initialize_hardware() jetson_clock_initialize_hardware()

#define Clock_driver_support_shutdown_hardware() jetson_clock_cleanup()

#define Clock_driver_support_install_isr(clock_isr, old_isr) \
  do {                                                 \
    jetson_clock_handler_install_isr(clock_isr);  \
    old_isr = NULL;                                    \
  } while (0)

#define CLOCK_DRIVER_USE_ONLY_BOOT_PROCESSOR 1

#include "../../../shared/clockdrv_shell.h"
