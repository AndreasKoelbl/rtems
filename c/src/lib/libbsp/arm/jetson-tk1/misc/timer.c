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
#include <rtems.h>
#include <rtems/btimer.h>
#include <bsp/jetson-tk1.h>

static bool benchmark_timer_find_average_overhead = false;

static uint64_t benchmark_timer_base;

void benchmark_timer_initialize( void )
{
}

benchmark_timer_t benchmark_timer_read( void )
{
  return 0;
}

void benchmark_timer_disable_subtracting_average_overhead(
  bool find_average_overhead )
{
  benchmark_timer_find_average_overhead = find_average_overhead;
}
