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
rtems_id     Timer2;
/* forward declarations to avoid warnings */
rtems_task Init(rtems_task_argument argument);

const char rtems_test_name[] = "HELLO WORLD";
rtems_printer rtems_test_printer;
uint8_t value = 0;

rtems_timer_service_routine Timer_Routine( rtems_id id, void *ignored )
{
  rtems_status_code status;

  if ( id == Timer1 )
    value = 1;
  else
    value = 2;

  status = rtems_timer_fire_after(
    id,
    2 * rtems_clock_get_ticks_per_second(),
    Timer_Routine,
    NULL
  );
}

rtems_task Init(
  rtems_task_argument ignored
)
{
  rtems_status_code status;
  rtems_print_printer_printf(&rtems_test_printer);

  printf( "\n\n*** LED BLINKER -- timer ***" );

  status = rtems_timer_create(rtems_build_name( 'T', 'M', 'R', '1' ), &Timer1);
  if ( status != RTEMS_SUCCESSFUL )
    fputs( "Timer1 create failed\n", stderr );

  status = rtems_timer_create(rtems_build_name( 'T', 'M', 'R', '2' ), &Timer2);
  if ( status != RTEMS_SUCCESSFUL )
    fputs( "Timer2 create failed\n", stderr );

  Timer_Routine(Timer1, NULL);
  printf("Ex-Routine1 value: %d", value);

  status = rtems_task_wake_after( rtems_clock_get_ticks_per_second() );

  Timer_Routine(Timer2, NULL);
  printf("Ex-Routine2 value: %d", value);

  while (1) {
    status = rtems_task_wake_after( 10 );
    printf("Ex-Routine3 value: %d", value);
  }

  status = rtems_task_delete( RTEMS_SELF );
}


/**************** START OF CONFIGURATION INFORMATION ****************/

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS         1
#define CONFIGURE_MAXIMUM_TIMERS        2

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXTRA_TASK_STACKS         (3 * RTEMS_MINIMUM_STACK_SIZE)

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
/****************  END OF CONFIGURATION INFORMATION  ****************/
