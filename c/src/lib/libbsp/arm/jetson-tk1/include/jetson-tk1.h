/*
 * Copyright (c) OTH Regensburg, 2016
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifndef LIBBSP_ARM_JETSONTK1_H
#define LIBBSP_ARM_JETSONTK1_H

#include <bspopts.h>
#include <stdint.h>
#include <rtems/termiostypes.h>

#define JAILHOUSE_ENABLE 1

static inline void cpu_relax(void)
{
	asm volatile("" : : : "memory");
}

#endif /* LIBBSP_ARM_JETSONTK1_JETSONTK1_H */
