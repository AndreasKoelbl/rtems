/**
 * @file
 *
 * @ingroup arm_start
 *
 * @brief This routine starts the application.  It includes application,
 *  board, and monitor specific initialization and configuration.
 *  The generic CPU dependent initialization has been performed
 *  before this routine is invoked.
 */

/*
 * Copyright (c) OTH Regensburg, 2016
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#include <bsp.h>
#include <rtems.h>
#include <bsp/start.h>

/* FIXME: do something productive here */
BSP_START_TEXT_SECTION __attribute__((flatten)) void bsp_reset(void)
{
  rtems_interrupt_level level;

  rtems_interrupt_local_disable(level);
  (void) level;
}
