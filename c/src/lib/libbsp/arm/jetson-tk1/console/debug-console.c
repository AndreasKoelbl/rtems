/*
 *  COPYRIGHT (c) 1989-2011, 2017.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#include <bsp/console.h>
#include <bsp/memory.h>
#include <rtems/bspIo.h>

#define JAILHOUSE_HC_DEBUG_CONSOLE_PUTC 8
#define UART_LSR                        0x14
#define  UART_LSR_RDR                   (1<<0)

#define UART_RBR                        (0<<0)

static void jailhouse_debug_console_out(char c)
{
  register uint32_t num_res asm("r0") = JAILHOUSE_HC_DEBUG_CONSOLE_PUTC;
  /* libc toupper fails - just ASCII support */
  if ((c > 'a') && (c < 'z')) {
    c -= 0x20;
  }
	register uint32_t arg1 asm("r1") = c;

	asm volatile(
		".arch_extension virt\n\t"
		"hvc #0x4a48\n\t"
		: "=r" (num_res)
		: "r" (num_res), "r" (arg1)
		: "memory");
}

static int jetsontk1_debug_console_in(void)
{
  int result;
  while (!(mmio_read32(UARTD + UART_LSR) & UART_LSR_RDR)) {
    asm volatile("nop");
  }
  /* Read byte from UARTD of jetson-tk1 */
  result = mmio_read32(UARTD + UART_RBR);
  /* Clear interrupts */
  mmio_write32(UARTD + UART_LSR, 0);
  return result;
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
  while (start < end) {
    swap(str + start, str + end);
    start++;
    end--;
  }
}

static void myItoa(uint32_t num, char* str, uint32_t base)
{
  int i = 0;
  int rem;

  /* Handle 0 explicitely, otherwise empty string is printed for 0 */
  if (num == 0) {
    str[i++] = '0';
    str[i] = '\0';
  }

  while (num != 0) {
    rem = num % base;
    if (rem > 9) {
      str[i++] = 'A' + (rem - 10);
    } else {
      str[i++] = '0' + rem;
    }
    num /= base;
  }

  str[i] = '\0';
  reverse(str, i);
}

#ifdef JAILHOUSE_ENABLE
void jailhouse_debug_console_write(const char *text)
{
  if (text == NULL) {
    return;
  }
  do {
    jailhouse_debug_console_out(*text);
  } while (*text++);
}
#else
static void ns8250_debug_console_out(char c)
{
  while (!(mmio_read32(UARTD + UART_LSR) & UART_LSR_THRE)) {
    asm volatile("nop");
  }

  if (c == '\n') {
      mmio_write32(UARTD + UART_TX, '\r');
  }

  /* libc toupper fails - just ASCII support */
  if ((c > 'a') && (c < 'z')) {
    c -= 0x20;
  }
  mmio_write32(UARTD + UART_TX, c);
}
#endif

void print_hex(uint32_t num)
{
  uint8_t size = 0;
  char str[9];
  const char* prefix = "0x";

  myItoa(num, str, 16);

#ifdef JAILHOUSE_ENABLE
  jailhouse_debug_console_write(prefix);
  jailhouse_debug_console_write(str);
#else
  ns8250_debug_console_write(prefix);
  ns8250_debug_console_write(str);
#endif
}

#ifdef JAILHOUSE_ENABLE
BSP_output_char_function_type BSP_output_char = jailhouse_debug_console_out;
BSP_polling_getchar_function_type BSP_poll_char = jetsontk1_debug_console_in;
#else
BSP_output_char_function_type BSP_output_char = ns8250_debug_console_out;
BSP_polling_getchar_function_type BSP_poll_char = jetsontk1_debug_console_in;
#endif
