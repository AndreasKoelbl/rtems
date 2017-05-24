/*
 * Copyright (c) OTH Regensburg, 2017
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifndef LIBBSP_ARM_JETSONTK1_CMDLINE_H
#define LIBBSP_ARM_JETSONTK1_CMDLINE_H

#include <bspopts.h>

#ifndef ASM
uint8_t bsp_get_boot_cpu(void);
#endif

#define BSP_CMDLINE_LOCATION ( TEGRA_BASE + MEMORY_SIZE - 4096 )

#endif /* LIBBSP_ARM_JETSONTK1_CMDLINE_H */
