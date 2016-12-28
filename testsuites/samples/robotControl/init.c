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

#define BEATS_PER_SEC       1000
rtems_id     Timer1;
uint8_t value = 0;

static uint64_t ticks_per_beat;
static volatile uint64_t expected_ticks;

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
  ticks_per_beat = timer_get_frequency() / BEATS_PER_SEC;
  expected_ticks = jetson_clock_get_timecount() + ticks_per_beat;

  rtems_print_printer_printf(&rtems_test_printer);

  status = rtems_timer_create(rtems_build_name( 'T', 'M', 'R', '1' ), &Timer1);
  if ( status != RTEMS_SUCCESSFUL )
  {
    fprintf(stderr, "Timer1 create failed\n");
    exit( 0 );
  }
  printf("Initializing the timer...\n");
  handle_IRQ(Timer1, NULL);
  while (1) {
    printf("Wait for interrupts\n");
    /* maybe we should sleep in here: rtems_task_wake_after */
    asm volatile("wfi" : : : "memory");
  }
  rtems_test_end();
  rtems_task_delete( RTEMS_SELF );
}

void handle_IRQ(rtems_id id, void* data)
{
  static uint32_t flag = 0;
  uint64_t delta;
  rtems_status_code status;
  if (flag == 0)
  {
    flag = 1;
  }
  else
  {
    flag = 0;
  }
  delta = jetson_clock_get_timecount() - expected_ticks;
  printf("ticks_per_beat / 10: %d id: %d data: %p, flag: %d, rtemsticks: %d, delta: %lu\n", ticks_per_beat / 10, id, data, flag, rtems_clock_get_ticks_since_boot(), delta);
  status = rtems_timer_fire_after(id, ticks_per_beat, handle_IRQ, NULL);
//  status = rtems_timer_fire_after(id, 1, handle_IRQ, NULL);
  if ( status != RTEMS_SUCCESSFUL )
  {
    fprintf(stderr, "Timer1 fire failed\n");
    exit( 0 );
  }
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
