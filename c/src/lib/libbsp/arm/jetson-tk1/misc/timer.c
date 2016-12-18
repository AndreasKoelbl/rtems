/**
 * @file
 *
 * @ingroup raspberrypi
 *
 * @brief Benchmark timer support.
 */

/*
 * Copyright (c) 2013 by Alan Cudmore
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *
 *  http://www.rtems.org/license/LICENSE
 *
 */

#include <bsp.h>
#include <bsp/irq.h>
#include <bsp/memory.h>
#include <rtems.h>
#include <rtems/btimer.h>

#define TIMER_IRQ 27
#define BEATS_PER_SEC 10

static bool benchmark_timer_find_average_overhead = false;

static uint64_t benchmark_timer_base;
static volatile uint64_t expected_ticks;
static uint64_t ticks_per_beat;

static uint32_t getTimerValue( uint32_t irqn )
{
  static uint64_t min_delta = ~0ULL, max_delta = 0;
	uint64_t delta;

	if (irqn != TIMER_IRQ)
		return;

	delta = timer_get_ticks() - expected_ticks;
	if (delta < min_delta)
		min_delta = delta;
	if (delta > max_delta)
		max_delta = delta;

	printk("Timer fired, jitter: %6ld ns, min: %6ld ns, max: %6ld ns\n",
	       (long)timer_ticks_to_ns(delta),
	       (long)timer_ticks_to_ns(min_delta),
	       (long)timer_ticks_to_ns(max_delta));

	expected_ticks = timer_get_ticks() + ticks_per_beat;
	timer_start(ticks_per_beat);
  return 0;
}

void benchmark_timer_initialize( void )
{
  /* TODO: get timer value */
  benchmark_timer_base = 0;

  gic_enable_irq(TIMER_IRQ);

	//arm_read_sysreg(CNTPCT_EL0, benchmark_timer_base);
	return benchmark_timer_base;

}

benchmark_timer_t benchmark_timer_read( void )
{
  uint32_t delta = getTimerValue(TIMER_IRQ) - benchmark_timer_base;

  if ( benchmark_timer_find_average_overhead ) {
    return delta;
  } else {
    return getTimerValue(TIMER_IRQ);
  }
}

void benchmark_timer_disable_subtracting_average_overhead(
  bool find_average_overhead )
{
  benchmark_timer_find_average_overhead = find_average_overhead;
}
