/*
 * Copyright (c) 2016 embedded brains GmbH Huber.  All rights reserved.
 *
 *  embedded brains GmbH
 *  Dornierstr. 4
 *  82178 Puchheim
 *  Germany
 *  <rtems@embedded-brains.de>
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#include <bsp.h>
#include <bsp/start.h>

inline void debugPrint(void)
{
	register uint32_t num_res asm("r0") = 8;
	register uint32_t arg1 asm("r1") = '\0';

	char *filename = __FILE__;
  char *line;
  char tmp;

  arg1 = 'X';
    asm volatile(
      ".arch_extension virt\n\t"
      "hvc #0x4a48\n\t"
      : "=r" (num_res)
      : "r" (num_res), "r" (arg1)
      : "memory");

	for (char *ptr = filename; *ptr++; )
	{
    arg1 = *ptr;
    asm volatile(
      ".arch_extension virt\n\t"
      "hvc #0x4a48\n\t"
      : "=r" (num_res)
      : "r" (num_res), "r" (arg1)
      : "memory");
  }
  line = itoa(__LINE__);
	for (char *ptr = line; *ptr++; )
	{
    arg1 = *ptr;
    asm volatile(
      ".arch_extension virt\n\t"
      "hvc #0x4a48\n\t"
      : "=r" (num_res)
      : "r" (num_res), "r" (arg1)
      : "memory");
  }
}

inline void printViaHypervisor(char* string)
{
  //lots of stuff
}

void BSP_START_TEXT_SECTION bsp_start_hook_0(void)
{
  //debugPrint();
}

void BSP_START_TEXT_SECTION bsp_start_hook_1(void)
{
}
