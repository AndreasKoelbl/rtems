/**
 * @file
 *
 * @ingroup Jailhouse Hypervisor Debug Console
 *
 * @brief This helps creating a jailhouse hypervisor guest
 */

/*
 *  COPYRIGHT (c) 1989-2011, 2016.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#include <rtems/sysinit.h>
#include <rtems/bspIo.h>

void jailhouse_debug_console_out(char c)
{
  register uint32_t num_res asm("r0") = 8;
	register uint32_t arg1 asm("r1") = c;

	asm volatile(
		".arch_extension virt\n\t"
		"hvc #0x4a48\n\t"
		: "=r" (num_res)
		: "r" (num_res), "r" (arg1)
		: "memory");
}

static void swap(char *first, char *second)
{
  char temp = *first;
  *first = *second;
  *second = temp;
}

static void reverse(char str[], int length)
{
  int start = 0;
  int end = length -1;
  while (start < end)
  {
      swap(str + start, str + end);
      start++;
      end--;
  }
}

void myItoa(uint32_t num, char* str, uint32_t base)
{
  int i = 0;

  /* Handle 0 explicitely, otherwise empty string is printed for 0 */
  if (num == 0)
  {
    str[i++] = '0';
    str[i] = '\0';
    return str;
  }

  while (num != 0)
  {
    int rem = num % base;
    if (rem > 9)
    {
      str[i++] = 'A' + (rem - 10);
    }
    else
    {
      str[i++] = '0' + rem;
    }
    num /= base;
  }

  str[i] = '\0'; // Append string terminator

  reverse(str, i);
}

void jailhouse_debug_console_write(char *text)
{
  do
  {
    jailhouse_debug_console_out(*text);
  } while (*text++);
}

void print_hex(uint32_t num)
{
  uint8_t size = 0;
  char str[9];
  const char* prefix = "0x";

  myItoa(num, str, 16);

  jailhouse_debug_console_write(prefix);
  jailhouse_debug_console_write(str);
}

static void jailhouse_debug_console_init(void)
{
  BSP_output_char = jailhouse_debug_console_out;
}

BSP_output_char_function_type BSP_output_char;

RTEMS_SYSINIT_ITEM(
  jailhouse_debug_console_init,
  RTEMS_SYSINIT_BSP_START,
  RTEMS_SYSINIT_ORDER_LAST
);
