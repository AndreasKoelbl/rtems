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

#include <bsp.h>

/* move to header */
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_MAXIMUM_TASKS             1
#define CONFIGURE_MAXIMUM_TIMERS            1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_MICROSECONDS_PER_TICK 100000

#define CONFIGURE_EXTRA_TASK_STACKS         (3 * RTEMS_MINIMUM_STACK_SIZE)
#define CONFIGURE_INIT

#include <rtems/confdefs.h>
/* ! move to header */

#include <rtems/printer.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <termios.h>

#include "header.h"

#define NUM_TICKS 1

rtems_task Init(rtems_task_argument argument);
rtems_timer_service_routine Timer_Routine( rtems_id id, void *ignored );

rtems_id Timer1;
static struct timespec current, old;


void handle_timer(rtems_id id, void* data);

rtems_printer rtems_test_printer;

rtems_task Init(rtems_task_argument ignored)
{
  rtems_status_code status;
  char buf[32];
  struct termios attribute;
  int err;

  rtems_print_printer_printf(&rtems_test_printer);
  printf("Hello, world!\n");
  printf("foo\n");

  err = tcgetattr(STDIN_FILENO, &attribute);
  if (err) {
    printf("error: tcgetattr\n");
    goto task_out;
  }

  attribute.c_lflag &= ~(ECHO);
  err = tcsetattr(STDIN_FILENO, TCSANOW, &attribute);
  if (err) {
    perror("tcsetattr");
    goto task_out;
  }

  status = rtems_timer_create(rtems_build_name( 'T', 'M', 'R', '1' ), &Timer1);
  if ( status != RTEMS_SUCCESSFUL )
  {
    fprintf(stderr, "Timer1 create failed\n");
    exit( 0 );
  }

  status = clock_gettime(CLOCK_REALTIME, &old);

  while (1) {
    printf("foo!\n");
    fflush(stdout);
    fgets(buf, sizeof(buf), stdin);
    puts(buf);
  }

  status = rtems_timer_fire_after(Timer1, NUM_TICKS, handle_timer, NULL);
  if ( status != RTEMS_SUCCESSFUL )
  {
    fprintf(stderr, "Timer1 fire failed\n");
    exit( 0 );
  }

  while (1) {
    asm volatile("wfi" : : : "memory");
  }

task_out:
  rtems_task_delete( RTEMS_SELF );
}

void handle_timer(rtems_id id, void* data)
{
  static unsigned int ctr = 0;
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
  /*
  printf("Please insert: ");
  fgets(buf, sizeof(buf), stdin);
  buf[strlen(buf) - 1] = '\0';
  scanf("%u", &num);
  printf("%u: num \n", num, buf);
  */

  old = current;
/*
  memset(buf, '\0', sizeof(buf));
  printf("lease insert string: \n");
  printf("after: \n");
  buf[strlen(buf) - 1] = '\0';
//  printf("Please insert num: ");
  scanf("%u", &num);
  printf("%u: num string: %s \n", num, buf);

*/
  /* Interrupt me every 1/10 second */
  if (++ctr < 20)
    rtems_timer_fire_after(id, NUM_TICKS, handle_timer, NULL);
}
