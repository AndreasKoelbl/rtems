/*
 *  COPYRIGHT (c) 1989-2012.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/printer.h>
#include <rtems/test.h>

#include <bsp.h> /* for device driver prototypes */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define abs(x) ((x)<0 ? -(x) : (x))

rtems_id     Timer1;
uint8_t value = 0;

static uint32_t ticks_per_beat;
static volatile uint32_t expected_ticks;

/* forward declarations to avoid warnings */
rtems_task Init(rtems_task_argument argument);
rtems_timer_service_routine Timer_Routine( rtems_id id, void *ignored );
void handle_IRQ(rtems_id id, void* data);
uint64_t timer_ticks_to_ns(uint64_t ticks);

const char rtems_test_name[] = "HELLO WORLD";
rtems_printer rtems_test_printer;

uint64_t timer_ticks_to_ns(uint64_t ticks)
{
	return (ticks * 1000) / (12000000 / 1000 / 1000);
}

rtems_task Init(
  rtems_task_argument ignored
)
{
  rtems_status_code status;
  ticks_per_beat = 1200000;

  rtems_print_printer_printf(&rtems_test_printer);

  status = rtems_timer_create(rtems_build_name( 'T', 'M', 'R', '1' ), &Timer1);
  if ( status != RTEMS_SUCCESSFUL )
  {
    fprintf(stderr, "Timer1 create failed\n");
    exit( 0 );
  }
  //random error removal
  expected_ticks = jetson_clock_get_timecount() + ticks_per_beat + 138583;
  status = rtems_timer_fire_after(Timer1, 1, handle_IRQ, NULL);
  if ( status != RTEMS_SUCCESSFUL )
  {
    fprintf(stderr, "Timer1 fire failed\n");
    exit( 0 );
  }

  while (1) {
    /* maybe we should sleep in here: rtems_task_wake_after */
    asm volatile("wfi" : : : "memory");
  }
  rtems_test_end();
  rtems_task_delete( RTEMS_SELF );
}

void handle_IRQ(rtems_id id, void* data)
{
  rtems_status_code status;
  uint64_t currentTime = 0;
  static uint64_t oldTime = 0;
  static uint64_t minDelta = UINT64_MAX;
  static uint64_t maxDelta = 0;
  uint64_t delta = 0;

  currentTime = jetson_clock_get_timecount();
  if (expected_ticks < currentTime) {
    delta = currentTime - expected_ticks;
  //printf("delta: %lld = currentTime: %lld - expected_ticks: %lld\n", delta, currentTime, expected_ticks);
  } else {
    delta = expected_ticks - currentTime;
  }

  if (delta < minDelta)
  {
    minDelta = delta;
  }
  if (delta > maxDelta)
  {
    maxDelta = delta;
  }

/*  printf("count: %lu ticks_per_beat: %lu id: %d rtemsticks: %d, delta: %lu,"\
         "expected_ticks: %lu\n", (uint32_t) currentTime, ticks_per_beat, id,
         rtems_clock_get_ticks_since_boot(), (uint32_t) delta, expected_ticks);
         */
	printf("Timer fired jitter: %6ld ns, min: %6ld ns, max: %6ld ns\n",
         (long)timer_ticks_to_ns(delta),
         (long)timer_ticks_to_ns(minDelta),
         (long)timer_ticks_to_ns(maxDelta));
  if (currentTime > UINT64_MAX - ticks_per_beat)
  {
    expected_ticks = ticks_per_beat + UINT64_MAX - currentTime;
  }
  else
  {
    expected_ticks = currentTime + ticks_per_beat;
  }
  oldTime = currentTime;
  status = rtems_timer_fire_after(id, 1, handle_IRQ, NULL);
}

/* NOTICE: the clock driver is explicitly disabled */
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_MAXIMUM_TASKS             1
#define CONFIGURE_MAXIMUM_TIMERS            1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION
#define CONFIGURE_MICROSECONDS_PER_TICK 100000

#define CONFIGURE_EXTRA_TASK_STACKS         (3 * RTEMS_MINIMUM_STACK_SIZE)
#define CONFIGURE_INIT
#include <rtems/confdefs.h>
