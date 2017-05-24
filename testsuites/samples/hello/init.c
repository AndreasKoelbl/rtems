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
#include <bsp/bootcard.h>
#include <bsp/cmdline.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 4096

/* forward declarations to avoid warnings */
rtems_task Init(rtems_task_argument argument);

const char rtems_test_name[] = "HELLO WORLD";
rtems_printer rtems_test_printer;

static void *pool[64000];

#define MALLOC_SIZE (PAGE_SIZE / 2)

rtems_task Init(
  rtems_task_argument ignored
)
{
  unsigned int i = 0;
  void *mem;

  rtems_print_printer_printf(&rtems_test_printer);
  rtems_test_begin();
  printf( "Hello World\n" );
  printf( "Pointer to commandlines: %p\n", bsp_boot_cmdline);
  printf( "cmdline: %s\n", bsp_boot_cmdline);
  printf( "Begin of memtest\n");

  printf("Allocating...\n");
  for (;;) {
    mem = malloc(MALLOC_SIZE);
    if (!mem)
      break;
    memset(mem, 0xaa, MALLOC_SIZE);
    pool[i++] = mem;
  }
  printf("Done allocating %u segments\n", i);

  printf("Freeing...\n");
  for (i = 0; pool[i]; i++) {
    free(pool[i]);
  }
  printf("Done.\n");

  rtems_test_end();
  exit( 0 );
}


/* NOTICE: the clock driver is explicitly disabled */
#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_MAXIMUM_TASKS            1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
