struct task {
  const char        name[4];
  const char        *tty;
  const int         prio;
  rtems_task_entry  entry_point;

  rtems_id id;
};

/* avg is stored as uint64_t to achieve more than a 15 day measurement period */
struct measure_data {
  struct timespec min;
  struct timespec max;
  struct timespec current;
  uint64_t jitters_secs;
  uint64_t jitters_nsecs;
  uint32_t iterations;
};

void worker_task_entry(rtems_task_argument unused);
rtems_status_code spawn(struct task *task);
rtems_status_code spawn(struct task *task)

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
    if (status) { \
    perror(#msg); \
  }

#define WORKER_TASK(NAME, TTY, PRIO) \
  { \
    .name = NAME, \
    .tty = TTY, \
    .prio = PRIO, \
    .id = -1, \
    .entry_point = worker_task_entry, \
  }

#define NUM_TICKS 1
#define WAIT_NS   (CONFIGURE_MICROSECONDS_PER_TICK * NUM_TICKS)

#define DEV1 "/dev/ttyS0"


#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 20
#define CONFIGURE_MAXIMUM_TASKS                  50
#define CONFIGURE_MAXIMUM_TIMERS                 50
#define CONFIGURE_MICROSECONDS_PER_TICK          100000
#define CONFIGURE_MAXIMUM_POSIX_MESSAGE_QUEUES   10

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_MAXIMUM_SEMAPHORES             20

#define CONFIGURE_EXTRA_TASK_STACKS              (10 * RTEMS_MINIMUM_STACK_SIZE)
//#define CONFIGURE_SCHEDULER_EDF

#define CONFIGURE_INIT

#include <rtems/confdefs.h>
