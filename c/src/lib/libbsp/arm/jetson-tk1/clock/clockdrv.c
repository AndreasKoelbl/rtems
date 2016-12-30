/**
 * @file
 *
 * @ingroup bsp_clock
 *
 * @brief Raspberry Pi clock support.
 */

/*
 * BCM2835 Clock driver
 *
 * Copyright (c) 2013 Alan Cudmore
 * Copyright (c) 2016 Pavel Pisa
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *
 *  http://www.rtems.org/license/LICENSE
 *
*/

#include <rtems.h>
#include <rtems/timecounter.h>
#include <bsp.h>
#include <bsp/jetson-tk1.h>
#include <bsp/memory.h>
#include <bsp/irq.h>
#include <bsp/irq-generic.h>

#define BEATS_PER_SEC  10U
#define USEC_PER_SEC	1000000L

/* Time Stamp Counter (TSC) base address */
#define TSC_CNTCR		0		/* TSC control registers */
#define TSC_CNTCR_ENABLE	(1 << 0)	/* Enable*/
#define TSC_CNTCR_HDBG		(1 << 1)	/* Halt on debug */
#define TSC_CNTCV0		0x8		/* TSC counter (LSW) */
#define TSC_CNTCV1		0xC		/* TSC counter (MSW) */
#define TSC_CNTFID0		0x20		/* TSC freq id 0 */

/* This is defined in ../../../shared/clockdrv_shell.h */
void Clock_isr(rtems_irq_hdl_param arg);
static struct timecounter clock_tc;
static uint32_t counter = 0;
static volatile uint64_t ticks_per_beat;
static volatile uint64_t expected_ticks;
static uint32_t arch_timer_us_mult, arch_timer_us_shift;

void clocks_calc_mult_shift(uint32_t *mult, uint32_t *shift, uint32_t from,
                            uint32_t to, uint32_t maxsec)
{
	uint64_t tmp;
	uint32_t sft, sftacc= 32;

	/*
	 * Calculate the shift factor which is limiting the conversion
	 * range:
	 */
	tmp = ((uint64_t)maxsec * from) >> 32;
	while (tmp) {
		tmp >>=1;
		sftacc--;
	}

	/*
	 * Find the conversion shift/mult pair which has the best
	 * accuracy and fits the maxsec conversion range:
	 */
	for (sft = 32; sft > 0; sft--) {
		tmp = (uint64_t) to << sft;
		tmp += from / 2;
		tmp /= from;
		if ((tmp >> sftacc) == 0)
			break;
	}
	*mult = tmp;
	*shift = sft;
}

uint32_t jetson_clock_get_timecount(struct timecounter *tc)
{
	uint32_t value;

	asm volatile("mrrc p15, 0, %Q0, %R0, c14" : "=r" (value));
  return value;
}

static void jetson_clock_at_tick(void)
{
}

static void jetson_clock_handler_install_isr(
  rtems_isr_entry clock_isr
)
{
  rtems_status_code sc = RTEMS_SUCCESSFUL;
  //printk("Installing %p\n", (void*) clock_isr);

  if (clock_isr != NULL) {
    sc = rtems_interrupt_handler_install(
      TIMER_IRQ,
      "Clock",
      RTEMS_INTERRUPT_UNIQUE,
      (rtems_interrupt_handler) clock_isr,
      NULL
    );
  } else {
    /* Remove interrupt handler */
    sc = rtems_interrupt_handler_remove(
      TIMER_IRQ,
      (rtems_interrupt_handler) Clock_isr,
      NULL
    );
  }
  if ( sc != RTEMS_SUCCESSFUL ) {
    printk("fatal: %p\n", (uint32_t*)clock_isr);
    rtems_fatal_error_occurred(0xdeadbeef);
  }
}

void gic_timer_start(uint32_t timeout)
{
	arm_write_sysreg_32(0, c14, c3, 0, timeout);
	arm_write_sysreg_32(0, c14, c3, 1, 1);
}

unsigned long timer_get_frequency(void)
{
	unsigned long freq;
	arm_read_sysreg_32(0, c14, c0, 0, freq);
	return freq;
}

static void jetson_clock_initialize_hardware(void)
{
  uint32_t us_per_tick = rtems_configuration_get_microseconds_per_tick();
  uint32_t timecounter_ticks_per_clock_tick =
            ( timer_get_frequency() * us_per_tick ) / USEC_PER_SEC;

  //4156 * 30213
  //4156 * 30213
  printk("Rtems timer start value:  %lu * %lu / %u = %d\n\n", timecounter_ticks_per_clock_tick);
	gic_timer_start(timecounter_ticks_per_clock_tick);
  clock_tc.tc_get_timecount = jetson_clock_get_timecount;
  clock_tc.tc_counter_mask = 0xffffffff;
  clock_tc.tc_frequency = FREQUENCY_TSC;
  clock_tc.tc_quality = RTEMS_TIMECOUNTER_QUALITY_CLOCK_DRIVER;
  rtems_timecounter_install( &clock_tc );
}

static void jetson_clock_cleanup(void)
{
  rtems_status_code sc = RTEMS_SUCCESSFUL;

  sc = rtems_interrupt_handler_remove(
    TIMER_IRQ,
    (rtems_interrupt_handler) Clock_isr,
    NULL
  );
  if (sc != RTEMS_SUCCESSFUL) {
    rtems_fatal_error_occurred(0xdeadbeef);
  }
}

#define Clock_driver_support_at_tick() jetson_clock_at_tick()

#define Clock_driver_support_initialize_hardware() jetson_clock_initialize_hardware()

#define Clock_driver_support_shutdown_hardware() jetson_clock_cleanup()

#define Clock_driver_support_install_isr(clock_isr, old_isr) \
  do {                                                 \
    jetson_clock_handler_install_isr(clock_isr);  \
    old_isr = NULL;                                    \
  } while (0)

#define CLOCK_DRIVER_USE_ONLY_BOOT_PROCESSOR 1

#include "../../../shared/clockdrv_shell.h"
