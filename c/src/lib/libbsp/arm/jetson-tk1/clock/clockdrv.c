/**
 * @file
 *
 * @ingroup arm_jetson-tk1
 *
 * @brief Jetson-TK1 foooooooo
 *
 * A short description of the purpose of this file.
 */

/*
 * Copyright (c) OTH Regensburg, 2016
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#include <rtems/irq-extension.h>
#include <rtems/config.h>
//#include <bsp/gic.h>
#include "../include/gic.h"

#define BEATS_PER_SEC 10

static uint64_t ticks_per_beat;
static volatile uint64_t expected_ticks;

static void handle_IRQ(unsigned int irqn)
{
  /*
  static uint64_t min_delta = ~0ULL, max_delta = 0;
	uint64_t delta;

	if (irqn != TIMER_IRQ)
		return;

  rtems_clock_tick();
	delta = timer_get_ticks() - expected_ticks;
	if (delta < min_delta)
		min_delta = delta;
	if (delta > max_delta)
		max_delta = delta;

	printk("Timer fired, jitter: %6ld ns, min: %6ld ns, max: %6ld ns\n",
         (long)timer_ticks_to_ns(delta),
	       (long)timer_ticks_to_ns(min_delta),
	       (long)timer_ticks_to_ns(max_delta));


  /* TODO: calculate the ticks we want */
  /*rtems_configuration_get_microseconds_per_tick();
  expected_ticks = timer_get_ticks() + ticks_per_beat;
  timer_start(ticks_per_beat);
  */
}

static void jetsontk1_support_initialize_hardware( void )
{
/*  gic_setup(handle_IRQ);
  gic_enable_irq(TIMER_IRQ);
	ticks_per_beat = timer_get_frequency() / BEATS_PER_SEC;
	expected_ticks = timer_get_ticks() + ticks_per_beat;
	timer_start(ticks_per_beat);
  */
}

static void jetsontk1_support_install_isr(rtems_interrupt_handler isr)
{
  rtems_status_code sc;

  sc = rtems_interrupt_handler_install(
    TIMER_IRQ,
    "Clock",
    RTEMS_INTERRUPT_UNIQUE,
    isr,
    NULL
  );
  if ( sc != RTEMS_SUCCESSFUL ) {
    bsp_fatal( TEGRA_FATAL_IRQ_INSTALL );
  }
}

static void jetsontk1_support_at_tick( void )
{
  /*
   * TODO: Clear interrupt only if there is time left to the next tick.
   * If time of the next tick has already passed then interrupt
   * request stays active and fires immediately after current tick
   * processing is finished.
   */
}

#define Clock_driver_support_at_tick() \
  jetsontk1_support_at_tick()
#define Clock_driver_support_install_isr( isr, old ) \
  jetsontk1_support_install_isr( isr )
#define Clock_driver_support_initialize_hardware() \
  jetsontk1_support_initialize_hardware()

/* Indicate that this clock driver lacks a proper timecounter in hardware */
#define CLOCK_DRIVER_USE_DUMMY_TIMECOUNTER
#define CLOCK_DRIVER_ISRS_PER_TICK 1
#define CLOCK_DRIVER_ISRS_PER_TICK_VALUE 1

#include "../../../shared/clockdrv_shell.h"
