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

rtems_id     Timer1;
uint8_t value = 0;

/* forward declarations to avoid warnings */
rtems_task Init(rtems_task_argument argument);
rtems_timer_service_routine Timer_Routine( rtems_id id, void *ignored );
void handle_IRQ(uint32_t data);

const char rtems_test_name[] = "HELLO WORLD";
rtems_printer rtems_test_printer;

rtems_task Init(
  rtems_task_argument ignored
)
{
  rtems_status_code status;
  rtems_print_printer_printf(&rtems_test_printer);

  status = rtems_timer_create(rtems_build_name( 'T', 'M', 'R', '1' ), &Timer1);
  if ( status != RTEMS_SUCCESSFUL )
  {
    fprintf(stderr, "Timer1 create failed\n");
  }
  else
  {
    printf("Timer1 create success\n");
  }


  Timer_Routine(Timer1, NULL);

  printf( "Hello World\n" );

  status = rtems_timer_fire_after(Timer1, 10, handle_IRQ, NULL);

  rtems_interval interval = rtems_clock_get_ticks_since_boot();
   while (1) {
    printf( "before wake\n" );
    interval = rtems_clock_get_ticks_since_boot();
    printf("interval: %d\n", interval);
    printf("Wait for interrupts\n");

    while (1)
      asm volatile("wfi" : : : "memory");
      status = rtems_task_wake_after( 1 );
      printf("after");
  }
  rtems_test_end();
  exit( 0 );
}

rtems_timer_service_routine Timer_Routine( rtems_id id, void *ignored )
{
  rtems_status_code status;
  printf("timer install after: %d\n", 2 * rtems_clock_get_ticks_per_second());

  if ( id == Timer1 )
    value = 1;
  else
    value = 2;

  status = rtems_timer_fire_after(
    id,
    1,
    Timer_Routine,
    NULL
  );
}

void handle_IRQ(uint32_t data)
{
  static uint32_t flag = 0;
  rtems_status_code status;
  if (flag == 0)
  {
    flag = 1;
  }
  else
  {
    flag = 0;
  }
  printf("data: %d received, flag: %d, ticks since boot: %d\n", data, flag, rtems_clock_get_ticks_since_boot());
  status = rtems_timer_fire_after(Timer1, rtems_clock_get_ticks_per_second() * 10, handle_IRQ, NULL);
  if ( status != RTEMS_SUCCESSFUL )
  {
    fprintf(stderr, "Timer1 fire failed\n");
  }
}



/* NOTICE: the clock driver is explicitly disabled */
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_MAXIMUM_TASKS            1
#define CONFIGURE_MAXIMUM_TIMERS        2

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION
#define CONFIGURE_MICROSECONDS_PER_TICK 12

#define CONFIGURE_EXTRA_TASK_STACKS         (3 * RTEMS_MINIMUM_STACK_SIZE)
#define CONFIGURE_INIT
#include <rtems/confdefs.h>
