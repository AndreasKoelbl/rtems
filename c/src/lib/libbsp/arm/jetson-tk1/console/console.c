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

#include "../../shared/include/linker-symbols.h"

#define JETSONTK1_BAUD_RATE 115200L
#define JETSONTK1_UART_SPEED 408000000L
#define DIVIDER ( (uint32_t)(JETSONTK1_UART_SPEED / \
                            ( JETSONTK1_BAUD_RATE * 16)) )

#define UART_IIR 0x8
#define UART_MSR 0x18

#define UART_IER_IE_RHR (1<<0)
#define UART_IER_IE_THR (1<<1)

typedef struct {
  rtems_termios_device_context base;
  void *regs;
  rtems_vector_number irq;
  bool console;
  const char *device_name;
#ifdef JETSONTK1_CONSOLE_USE_INTERRUPTS
  volatile bool transmitting;
#endif
} jetsontk1_uart_context;

static void hexdump(void *base, unsigned int size)
{
  int i = -1;
  unsigned int line = 0;
  unsigned int content;

  for (i = 0; i < size/4; i++) {
    content = *(unsigned int*)(base + 4*i);
    printk("%d: %08x\n", i, content);
  }
  printk("\n");
}

static jetsontk1_uart_context jetsontk1_uart_instances[] = {
  {
    .regs = UART3,
    .irq = 90 + 32,
    .device_name = "/dev/console",
#ifdef JETSONTK1_CONSOLE_USE_INTERRUPTS
    .transmitting = false,
#endif
  }
};

static uint8_t jetsontk1_driver_is_writeable(void *base)
{
	return mmio_read32(base + UART_LSR) & UART_LSR_THRE;
}

static uint8_t jetsontk1_driver_is_readable(void *base)
{
  return mmio_read32(base + UART_LSR) & UART_LSR_RDR;
}

static void writeChar(void *base, char out)
{
  while (!jetsontk1_driver_is_writeable(base)) {
    cpu_relax();
  }

  if (out == '\n') {
    mmio_write32(base + UART_TX, '\r');
  }

  mmio_write32(base + UART_TX, out);
}


static void jetsontk1_driver_write(
  rtems_termios_device_context *context,
  const char                   *buf,
  size_t                        len
)
{
  size_t i;
  jetsontk1_uart_context *ctx = (jetsontk1_uart_context *) context;

#ifdef JETSONTK1_CONSOLE_USE_INTERRUPTS
  if (len) {
    ctx->transmitting = true;
    mmio_write32(ctx->regs + UART_TX, buf[0]);
    mmio_write32(ctx->regs + UART_IER,
                 mmio_read32(ctx->regs + UART_IER) | UART_IER_IE_THR);
  } else {
    mmio_write32(ctx->regs + UART_IER,
                 mmio_read32(ctx->regs + UART_IER) & ~UART_IER_IE_THR);
    ctx->transmitting = false;
  }
#else
	for (i = 0; i < len; i++) {
    writeChar(ctx->regs, buf[i]);
	}
#endif
}

#ifdef JETSONTK1_CONSOLE_USE_INTERRUPTS
static void jetsontk1_driver_interrupt_read(void* arg)
{
  rtems_termios_tty *tty = arg;
  jetsontk1_uart_context *ctx = rtems_termios_get_device_context(tty);
  uint32_t lsr = mmio_read32(ctx->regs + UART_LSR);
  char input;

  if (lsr & UART_LSR_RDR) {
    input = mmio_read32(ctx->regs + UART_RBR);
    rtems_termios_enqueue_raw_characters(tty, &input, 1);
  }

  if (ctx->transmitting && lsr & UART_LSR_THRE) {
    rtems_termios_dequeue_characters(tty, 1);

  }
}
#endif

static int jetsontk1_driver_poll_read(rtems_termios_device_context *context)
{
  jetsontk1_uart_context *ctx = (jetsontk1_uart_context *) context;
  int result;
  /* Receive Buffer/Hold Register is empty if LSR[0] is 0 */
  while (!jetsontk1_driver_is_readable(ctx->regs)) {
    cpu_relax();
  }
  result = mmio_read32(ctx->regs + UART_RBR);
  //Read the UART.LSR register to clear interrupts
  mmio_write32(ctx->regs + UART_LCR_DLAB, 0);
  return result;
}

static bool jetsontk1_driver_set_attributes(
  rtems_termios_device_context  *context,
  const struct termios          *term
)
{
  jetsontk1_uart_context *ctx = (jetsontk1_uart_context *) context;
  rtems_termios_baud_t baud;
  unsigned int lcr;

  lcr = mmio_read32(ctx->regs + UART_LCR) | UART_LCR_DLAB;
  mmio_write32(ctx->regs + UART_LCR, lcr);

  baud = rtems_termios_baud_to_number(term->c_cflag);
	mmio_write32(
    ctx->regs + UART_DLL, (JETSONTK1_UART_SPEED / (baud * 16))
    & 0xff
  );
	mmio_write32(
    ctx->regs + UART_DLM,
    (JETSONTK1_UART_SPEED / (baud * 16)) >> 8 & 0xff
  );
  lcr = UART_LCR_8N1;
  mmio_write32(ctx->regs + UART_LCR, lcr);

  return true;
}

static bool jetsontk1_driver_first_open(
  rtems_termios_tty             *tty,
  rtems_termios_device_context  *context,
  struct termios                *term,
  rtems_libio_open_close_args_t *args
)
{
#ifdef JETSONTK1_CONSOLE_USE_INTERRUPTS
  rtems_status_code status;
  jetsontk1_uart_context *ctx = (jetsontk1_uart_context *) context;
#endif

  /*uint32_t gate_nr = (65 % 32);
  void *clock_reg = 0x60006000 + 0x330;*/

#ifndef JAILHOUSE_ENABLE
  mmio_write32(clock_reg, mmio_read32(clock_reg) | (1 << gate_nr));
#endif

  rtems_termios_set_initial_baud(tty, JETSONTK1_BAUD_RATE);
  /* status code */
  if (!jetsontk1_driver_set_attributes(context, term))
    return false;

#ifdef JETSONTK1_CONSOLE_USE_INTERRUPTS
  mmio_read32(ctx->regs + UART_RBR);
  mmio_read32(ctx->regs + UART_IIR);
  mmio_read32(ctx->regs + UART_LSR);
  mmio_read32(ctx->regs + UART_MSR);
    status = rtems_interrupt_handler_install(
    ctx->irq,
    ctx->device_name,
    RTEMS_INTERRUPT_UNIQUE,
    jetsontk1_driver_interrupt_read,
    tty
  );
  if (status != RTEMS_SUCCESSFUL)
    return false;

  mmio_write32(ctx->regs + UART_LCR, UART_LCR_8N1);
  mmio_write32(ctx->regs + UART_IER, UART_IER_IE_RHR);
#endif
  return true;
}

static void jetsontk1_driver_last_close(
  rtems_termios_tty             *tty,
  rtems_termios_device_context  *context,
  rtems_libio_open_close_args_t *args
)
{
}

static const rtems_termios_device_handler jetsontk1_driver_handler = {
  .first_open = jetsontk1_driver_first_open,
  .last_close = jetsontk1_driver_last_close,
  .write = jetsontk1_driver_write,
  .set_attributes = jetsontk1_driver_set_attributes,
#ifdef JETSONTK1_CONSOLE_USE_INTERRUPTS
  .mode = TERMIOS_IRQ_DRIVEN,
#else
  .poll_read = jetsontk1_driver_poll_read,
  .mode = TERMIOS_POLLED
#endif
};

rtems_status_code console_initialize(
  rtems_device_major_number  major,
  rtems_device_minor_number  minor,
  void                      *arg
)
{
  rtems_status_code status;
  rtems_termios_initialize();

  status = rtems_termios_device_install(
    jetsontk1_uart_instances[0].device_name,
    &jetsontk1_driver_handler,
    NULL,
    &jetsontk1_uart_instances[0].base
  );
  if (status != RTEMS_SUCCESSFUL) {
    rtems_fatal_error_occurred(status);
  }

  return RTEMS_SUCCESSFUL;
}

BSP_polling_getchar_function_type BSP_poll_char = jetsontk1_driver_poll_read;
