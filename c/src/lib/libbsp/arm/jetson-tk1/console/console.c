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

#include <bsp.h>
#include <bsp/irq.h>
#include <bsp/jetson-tk1.h>
#include <bsp/fatal.h>
#include <bsp/console.h>
#include <bsp/memory.h>
#include <rtems/bspIo.h>
#include <rtems/console.h>
#include <rtems/termiostypes.h>
#include <unistd.h>

#define JAILHOUSE_HC_DEBUG_CONSOLE_PUTC 8
#define JETSONTK1_BAUD_RATE 115200

typedef struct {
  rtems_termios_device_context base;
  void *regs;
  rtems_vector_number irq;
} jetsontk1_uart_context;

static jetsontk1_uart_context jetsontk1_uart_instances[] = { {
  .regs = UART0,
  .irq = UART0_IRQn,
}
};

static uint32_t jetsontk1_driver_console_in(void)
{
  //Read the UART.LSR register to clear interrupts
  mmio_write32( UART0 + UART_LCR_DLAB, 0 );
  return mmio_read32( UART0 + UART_RBR );
}

static int is_busy(void)
{
	return !(mmio_read32(UART0 + UART_LSR) & UART_LSR_THRE);
}

void jetsontk1_driver_write(
  rtems_termios_device_context *base,
  const char                   *buf,
  size_t                        len
)
{
  size_t i;
  char c = 0;
  jetsontk1_driver_context *ctx = (jetsontk1_driver_context *) base;

	for (i = 0; i < len; i++) {
    if (c == '\n')
    {
      c = '\r';
    } else
    {
      c = buf[i];
    }
    while (is_busy())
    {
      cpu_relax();
    }

    writeChar(buf[i]);
	}
}

static bool jetsontk1_driver_set_attributes(
  rtems_termios_device_context  *base,
  const struct termios          *term
)
{
  jetsontk1_driver_context *ctx = (jetsontk1_driver_context *) base;
  /*
  * Inspect the termios data structure and configure the device
  * appropriately. The driver should only be concerned with the
  * parts of the structure that specify hardware setting for the
  * communications channel such as baud, character size, etc.
  */
  /*
  * Return true to indicate a successful set attributes, and false
  * otherwise.
  */
  return true;
}

static void uart_init()
{
	mmio_write32( UART0 + UART_LCR, UART_LCR_DLAB);
	mmio_write32( UART0  + UART_DLL, 0xdd);
	mmio_write32( UART0  + UART_DLM, 0);
	mmio_write32( UART0  + UART_LCR, UART_LCR_8N1);
}


static bool jetsontk1_driver_first_open(
  rtems_termios_tty             *tty,
  rtems_termios_device_context  *base,
  struct termios                *term,
  rtems_libio_open_close_args_t *args
)
{
  jetsontk1_driver_context *ctx = (jetsontk1_driver_context *) base;
  rtems_status_code sc;
  bool ok;

  /*
   * Sets the initial baud rate.  This should be set to the value of
   * the boot loader.  This function accepts only exact Termios baud
   * values.
   */
  sc = rtems_termios_set_initial_baud(tty, JETSONTK1_BAUD_RATE);
  if (sc != RTEMS_SUCCESSFUL) {
    /* Not a valid Termios baud */
  }
  /*
   * To propagate the initial Termios attributes to the device use
   * this.
   */
  ok = jetsontk1_driver_set_attributes(base, term);
  if (!ok) {
    return false;
  }

  /*
   * Return true to indicate a successful set attributes, and false
   * otherwise.
   */
  return true;
}

static void jetsontk1_driver_last_close(
  rtems_termios_tty             *tty,
  rtems_termios_device_context  *base,
  rtems_libio_open_close_args_t *args
)
{
  jetsontk1_driver_context *ctx = (jetsontk1_driver_context *) base;

  /*
   * The driver may do some cleanup here.
   */
}

static int jetsontk1_driver_poll_read(rtems_termios_device_context *base)
{
  jetsontk1_driver_context *ctx = (jetsontk1_driver_context *) base;

	return jetsontk1_driver_console_in();
}

static const rtems_termios_device_handler jetsontk1_uart_handler = {
  .first_open = jetsontk1_driver_first_open,
  .last_close = jetsontk1_driver_last_close,
  .poll_read = jetsontk1_driver_poll_read,
  .write = jetsontk1_driver_write,
  .set_attributes = jetsontk1_driver_set_attributes,
  .mode = TERMIOS_POLLED
};

rtems_status_code console_initialize(
  rtems_device_major_number  major,
  rtems_device_minor_number  minor,
  void                      *arg
)
{
  const char* hello = "console initialized\n";
  rtems_status_code status;

  rtems_termios_initialize();
  uart_init();

  status = rtems_termios_device_install(
    "/dev/console",
    &jetsontk1_uart_handler,
    NULL,
    &jetsontk1_uart_instances[0].base
  );

  if (status != RTEMS_SUCCESSFUL)
    rtems_fatal_error_occurred(status);

  jetsontk1_driver_write((struct rtems_termios_device_context*) arg, \
                          hello, strlen(hello) + 1);

  return RTEMS_SUCCESSFUL;
}

void printHex(uint32_t num)
{
  uint8_t size = 0;
  char str[9];
  const char* prefix = "0x";
  rtems_termios_device_context debugContext;

  myItoa(num, str, 16);

  jetsontk1_driver_write(&debugContext, prefix, strlen(prefix) + 1);
  jetsontk1_driver_write(&debugContext, str, strlen(str) + 1);
}

void writeChar(char out)
{
  rtems_termios_device_context context;

  mmio_write32( UART0 + UART_TX, out);
/*  register uint32_t num_res asm("r0") = 8;
	register uint32_t arg1 asm("r1") = out;

	asm volatile(
		".arch_extension virt\n\t"
		"hvc #0x4a48\n\t"
		: "=r" (num_res)
		: "r" (num_res), "r" (arg1)
		: "memory");
    */
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

    // Reverse the string
    reverse(str, i);
}

BSP_output_char_function_type BSP_output_char = writeChar;

BSP_polling_getchar_function_type BSP_poll_char = jetsontk1_driver_poll_read;
