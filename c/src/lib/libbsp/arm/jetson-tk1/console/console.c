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
} jetsontk1_uart_context;

static jetsontk1_uart_context jetsontk1_uart_instances[] = {
  {
    .regs = UART0,
  }
};

static uint32_t jetsontk1_driver_console_in(rtems_termios_device_context *base)
{
  //Read the UART.LSR register to clear interrupts
  mmio_write32(UART0 + UART_LCR_DLAB, 0);
  return mmio_read32(UART0 + UART_RBR);
}

static uint8_t is_busy(void)
{
	return !(mmio_read32(UART0 + UART_LSR) & UART_LSR_THRE);
}

void writeChar(char out)
{
  mmio_write32(UART0 + UART_TX, out);
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
    if (c == '\n') {
      c = '\r';
    } else {
      c = buf[i];
    }
    while (is_busy()) {
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

static bool jetsontk1_driver_first_open(
  rtems_termios_tty             *tty,
  rtems_termios_device_context  *base,
  struct termios                *term,
  rtems_libio_open_close_args_t *args
)
{
  jetsontk1_driver_context *ctx = (jetsontk1_driver_context *) base;
  rtems_termios_set_initial_baud(tty, JETSONTK1_BAUD_RATE);

  return true;
}

static void jetsontk1_driver_last_close(
  rtems_termios_tty             *tty,
  rtems_termios_device_context  *base,
  rtems_libio_open_close_args_t *args
)
{
}

static uint32_t jetsontk1_driver_poll_read(rtems_termios_device_context *base)
{
  jetsontk1_driver_context *ctx = (jetsontk1_driver_context *) base;

	return jetsontk1_driver_console_in(base);
}

static uint32_t jetsontk1_driver_interrupt_read(
  rtems_termios_device_context *base
)
{
  jetsontk1_driver_context *ctx = (jetsontk1_driver_context *) base;

  uint32_t result = mmio_read32(UART0 + UART_RBR);
  //Read the UART.LSR register to clear interrupts
  mmio_write32(UART0 + UART_LCR_DLAB, 0);
  return result;
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
  rtems_status_code status;

  rtems_termios_initialize();

  /* Setup the divider */
  mmio_write32( UART0 + UART_LCR, UART_LCR_DLAB);
	mmio_write32( UART0  + UART_DLL, 25459200 / JETSONTK1_BAUD_RATE);
	mmio_write32( UART0  + UART_DLM, 0);
	mmio_write32( UART0  + UART_LCR, UART_LCR_8N1);

  status = rtems_interrupt_handler_install(
    0x0,
    "Uart",
    RTEMS_INTERRUPT_UNIQUE,
    (rtems_interrupt_handler) jetsontk1_driver_interrupt_read,
    NULL
  );
  if (status != RTEMS_SUCCESSFUL) {
    printk("fatal: %p\n", (uint32_t*)jetsontk1_driver_interrupt_read);
    rtems_fatal_error_occurred(0xdeadbeef);
  }

  status = rtems_termios_device_install(
    "/dev/console",
    &jetsontk1_uart_handler,
    NULL,
    &jetsontk1_uart_instances[0].base
  );

  if (status != RTEMS_SUCCESSFUL) {
    rtems_fatal_error_occurred(status);
  }

  return RTEMS_SUCCESSFUL;
}

BSP_polling_getchar_function_type BSP_poll_char = jetsontk1_driver_console_in;
