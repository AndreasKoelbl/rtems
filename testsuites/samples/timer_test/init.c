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

#define CONFIGURE_INIT
#include "system.h"

#define NUM_TICKS       1
#define EXPECTED_NSECS  (CONFIGURE_MICROSECONDS_PER_TICK * NUM_TICKS)

/* avg is stored as uint64_t to achieve more than a 15 day measurement period */
struct measure_data {
  struct timespec min;
  struct timespec max;
  struct timespec last;
  uint64_t jitters_secs;
  uint64_t jitters_nsecs;
  uint32_t iterations;
};

static rtems_id timer;
static struct timespec expected_time;

rtems_task Init(
  rtems_task_argument argument
)
{
  rtems_status_code status;
  struct measure_data *measurement = (struct measure_data*)
    malloc(sizeof(struct measure_data));

  expected_time.tv_sec = 0;
  expected_time.tv_nsec = EXPECTED_NSECS;
  memset(measurement, 0, sizeof(struct measure_data));
  rtems_timespec_set(&measurement->min, INT_MAX, LONG_MAX);
  measurement->iterations = 2;

  status = rtems_timer_create(rtems_build_name( 'T', 'M', 'R', '1' ), &timer);
  rtems_directive_failed(status, "timer create");
  status = clock_gettime(CLOCK_REALTIME, &measurement->last);
  posix_directive_failed(status, "clock_gettime");

  status = rtems_timer_fire_after(timer, NUM_TICKS, handle_timer, measurement);
  rtems_directive_failed(status, "timer fire");

  //rtems_task_suspend(RTEMS_SELF);
  rtems_task_suspend(rtems_task_self());
  free(measurement);

  rtems_task_delete(RTEMS_SELF);

}

void handle_timer(rtems_id id, void* data)
{
  struct timespec current;
  rtems_status_code status;

  status = clock_gettime(CLOCK_REALTIME, &current);
  posix_directive_failed(status, "clock_gettime");

  printf("%ld:%ld\n", current.tv_sec % 10, current.tv_nsec);

  posix_directive_failed(status, "clock_gettime");
  /* Interrupt me every 1/10 second */
  rtems_timer_fire_after(id, NUM_TICKS, handle_timer, NULL);
}
