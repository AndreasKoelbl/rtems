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

#define BEATS_PER_SEC       1000
#define abs(x) ((x)<0 ? -(x) : (x))

rtems_id     Timer1;
uint8_t value = 0;

static uint32_t ticks_per_beat;
static volatile uint32_t expected_ticks;

/* forward declarations to avoid warnings */
rtems_task Init(rtems_task_argument argument);
rtems_timer_service_routine Timer_Routine( rtems_id id, void *ignored );
void handle_IRQ(rtems_id id, void* data);

const char rtems_test_name[] = "HELLO WORLD";
rtems_printer rtems_test_printer;

rtems_task Init(
  rtems_task_argument ignored
)
{
  rtems_status_code status;
  ticks_per_beat = 10000;

  rtems_print_printer_printf(&rtems_test_printer);

  status = rtems_timer_create(rtems_build_name( 'T', 'M', 'R', '1' ), &Timer1);
  if ( status != RTEMS_SUCCESSFUL )
  {
    fprintf(stderr, "Timer1 create failed\n");
    exit( 0 );
  }
  status = rtems_timer_fire_after(Timer1, ticks_per_beat * 2, handle_IRQ, NULL);
  if ( status != RTEMS_SUCCESSFUL )
  {
    fprintf(stderr, "Timer1 fire failed\n");
    exit( 0 );
  }

  while (1) {
    printf("\nWait for interrupts\n");
    /* maybe we should sleep in here: rtems_task_wake_after */
    asm volatile("wfi" : : : "memory");
  }
  rtems_test_end();
  rtems_task_delete( RTEMS_SELF );
}

void handle_IRQ(rtems_id id, void* data)
{
  rtems_status_code status;
  uint32_t currentTime = jetson_clock_get_timecount();
  static uint32_t oldTime = 0;
  static int32_t minDelta = INT_MAX;
  static int32_t maxDelta = INT_MIN;
  int32_t delta = 0;

  expected_ticks = 1000000 + ticks_per_beat;
  if (oldTime < currentTime)
  {
    delta = (int32_t) (currentTime - oldTime - expected_ticks);
  } else {
    delta = (int32_t) (0xffffffff - oldTime + currentTime - expected_ticks);
  }

  if (abs(delta) < minDelta)
  {
    minDelta = abs(delta);
  }
  if ((delta > maxDelta) )//  && (delta < 99999))
  {
    maxDelta = delta;
  }

	printf("Timer fired jitter: %6ld ns, min: %6ld ns, max: %6ld ns\n",
         delta, minDelta, maxDelta);
  oldTime = currentTime;
//  printf("count: %lu ticks_per_beat: %lu id: %d data: %p, rtemsticks: %d, "\
//         "delta: %lu expected_ticks: %lu\n", currentTime, ticks_per_beat, id, data,
//         rtems_clock_get_ticks_since_boot(), delta, expected_ticks);

  status = rtems_timer_fire_after(id, 20000, handle_IRQ, NULL);

}



/* NOTICE: the clock driver is explicitly disabled */
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_MAXIMUM_TASKS             1
#define CONFIGURE_MAXIMUM_TIMERS            1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION
#define CONFIGURE_MICROSECONDS_PER_TICK 1000000

#define CONFIGURE_EXTRA_TASK_STACKS         (3 * RTEMS_MINIMUM_STACK_SIZE)
#define CONFIGURE_INIT
#include <rtems/confdefs.h>
