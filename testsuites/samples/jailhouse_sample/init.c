/*
 * Copyright (c) OTH Regensburg, 2017
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/printer.h>

#include <bsp.h>

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

rtems_task Init(rtems_task_argument argument);
rtems_timer_service_routine Timer_Routine( rtems_id id, void *ignored );

rtems_id Timer1;
static struct timespec current, old;


void handle_IRQ(rtems_id id, void* data);

rtems_printer rtems_test_printer;

rtems_task Init(rtems_task_argument ignored)
{
  rtems_status_code status;

  rtems_print_printer_printf(&rtems_test_printer);

  status = rtems_timer_create(rtems_build_name( 'T', 'M', 'R', '1' ), &Timer1);
  if ( status != RTEMS_SUCCESSFUL )
  {
    fprintf(stderr, "Timer1 create failed\n");
    exit( 0 );
  }

  status = clock_gettime(CLOCK_REALTIME, &old);
  status = rtems_timer_fire_after(Timer1, 1, handle_IRQ, NULL);
  if ( status != RTEMS_SUCCESSFUL )
  {
    fprintf(stderr, "Timer1 fire failed\n");
    exit( 0 );
  }

  while (1) {
    asm volatile("wfi" : : : "memory");
  }

  rtems_test_end();
  rtems_task_delete( RTEMS_SELF );
}

void handle_IRQ(rtems_id id, void* data)
{
  uint32_t currentTime = 0;
  uint32_t num = 0;
  char buf[32];
  struct timespec diff;
  static struct timespec minDelta =
  {
    .tv_sec = INT32_MAX,
    .tv_nsec = INT32_MAX,
  };

  static struct timespec maxDelta =
  {
    .tv_sec = INT32_MIN,
    .tv_nsec = INT32_MIN,
  };

  rtems_status_code status = clock_gettime(CLOCK_REALTIME, &current);

  if (current.tv_sec < old.tv_sec) {
    fprintf(stderr, "Time difference is negative\n");
  }

  diff.tv_sec = current.tv_sec - old.tv_sec;

  if (diff.tv_sec > 0) {
    diff.tv_nsec = 1000000000 - old.tv_nsec + current.tv_nsec - 100000000;
  } else {
    diff.tv_nsec = current.tv_nsec - old.tv_nsec - 100000000;
  }


  printf("Current: %ld\t" \
         "Old: %ld\t" \
         "Jitter: %ld\n" \
         , current.tv_nsec, old.tv_nsec, diff.tv_nsec);
  old = current;
  //memset(buf, '\0', sizeof(buf));
  printf("Please insert num: ");
  //fgets(buf, sizeof(buf), stdin);
  printf("num: %s", buf);

  /* Interrupt me every 1/10 second */
  rtems_timer_fire_after(id, 1, handle_IRQ, NULL);
}

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_MAXIMUM_TASKS             1
#define CONFIGURE_MAXIMUM_TIMERS            1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_MICROSECONDS_PER_TICK 100000

#define CONFIGURE_EXTRA_TASK_STACKS         (3 * RTEMS_MINIMUM_STACK_SIZE)
#define CONFIGURE_INIT
#include <rtems/confdefs.h>
