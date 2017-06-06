/*
 * Copyright (c) 2013 embedded brains GmbH.  All rights reserved.
 *
 *  embedded brains GmbH
 *  Obere Lagerstr. 30
 *  82178 Puchheim
 *  Germany
 *  <rtems@embedded-brains.de>
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif

#include <rtems/score/cpu.h>
#include <rtems/fatal.h>
#include <errno.h>

static int __attribute__((always_inline)) hex_dump(void *addr, int len) {
    int i;
    unsigned char buff[17];
    unsigned char *pc = (unsigned char*)addr;

    if (len <= 0) {
        return -EINVAL;
    }

    for (i = 0; i < len; i++) {

        if ((i % 16) == 0) {
            if (i != 0) {
                printk("  %s\n", buff);
            }
            printk("  %04x ", i);
        }

        printk(" %02x", pc[i]);

        if ((pc[i] < 0x20) || (pc[i] > 0x7e)) {
            buff[i % 16] = '.';
        } else {
            buff[i % 16] = pc[i];
        }
        buff[(i % 16) + 1] = '\0';
    }

    while ((i % 16) != 0) {
        printk("   ");
        i++;
    }

    printk("  %s\n", buff);
}

void _ARM_Exception_default( CPU_Exception_frame *frame )
{
  _CPU_Exception_frame_print(frame);
  register unsigned int* sp asm("sp");
  hex_dump(sp, 1024);

  rtems_fatal( RTEMS_FATAL_SOURCE_EXCEPTION, (rtems_fatal_code) frame );
}
