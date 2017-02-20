/*
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rtems.h>
#include <rtems/score/todimpl.h>
#include <rtems/timespec.h>

rtems_task Init(rtems_task_argument argument);
void handle_timer(rtems_id id, void* data);

#define rtems_directive_failed(status, msg) \
  if (status != RTEMS_SUCCESSFUL) { \
    printf("\n%s FAILED -- expected (%s) got (%s)\n", msg, \
      rtems_status_text(RTEMS_SUCCESSFUL), rtems_status_text(status)); \
    fflush(stdout); \
  }

#define posix_directive_failed(status, msg) \
  if (status) { \
    printf("%s: %s\n", msg, strerror(status)); \
    fflush(stdout); \
  }

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_MICROSECONDS_PER_TICK         10000
#define CONFIGURE_MAXIMUM_TASKS                 5
#define CONFIGURE_MAXIMUM_TIMERS                1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXTRA_TASK_STACKS             (10 * RTEMS_MINIMUM_STACK_SIZE)

#include <rtems/confdefs.h>
