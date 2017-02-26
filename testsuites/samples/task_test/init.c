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

mqd_t worker_queue;

static struct task workers[] = {
  WORKER_TASK("TSK1", 8),
  WORKER_TASK("TSK2", 8),
  WORKER_TASK("TSK3", 8),
  WORKER_TASK("TSK4", 8),
  WORKER_TASK("TSK5", 8),
  WORKER_TASK("TSK6", 8),
  WORKER_TASK("TSK7", 8),
  WORKER_TASK("TSK8", 8),
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

rtems_status_code calc_jitter(struct measure_data *data)
{
  char jitter_info[255];
  uint32_t iterations = 1;

  //jitter = (diff.tv_nsec + diff.tv_sec * 1000000000) - WAIT_NS;
}
rtems_task Init(rtems_task_argument ignored)
{
  rtems_status_code status;
  int i;
  unsigned int sender;
  struct timespec recv_buf;
  rtems_task_priority current_priority;
  struct task *task;
  struct mq_attr msgq_attr;

  memset(&recv_buf, 0, sizeof(recv_buf));
  printf("Initializing Queue\n");

  msgq_attr.mq_msgsize = 255;
  msgq_attr.mq_maxmsg = 32;

  worker_queue = mq_open("/work_queue", O_RDWR | O_CREAT | O_NONBLOCK | O_EXCL |
                         S_IRWXU | S_IRWXG, 0x777, &msgq_attr);
  posix_directive_failed(worker_queue, "mq_open");

  status = mq_getattr(worker_queue, &msgq_attr);
  posix_directive_failed(status, "mq_getattr");

  printf("Queue \"%s\":\n\t- stores at most %ld messages\n\t- large at most %ld"
         " bytes each\n\t- currently holds %ld messages\n", "/work_queue",
         msgq_attr.mq_maxmsg, msgq_attr.mq_msgsize, msgq_attr.mq_curmsgs);

  printf("Initialising Tasks...\n");
  for (i = 0; i < RTEMS_ARRAY_SIZE(workers); i++) {
    task = workers + i;
    status = spawn(task);
    if (status) {
      printk("error spawning task %d\n", i);
    }
  }

  status = rtems_task_set_priority(RTEMS_SELF, 128, &current_priority);
  rtems_directive_failed(status, "set_priority");
  printf("curr prio: %lu\n", current_priority);
  while (true) {
    //mq_notify(worker_queue, NULL);
    status = mq_receive(worker_queue, (const char*)&recv_buf, sizeof(recv_buf),
                        &sender);
    if (status != -1 && status != 0xff) {
      printf("Received message (%d bytes) from %u: %ld:%ld\n", status, sender,
             recv_buf.tv_sec % 1000, recv_buf.tv_nsec);
      fflush(stdout);
      /* TODO figure out sched id */
      status = rtems_task_set_priority(RTEMS_SELF, 128, &current_priority);
      rtems_directive_failed(status, "set_priority");
      printf("curr prio: %lu\n", current_priority);
    } else {
      rtems_task_wake_after(NUM_TICKS);
    }
  }
  mq_close(worker_queue);
  status = rtems_task_delete(RTEMS_SELF);
  rtems_directive_failed(status, "rtems_task_delete");
}

void worker_task_entry(rtems_task_argument unused)
{
  struct timespec start, end, diff;
  rtems_status_code status;

  while (true) {
    status = clock_gettime(CLOCK_REALTIME, &start);
    posix_directive_failed(status, "clock_gettime");
    rtems_task_wake_after(NUM_TICKS);
    //clock_nanosleep(CLOCK_REALTIME, 0, &interval, NULL);
    status = clock_gettime(CLOCK_REALTIME, &end);
    posix_directive_failed(status, "clock_gettime");

    rtems_timespec_subtract(&start, &end, &diff);
    if (diff.tv_sec < 0 || diff.tv_nsec < 0)
      fprintf(stderr, "Time difference is negative\n");

    status = mq_send(worker_queue, (const char*)&diff, sizeof(diff),
                     RTEMS_CURRENT_PRIORITY);
    posix_directive_failed(status, "mq_send");
    if (status)
      perror("mq_send\n");
    //printf("%ld:%ld\n", end.tv_sec, end.tv_nsec);
    fflush(stdout);
  }
  rtems_task_delete(RTEMS_SELF);
}
