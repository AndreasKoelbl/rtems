/**
 * @file
 *
 * @ingroup arm_start
 *
 * @brief Raspberry pi startup code.
 */

/*
 * Copyright (c) 2013 by Alan Cudmore
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *
 *  http://www.rtems.org/license/LICENSE
 */

#include <bsp/irq-generic.h>

void bsp_start(void)
{
    bsp_interrupt_initialize();
}
