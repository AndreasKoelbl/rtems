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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <rtems/printer.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <termios.h>

#include "header.h"

#define NUM_TICKS 1

rtems_task Init(rtems_task_argument argument);
rtems_timer_service_routine Timer_Routine( rtems_id id, void *ignored );
void console_test(int fd);

rtems_id Timer1;
static struct timespec current, old;


void handle_timer(rtems_id id, void* data);

rtems_printer rtems_test_printer;

rtems_task Init(rtems_task_argument ignored)
{
  rtems_status_code status;
  char buf[32];
  int err;
	int fd;

  rtems_print_printer_printf(&rtems_test_printer);
	fd = open("/dev/ttyS1", O_RDWR | O_NOCTTY );

	if(!fd) {
		perror("open_port: Unable to open /dev/ttyS0 - ");
	}	else {
		printf("port is open.\n");
	}
	//console_test(STDOUT_FILENO);
	//console_test(STDIN_FILENO);
	//console_test(fd);
  dprintf(fd, "TEST\n");

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


void console_test(int fd)
{
  int err;
  struct termios attribute;
  dprintf(fd,"Hello, world!\n");

  err = tcgetattr(STDIN_FILENO, &attribute);

  if (err) {
    printf("tcgetattr\n");
  }
  attribute.c_lflag &= ~(ECHO);

  err = cfsetispeed(&attribute, B115200);
  if (err) {
    perror("setiBaud 115200");
  }
  err = cfsetospeed(&attribute, B115200);
  if (err) {
    perror("setBaud 115200");
  }
  err = tcsetattr(fd, TCSANOW, &attribute);
  if (err) {
    perror("tcsetattr");
    rtems_task_delete( RTEMS_SELF );
  }
}
