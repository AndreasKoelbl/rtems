/*
 * Copyright (c) OTH Regensburg, 2017
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#include <bsp.h>
#include <bsp/cmdline.h>

#define ARM_MAX_CPU_COUNT 4

#define DEFAULT_BOOT_CPU (1 << 0)

rtems_status_code bsp_get_boot_cpu( void ) {
  char boot_cpu_str[32];
  char c;

  if (rtems_bsp_cmdline_get_param_rhs("boot_cpu", boot_cpu_str,
                                      sizeof(boot_cpu_str)) == NULL) {
    printk("No boot cpu argument provided %lx\n", BSP_CMDLINE_LOCATION);
    return DEFAULT_BOOT_CPU;
  }

  c = boot_cpu_str[0];
  if (c >= '0' && c <= '3')
    return c - '0';

  printk("Warning: Invalid boot CPU '%c'\n", c == 0 ? '?' : c);

  return DEFAULT_BOOT_CPU;
}
