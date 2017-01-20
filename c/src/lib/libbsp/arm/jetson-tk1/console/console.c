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
#include <rtems/console.h>
#include <rtems/termiostypes.h>
#include <unistd.h>

#include "../../shared/include/linker-symbols.h"

#define UART_TX			      0x0
#define UART_DLL          0x0
#define UART_RBR          0x0
#define UART_DLM          0x4
#define UART_IER          0x4
#define UART_IIR          0x8
#define UART_LCR          0xc
#define  UART_LCR_8N1     ((1<<0)|(1<<1))
#define  UART_LCR_DLAB    (1<<7)
#define UART_LSR          0x14
#define  UART_LSR_RDR     (1<<0)
#define  UART_LSR_THRE    (1<<5)
#define UART_MSR          0x18

#define UART_IER_IE_RHR (1<<0)
#define UART_IER_IE_THR (1<<1)

#define JETSONTK1_BAUD_RATE 115200L
#define JETSONTK1_UART_SPEED 408000000L
#define DIVIDER ( (uint32_t)(JETSONTK1_UART_SPEED / \
                            ( JETSONTK1_BAUD_RATE * 16)) )
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
    .regs = UARTD,
    .irq = 90 + 32,
    .device_name = "ttyS0",
#ifdef JETSONTK1_CONSOLE_USE_INTERRUPTS
    .transmitting = false,
#endif
  },
  {
    .regs = UARTA,
    .irq = 36 + 32,
    .device_name = CONSOLE_DEVICE_NAME,//"ttyS1",
#ifdef JETSONTK1_CONSOLE_USE_INTERRUPTS
    .transmitting = false,
#endif
  },
  {
    .regs = UARTC,
    .irq = 46 + 32,
    .device_name = "ttys2",
#ifdef JETSONTK1_CONSOLE_USE_INTERRUPTS
    .transmitting = false,
#endif
  },

};

static void jetsontk1_uart_write(
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
    while (!(mmio_read32(ctx->regs + UART_LSR) & UART_LSR_THRE)) {
      cpu_relax();
    }

    if (buf[i] == '\n') {
      mmio_write32(ctx->regs + UART_TX, '\r');
    }

    mmio_write32(ctx->regs + UART_TX, buf[i]);
	}
#endif
}

#ifdef JETSONTK1_CONSOLE_USE_INTERRUPTS
static void jetsontk1_uart_interrupt_read(void* arg)
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

static int jetsontk1_uart_poll_read(rtems_termios_device_context *context)
{
  jetsontk1_uart_context *ctx = (jetsontk1_uart_context *) context;
  int result;
  while (!(mmio_read32(ctx->regs + UART_LSR) & UART_LSR_RDR)) {

    cpu_relax();
  }
  result = mmio_read32(ctx->regs + UART_RBR);
  /* Clear interrupts */
  mmio_write32(ctx->regs + UART_LSR, 0);
  return result;
}

static bool jetsontk1_uart_set_attributes(
  rtems_termios_device_context  *context,
  const struct termios          *term
)
{
  jetsontk1_uart_context *ctx = (jetsontk1_uart_context *) context;
  rtems_termios_baud_t baud;
  unsigned int lcr;
  uint16_t divider;

  /* We only have baud rate support yet */
  baud = rtems_termios_baud_to_number(term->c_cflag);
  divider =  JETSONTK1_UART_SPEED / (baud * 16);
  lcr = mmio_read32(ctx->regs + UART_LCR) | UART_LCR_DLAB;
  mmio_write32(ctx->regs + UART_LCR, lcr);

	mmio_write32(ctx->regs + UART_DLL, divider & 0xff);
	mmio_write32(ctx->regs + UART_DLM, (divider >> 8) & 0xff);
  lcr = UART_LCR_8N1;
  mmio_write32(ctx->regs + UART_LCR, lcr);

  return true;
}

static bool jetsontk1_uart_first_open(
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
  if (!jetsontk1_uart_set_attributes(context, term))
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
    jetsontk1_uart_interrupt_read,
    tty
  );
  if (status != RTEMS_SUCCESSFUL)
    return false;

  mmio_write32(ctx->regs + UART_LCR, UART_LCR_8N1);
  mmio_write32(ctx->regs + UART_IER, UART_IER_IE_RHR);
#endif
  return true;
}

static void jetsontk1_uart_last_close(
  rtems_termios_tty             *tty,
  rtems_termios_device_context  *context,
  rtems_libio_open_close_args_t *args
)
{
#ifdef JETSONTK1_CONSOLE_USE_INTERRUPTS
  rtems_status_code status;
  jetsontk1_uart_context *ctx = (jetsontk1_uart_context *) context;

  status = rtems_interrupt_handler_remove(
    ctx->irq,
    jetsontk1_uart_interrupt_read,
    tty
  );
  if (status != RTEMS_SUCCESSFUL) {
    rtems_fatal_error_occurred(status);
  }
#endif
}

static const rtems_termios_device_handler jetsontk1_uart_handler = {
  .first_open = jetsontk1_uart_first_open,
  .last_close = jetsontk1_uart_last_close,
  .write = jetsontk1_uart_write,
  .set_attributes = jetsontk1_uart_set_attributes,
#ifdef JETSONTK1_CONSOLE_USE_INTERRUPTS
  .mode = TERMIOS_IRQ_DRIVEN,
#else
  .poll_read = jetsontk1_uart_poll_read,
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

  for (uint8_t i = 0; i < sizeof(jetsontk1_uart_instances) / sizeof(jetsontk1_uart_instances[0]); i++)
  {
    status = rtems_termios_device_install(
      jetsontk1_uart_instances[i].device_name,
      &jetsontk1_uart_handler,
      NULL,
      &jetsontk1_uart_instances[i].base
    );
    if (status != RTEMS_SUCCESSFUL) {
      rtems_fatal_error_occurred(status);
    }
  }
  return RTEMS_SUCCESSFUL;
}
