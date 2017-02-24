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

struct task {
  const char        name[4];
  const char        *tty;
  const int         prio;
  rtems_task_entry  entry_point;

  rtems_id id;
};

struct worker_info {
  rtems_id id;
  uint64_t jitters;
  int64_t jitter;
  int64_t jitter_min, jitter_max;
};

static void worker_task_entry(rtems_task_argument unused);
rtems_status_code spawn(struct task *task);

/* Soft error handling */
/* RTEMS Classic API, no posix */
#undef directive_failed
#define rtems_directive_failed(status, msg) \
  if (status != RTEMS_SUCCESSFUL) { \
    printf("\n%s FAILED -- expected (%s) got (%s)\n", msg, \
           rtems_status_text(RTEMS_SUCCESSFUL), rtems_status_text(status)); \
    fflush(stdout); \
  }

#define posix_directive_failed(status, msg) \
if (status != 0) \
  printf("%s: %s", msg, strerror(status))

#define WORKER_TASK(NAME, TTY, PRIO) \
  { \
    .name = NAME, \
    .tty = TTY, \
    .prio = PRIO, \
    .id = -1, \
    .entry_point = worker_task_entry, \
  }

#define WAIT_NS (CONFIGURE_MICROSECONDS_PER_TICK * 1000)

#define DEV1 "/dev/ttyS0"

mqd_t worker_queue;

static struct task my_tasks[] = {
  WORKER_TASK("TSK1", DEV1, 8),
  WORKER_TASK("TSK2", DEV1, 8),
  WORKER_TASK("TSK3", DEV1, 8),
  WORKER_TASK("TSK4", DEV1, 8),
  WORKER_TASK("TSK5", DEV1, 8),
  WORKER_TASK("TSK6", DEV1, 8),
  WORKER_TASK("TSK7", DEV1, 8),
  WORKER_TASK("TSK8", DEV1, 8),
};

rtems_status_code spawn(struct task *task)
{
  rtems_status_code status;

  status = rtems_task_create(rtems_build_name(task->name[0], task->name[1],
                               task->name[2], task->name[3]),
             task->prio,
             RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
             RTEMS_FLOATING_POINT | RTEMS_DEFAULT_ATTRIBUTES,
             &task->id);
  rtems_directive_failed(status, "task_create");

  status = rtems_task_start(task->id, worker_task_entry, 0);
  rtems_directive_failed(status, "task_start");
  return status;
}

rtems_task Init(rtems_task_argument ignored)
{
  rtems_status_code status;
  int i;
  int sender;
  char receive_buf[80];
  const char *test_str = "this is a test";
  rtems_task_priority current_priority;
  struct task *task;
  struct mq_attr msgq_attr;

  memset(receive_buf, 0, RTEMS_ARRAY_SIZE(receive_buf));
  printf("Initializing Queue\n");
  worker_queue = mq_open("/work_queue", O_RDWR | O_CREAT | O_EXCL, S_IRWXU |
                         S_IRWXG, NULL);
  posix_directive_failed(worker_queue, "mq_open");
  status = mq_getattr(worker_queue, &msgq_attr);

  posix_directive_failed(worker_queue, "mq_getatt");
  printf("Queue \"%s\":\n\t- stores at most %ld messages\n\t- large at most %ld"
         "bytes each\n\t- currently holds %ld messages\n", "/work_queue",
         msgq_attr.mq_maxmsg, msgq_attr.mq_msgsize, msgq_attr.mq_curmsgs);

  printf("Initialising Tasks...\n");
  for (i = 0; i < RTEMS_ARRAY_SIZE(my_tasks); i++) {
    task = my_tasks + i;
    status = spawn(task);
    if (status) {
      printk("error spawning task %d\n", i);
    }
  }

  status = rtems_task_set_priority(RTEMS_SELF, 128, &current_priority);
  rtems_directive_failed(status, "set_priority");
  printf("curr prio: %lu\n", current_priority);
  status = rtems_task_set_priority(RTEMS_SELF, 128, &current_priority);
  rtems_directive_failed(status, "set_priority");
  printf("curr prio: %lu\n", current_priority);
  while (true) {
    status = mq_send(worker_queue, test_str, strlen(test_str),
            RTEMS_CURRENT_PRIORITY);
    posix_directive_failed(status, "mq_send");
    status = mq_receive(worker_queue, receive_buf,
                        RTEMS_ARRAY_SIZE(receive_buf), &sender);
    /*if (status != 0xff) */{
      printf("Received message (%d bytes) from %d: %s\n", status, sender,
             receive_buf);
      fflush(stdout);
    }
  }
  mq_close(worker_queue);
  status = rtems_task_delete(RTEMS_SELF);
  rtems_directive_failed(status, "rtems_task_delete");
}

static void worker_task_entry(rtems_task_argument worker_info)
{
  char jitter_info[80];
  uint32_t iterations = 1;
  uint64_t jitters;
  int64_t jitter, jitter_min, jitter_max;
  struct timespec start, end, diff, interval;
  rtems_status_code status;

  jitter_min = LONG_MAX;
  jitter_max = LONG_MIN;
  interval.tv_sec = 0;
  interval.tv_nsec = WAIT_NS / 2;

  while (true) {
    /*
    status = clock_gettime(CLOCK_REALTIME, &start);
    posix_directive_failed(status, "clock_gettime");
    */
    rtems_task_wake_after(100);
    //clock_nanosleep(CLOCK_REALTIME, 0, &interval, NULL);
    status = clock_gettime(CLOCK_REALTIME, &end);
    posix_directive_failed(status, "clock_gettime");

    /*
    rtems_timespec_subtract(&start, &end, &diff);
    if (diff.tv_sec < 0 || diff.tv_nsec < 0)
      fprintf(stderr, "Time difference is negative\n");

    jitter = (diff.tv_nsec + diff.tv_sec * 1000000000) - WAIT_NS;

    if (jitter < jitter_min)
      jitter_min = jitter;
    else if (jitter > jitter_max)
      jitter_max = jitter;

    jitters += labs(jitter);
    */
    /*
    printf("new: %ld:%ld old: %ld:%ld diff: %ld:%ld\n", start.tv_sec % 1000,
           start.tv_nsec, end.tv_sec % 1000, end.tv_nsec, diff.tv_sec,
           diff.tv_nsec);
           */
    /*
    snprintf(jitter_info, RTEMS_ARRAY_SIZE(jitter_info),
             "id: %lu "
             "Current: %lld "
             "Min: %lld "
             "Avg: %llu "
             "Max: %lld\n",
             RTEMS_SELF,
             jitter,
             jitter_min,
             jitters / iterations,
             jitter_max);
    printf("%s\n", jitter_info);
    status = mq_send(worker_queue, jitter_info, RTEMS_ARRAY_SIZE(jitter_info),
            RTEMS_CURRENT_PRIORITY);
    posix_directive_failed(status, "mq_send");
    */
    printf("%ld:%ld\n", end.tv_sec, end.tv_nsec);
    fflush(stdout);
    ++iterations;
  }
  rtems_task_delete(RTEMS_SELF);
}
