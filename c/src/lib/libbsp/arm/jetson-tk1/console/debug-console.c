/**
 * @file
 *
 * @ingroup arm_jetson-tk1
 *
 * @brief Jetson-TK1 console driver
 *
 */

/*
 * Copyright (c) OTH Regensburg, 2016
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#include <bsp/console.h>
#include <bsp/jetson-tk1.h>
#include <rtems/bspIo.h>
#include <rtems/sysinit.h>

#define JAILHOUSE_HC_DEBUG_CONSOLE_PUTC 8

void hypervisor_putc(char c)
{
  register uint32_t num_res asm("r0") = JAILHOUSE_HC_DEBUG_CONSOLE_PUTC;
  register uint32_t arg1 asm("r1") = c;

  asm volatile(
    ".arch_extension virt\n\t"
    "hvc #0x4a48\n\t"
    : "=r" (num_res)
    : "r" (num_res), "r" (c)
    : "memory");
}

void jetsontk1_debug_write(char* string, size_t len)
{
  for (uint32_t i = 0; i < len; i++)
  {
    hypervisor_putc(string[i]);
  }
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

static void jetsontk1_debug_console_init(void)
{
  BSP_output_char = hypervisor_putc;
}

void printHex(uint32_t num)
{
  uint8_t size = 0;
  char str[9];
  const char* prefix = "0x";
  rtems_termios_device_context debugContext;

  myItoa(num, str, 16);

  jetsontk1_debug_write(prefix, strlen(prefix) + 1);
  jetsontk1_debug_write(str, strlen(str) + 1);
}

static void jetsontk1_debug_console_early_init(char c)
{
  hypervisor_putc(c);
}

static int jetsontk1_debug_console_in(void)
{
  /* we do not read yet... */
  return (int)'J';
}

BSP_output_char_function_type BSP_output_char = jetsontk1_debug_console_early_init;

BSP_polling_getchar_function_type BSP_poll_char = jetsontk1_debug_console_in;

RTEMS_SYSINIT_ITEM(
  jetsontk1_debug_console_init,
  RTEMS_SYSINIT_BSP_START,
  RTEMS_SYSINIT_ORDER_FIRST
);
