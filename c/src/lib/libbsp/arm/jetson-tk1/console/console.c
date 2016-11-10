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
/*
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#define NO_BSP_INIT

#include <bsp.h>
#include <rtems/console.h>
#include <bsp/fatal.h>
#include <rtems/libio.h>

#define JAILHOUSE_HC_DEBUG_CONSOLE_PUTC 8

/******* modification of jailhouse code *******/

static void hypervisor_putc(char c)
{
	register u32 num_res asm("r0") = JAILHOUSE_HC_DEBUG_CONSOLE_PUTC;
	register u32 arg1 asm("r1") = c;

	asm volatile(
		".arch_extension virt\n\t"
		"hvc #0x4a48\n\t"
		: "=r" (num_res)
		: "r" (num_res), "r" (arg1)
		: "memory");
}

static void jailhouse_dbgcon_write(
  rtems_termios_device_context *base,
  const char                   *buf,
  size_t                        n
)
{
  size_t i;
  jetsontk1_driver_context *ctx = (jetsontk1_driver_context *) base;

	for (i = 0; i < n; i++) {
		hypervisor_putc(buf[i]);
	}
}

/******* no_bsp stub *******/

/*  console_initialize
 *
 *  This routine initializes the console IO driver.
 *
 *  Input parameters: NONE
 *
 *  Output parameters:  NONE
 *
 *  Return values:
 */

rtems_device_driver console_initialize(
  rtems_device_major_number  major,
  rtems_device_minor_number  minor,
  void                      *arg
)
{
  const char* hello = "console initialized\n"
  rtems_status_code status;

  status = rtems_io_register_name(
    "/dev/console",
    major,
    (rtems_device_minor_number) 0
  );

  if (status != RTEMS_SUCCESSFUL)
    rtems_fatal_error_occurred(status);

  jailhouse_dbgcon_write(hello, strlen(hello) + 1);

  return RTEMS_SUCCESSFUL;
}

/*  is_character_ready
 *
 *  This routine returns TRUE if a character is available.
 *
 *  Input parameters: NONE
 *
 *  Output parameters:  NONE
 *
 *  Return values:
 */

bool is_character_ready(
  char *ch
)
{
  *ch = '\0';   /* return NULL for no particular reason */
  return true;
}

/*  inbyte
 *
 *  This routine reads a character from the SOURCE.
 *
 *  Input parameters: NONE
 *
 *  Output parameters:  NONE
 *
 *  Return values:
 *    character read from SOURCE
 */

char inbyte( void )
{
  /*
   *  If polling, wait until a character is available.
   */

  return '\0';
}

/*  outbyte
 *
 *  This routine transmits a character out the SOURCE.  It may support
 *  XON/XOFF flow control.
 *
 *  Input parameters:
 *    ch  - character to be transmitted
 *
 *  Output parameters:  NONE
 */

void outbyte(
  char ch
)
{
  /*
   *  If polling, wait for the transmitter to be ready.
   *  Check for flow control requests and process.
   *  Then output the character.
   */

  /*
   *  Carriage Return/New line translation.
   */

  if ( ch == '\n' )
    outbyte( '\r' );
}

/*
 *  Open entry point
 */

rtems_device_driver console_open(
  rtems_device_major_number major,
  rtems_device_minor_number minor,
  void                    * arg
)
{
  return RTEMS_SUCCESSFUL;
}

/*
 *  Close entry point
 */

rtems_device_driver console_close(
  rtems_device_major_number major,
  rtems_device_minor_number minor,
  void                    * arg
)
{
  return RTEMS_SUCCESSFUL;
}

/*
 * read bytes from the serial port. We only have stdin.
 */

rtems_device_driver console_read(
  rtems_device_major_number major,
  rtems_device_minor_number minor,
  void                    * arg
)
{
  rtems_libio_rw_args_t *rw_args;
  char *buffer;
  int maximum;
  int count = 0;

  rw_args = (rtems_libio_rw_args_t *) arg;

  buffer = rw_args->buffer;
  maximum = rw_args->count;

  for (count = 0; count < maximum; count++) {
    buffer[ count ] = inbyte();
    if (buffer[ count ] == '\n' || buffer[ count ] == '\r') {
      buffer[ count++ ]  = '\n';
      break;
    }
  }

  rw_args->bytes_moved = count;
  return (count >= 0) ? RTEMS_SUCCESSFUL : RTEMS_UNSATISFIED;
}

/*
 * write bytes to the serial port. Stdout and stderr are the same.
 */

rtems_device_driver console_write(
  rtems_device_major_number major,
  rtems_device_minor_number minor,
  void                    * arg
)
{
  int count;
  int maximum;
  rtems_libio_rw_args_t *rw_args;
  char *buffer;

  rw_args = (rtems_libio_rw_args_t *) arg;

  buffer = rw_args->buffer;
  maximum = rw_args->count;

  for (count = 0; count < maximum; count++) {
    if ( buffer[ count ] == '\n') {
      outbyte('\r');
    }
    outbyte( buffer[ count ] );
  }

  rw_args->bytes_moved = maximum;
  return 0;
}

/*
 *  IO Control entry point
 */

rtems_device_driver console_control(
  rtems_device_major_number major,
  rtems_device_minor_number minor,
  void                    * arg
)
{
  return RTEMS_SUCCESSFUL;
}

/******* bsp_howto *******/
/* TODO: move this functions to 8250.h */

const rtems_termios_handler jetsontk1_driver_handler_polled = {
.first_open = jetsontk1_driver_first_open,
.last_close = jetsontk1_driver_last_close,
.poll_read = jetsontk1_driver_poll_read,
.write = jailhouse_dbgcon_write,
.set_attributes = jetsontk1_driver_set_attributes,
.stop_remote_tx = NULL,
.start_remote_tx = NULL,
.mode = TERMIOS_POLLED
}

static int jetsontk1_driver_poll_read(rtems_termios_device_context *base)
{
  jetsontk1_driver_context *ctx = (jetsontk1_driver_context *) base;
  return -1;
}

static void jetsontk1_driver_interrupt_handler(
  rtems_vector_number  vector,
  void                *arg
)
{
  rtems_termios_tty *tty = arg;
  jetsontk1_driver_context *ctx = jetsontk1_termios_get_device_context(tty);
  char buf[N];
  size_t n;

  /*
   * Check if we have received something.  The function reads the
   * received characters from the device and stores them in the
   * buffer.  It returns the number of read characters.
   */
  n = jetsontk1_driver_read_received_chars(ctx, buf, N);
  if (n > 0) {
    /* Hand the data over to the Termios infrastructure */
    rtems_termios_enqueue_raw_characters(tty, buf, n);
  }

  /*
   * Check if we have something transmitted.  The functions returns
   * the number of transmitted characters since the last write to the
   * device.
   */
  n = jetsontk1_driver_transmitted_chars(ctx);
  if (n > 0) {
    /*
     * Notify Termios that we have transmitted some characters.  It
     * will call now the interrupt write function if more characters
     * are ready for transmission.
     */
    rtems_termios_dequeue_characters(tty, n);
  }
}

static void jetsontk1_driver_interrupt_write(
  rtems_termios_device_context  *base,
  const char                    *buf,
  size_t                         n
)
{
  jetsontk1_driver_context *ctx = (jetsontk1_driver_context *) base;

  /*
   * Tell the device to transmit some characters from buf (less than
   * or equal to n).  When the device is finished it should raise an
   * interrupt.  The interrupt handler will notify Termios that these
   * characters have been transmitted and this may trigger this write
   * function again.  You may have to store the number of outstanding
   * characters in the device data structure.
   */

  /*
   * Termios will set n to zero to indicate that the transmitter is
   * now inactive.  The output buffer is empty in this case.  The
   * driver may disable the transmit interrupts now.
   */
}


static jetsontk1_driver_context driver_context_table[M] = { /* Some values */ };

rtems_device_driver console_initialize(
  rtems_device_major_number  major,
  rtems_device_minor_number  minor,
  void                      *arg
)
{
  rtems_status_code sc;
  const rtems_termios_handler *handler = &jetsontk1_driver_handler_polled;

  /*
   * Initialize the Termios infrastructure.  If Termios has already
   * been initialized by another device driver, then this call will
   * have no effect.
   */
  rtems_termios_initialize();

  /* Initialize each device */
  for (
    minor = 0;
    minor < RTEMS_ARRAY_SIZE(driver_context_table);
    ++minor
  ) {
    jetsontk1_driver_context *ctx = &driver_context_table[minor];

    /*
     * Install this device in the file system and Termios.  In order
     * to use the console (i.e. being able to do printf, scanf etc.
     * on stdin, stdout and stderr), one device must be registered as
     * "/dev/console" (CONSOLE_DEVICE_NAME).
     */
    sc = rtems_termios_device_install(
      ctx->device_name,
      major,
      minor,
      handler,
      NULL,
      ctx
    );
    if (sc != RTEMS_SUCCESSFUL) {
      bsp_fatal(TEGRAK1_BSP_FATAL_CONSOLE_DEVICE_INSTALL);
    }
  }

  return RTEMS_SUCCESSFUL;
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
   * You may add some initialization code here.
   */

  /*
   * Sets the initial baud rate.  This should be set to the value of
   * the boot loader.  This function accepts only exact Termios baud
   * values.
   */
  sc = rtems_termios_set_initial_baud(tty, MY_DRIVER_BAUD_RATE);
  if (sc != RTEMS_SUCCESSFUL) {
    /* Not a valid Termios baud */
  }

  /*
   * Alternatively you can set the best baud.
   */
  rtems_termios_set_best_baud(term, MY_DRIVER_BAUD_RATE);

  /*
   * To propagate the initial Termios attributes to the device use
   * this.
   */
  ok = jetsontk1_driver_set_attributes(base, term);
  if (!ok) {
    /* This is bad */
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

static bool jetsontk1_driver_set_attributes(
  rtems_termios_device_context *base,
  const struct termios         *term
)
{
  jetsontk1_driver_context *ctx = (jetsontk1_driver_context *) base;

  /*
   * Inspect the termios data structure and configure the device
   * appropriately.  The driver should only be concerned with the
   * parts of the structure that specify hardware setting for the
   * communications channel such as baud, character size, etc.
   */

  /*
   * Return true to indicate a successful set attributes, and false
   * otherwise.
   */
  return true;
}

