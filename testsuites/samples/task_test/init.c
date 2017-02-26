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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>

#include <rtems/printer.h>
#include <rtems/timespec.h>
#include <rtems/libio.h>

#include <tmacros.h>

#include <rtems.h>

#include "system.h"

#define NUM_TICKS     10
#define WAIT_NS       (CONFIGURE_MICROSECONDS_PER_TICK * NUM_TICKS)
#define NSECS_PER_SEC 1000000000

#define MSGQ_TYPE (struct timespec*)

static struct task workers[] = {
  WORKER_TASK("TSK1", 8),
/*
  WORKER_TASK("TSK2", 8),
  WORKER_TASK("TSK3", 8),
  WORKER_TASK("TSK4", 8),
  WORKER_TASK("TSK5", 8),
  WORKER_TASK("TSK6", 8),
  WORKER_TASK("TSK7", 8),
  WORKER_TASK("TSK8", 8),
  */
};

rtems_status_code spawn(struct task *task, rtems_task_argument arg)
{
  rtems_status_code status;

  status = rtems_task_create(rtems_build_name(task->name[0], task->name[1],
                               task->name[2], task->name[3]),
             task->prio,
             RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
             RTEMS_FLOATING_POINT | RTEMS_DEFAULT_ATTRIBUTES,
             &task->id);
  rtems_directive_failed(status, "task_create");

  status = rtems_task_start(task->id, worker_task_entry, arg);
  rtems_directive_failed(status, "task_start");
  return status;
}

void calc_results(struct measure_data *result, struct timespec *measurement)
{
  struct timespec exp_diff;

  ++result->iterations;
  exp_diff.tv_sec = labs(measurement->tv_sec);
  exp_diff.tv_nsec = labs(measurement->tv_nsec);

  result->sec_jitters += exp_diff.tv_sec;
  result->nsec_jitters += exp_diff.tv_nsec;

  if (rtems_timespec_greater_than(&result->max, &exp_diff)) {
    printf("greater\n");
    memcpy(&result->max, &exp_diff, sizeof(struct timespec));
  }
  if (rtems_timespec_less_than(&result->min, &exp_diff)) {
    printf("lesser\n");
    memcpy(&result->min, &exp_diff, sizeof(struct timespec));
  }
}

void print_results(struct measure_data *result, struct timespec *measurement,
                   int worker)
{
  /*
  printf("id: %d "
         "curr: %ld:%ld "
         "min: %ld:%ld "
         "max: %ld:%ld "
         "avg: %llu\n",
         worker,
         measurement->tv_sec % 1000, measurement->tv_nsec,
         result->min.tv_sec % 1000, result->min.tv_nsec,
         result->max.tv_sec % 1000, result->max.tv_nsec,
         ((long long unsigned) result->sec_jitters * NSECS_PER_SEC +
         result->nsec_jitters) / result->iterations);
         */
  printf("curr: %ld:%ld\n", measurement->tv_sec % 1000, measurement->tv_nsec);
}

  //jitter = (diff.tv_nsec + diff.tv_sec * 1000000000) - WAIT_NS;

rtems_task Init(rtems_task_argument ignored)
{
  int i, pstatus = 0;
  unsigned int prio;
  char buf[32];
  struct timespec recv_buf;
  struct task *task;
  struct mq_attr msgq_attr;
  struct measure_data results[RTEMS_ARRAY_SIZE(workers)];

  rtems_status_code cstatus = 0;
  rtems_task_priority current_priority;
  rtems_id scheduler;

  mqd_t worker_queues[RTEMS_ARRAY_SIZE(workers)];

  memset(&recv_buf, 0, sizeof(recv_buf));
  printf("Initializing Queue\n");

  msgq_attr.mq_msgsize = sizeof(struct timespec);
  msgq_attr.mq_maxmsg = 128;

  printf("Opening Queues...\n");
  for (i = 0; i < RTEMS_ARRAY_SIZE(worker_queues); i++) {
    snprintf(buf, sizeof(buf), "/work_queue%d", i);
    worker_queues[i] = mq_open(buf, O_RDWR | O_CREAT | O_NONBLOCK | O_EXCL |
        S_IRWXU | S_IRWXG, 0x777, &msgq_attr);
    posix_directive_failed(worker_queues[i], "mq_open");

    pstatus = mq_getattr(worker_queues[i], &msgq_attr);
    posix_directive_failed(pstatus, "mq_getattr");
  }

  for (i = 0; i < RTEMS_ARRAY_SIZE(results); i++) {
    memset(&results[i], 0, sizeof(results[0]));
    results[i].min.tv_sec = LONG_MAX;
    results[i].min.tv_nsec = LONG_MAX;
  }

  printf("Initialising Tasks...\n");
  for (i = 0; i < RTEMS_ARRAY_SIZE(workers); i++) {
    task = workers + i;
    cstatus = spawn(task, (rtems_task_argument)&worker_queues[i]);
    if (cstatus)
      printk("error spawning task %d\n", i);
  }

  cstatus = rtems_task_set_priority(rtems_task_self(), 128, &current_priority);
  rtems_directive_failed(cstatus, "set_priority");

  cstatus = rtems_task_get_scheduler(rtems_task_self(), &scheduler);
  rtems_directive_failed(cstatus, "get_scheduler");

  cstatus = rtems_task_get_priority(rtems_task_self(), scheduler, &current_priority);
  rtems_directive_failed(cstatus, "get_priority");
  printf("curr prio: %lu\n", current_priority);

  while (true) {
    for (i = 0; i < RTEMS_ARRAY_SIZE(workers); i++) {
      //mq_notify(worker_queues[i], NULL);
      pstatus = mq_receive(worker_queues[i], (char*)&recv_buf,
                           sizeof(recv_buf), &prio);
      if (pstatus != -1 && pstatus != 0xff) {
        calc_results(&results[i], &recv_buf);
        print_results(&results[i], &recv_buf, i);
        /*
        printf("Received message (%d bytes) with prio %u from %d: %ld:%ld\n",
            pstatus, prio, i, recv_buf.tv_sec % 1000, recv_buf.tv_nsec);
        fflush(stdout);
        */
      }
    }
    rtems_task_wake_after(NUM_TICKS * 2);
  }
  for (i = 0; i < RTEMS_ARRAY_SIZE(worker_queues); i++) {
    mq_close(worker_queues[i]);
  }

  cstatus = rtems_task_delete(rtems_task_self());
  rtems_directive_failed(cstatus, "rtems_task_delete");
}

void worker_task_entry(rtems_task_argument queue)
{
  struct timespec start, end, diff;
  rtems_status_code status;
  struct timespec interval;
  interval.tv_sec = 0;
  interval.tv_nsec = WAIT_NS;

  while(true) {
    status = clock_gettime(CLOCK_REALTIME, &start);
    posix_directive_failed(status, "clock_gettime");
    //rtems_task_wake_after(NUM_TICKS);
    nanosleep(&interval, NULL);
    status = clock_gettime(CLOCK_REALTIME, &end);
    posix_directive_failed(status, "clock_gettime");

    rtems_timespec_subtract(&start, &end, &diff);
    if (diff.tv_sec < 0 || diff.tv_nsec < 0)
      fprintf(stderr, "Time difference is negative\n");
    diff.tv_nsec -= WAIT_NS;

    //posix_directive_failed(status, "mq_send");
    status = mq_send(*((mqd_t *)queue), (const char*)&diff, sizeof(diff),
                     RTEMS_CURRENT_PRIORITY);
    while (status) {
      nanosleep(&interval, NULL);
      status = mq_send(*((mqd_t *)queue), (const char*)&diff, sizeof(diff),
                       RTEMS_CURRENT_PRIORITY);
    }
    if (status) {
      perror("mq_send ");
      printf("with ret %d\n", status);
    }
    //printf("%ld:%ld\n", end.tv_sec, end.tv_nsec);
    fflush(stdout);
  }
  rtems_task_delete(rtems_task_self());
}
