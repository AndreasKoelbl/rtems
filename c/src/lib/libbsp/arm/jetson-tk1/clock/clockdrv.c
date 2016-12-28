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

uint64_t jetson_clock_get_timecount(struct timecounter *tc)
{
	uint32_t value;
  uint32_t tsc_ref_freq = 1000000;// tegra_clk_measure_input_freq();
  clocks_calc_mult_shift(&arch_timer_us_mult, &arch_timer_us_shift,
    tsc_ref_freq, USEC_PER_SEC, 0);

	asm volatile("mrc p15, 0, %0, c14, c2, 0" : "=r" (value));
  value = (uint64_t)((uint64_t)value * arch_timer_us_mult)
    >> arch_timer_us_shift;
  return 0xffffffff - value;
}

static void jetson_clock_at_tick(void)
{
}

static void jetson_clock_handler_install_isr(
  rtems_isr_entry clock_isr
)
{
  rtems_status_code sc = RTEMS_SUCCESSFUL;
  printk("Installing %p\n", (uint32_t*)clock_isr);

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

void timer_start(uint64_t timeout)
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

void init_timer(void)
{
  uint32_t tsc_ref_freq = 1000000;// tegra_clk_measure_input_freq();
  uint32_t reg = 1000000;

    clocks_calc_mult_shift(&arch_timer_us_mult, &arch_timer_us_shift,
      tsc_ref_freq, USEC_PER_SEC, 0);

		/* Set the Timer System Counter (TSC) reference frequency
		   NOTE: this is a write once register */
		mmio_write32(TSC_BASE + TSC_CNTFID0, tsc_ref_freq);
		/* Program CNTFRQ to the same value.
		   NOTE: this is a write once (per CPU reset) register. */
    /* This will kill the jetson
		__asm__("mcr p15, 0, %0, c14, c0, 0\n" : : "r" (tsc_ref_freq));
    */
		/* CNTFRQ must agree with the TSC reference frequency. */
		__asm__("mrc p15, 0, %0, c14, c0, 0\n" : "=r" (reg));
    if (reg != tsc_ref_freq)
    {
      printk("Error: reg is wrong\n");
    }
		/* Enable the TSC. */
		reg = mmio_read32(TSC_CNTCR);
		reg |= TSC_CNTCR_ENABLE | TSC_CNTCR_HDBG;
		mmio_write32(TSC_BASE + TSC_CNTCR, reg);
}
static void jetson_clock_initialize_hardware(void)
{
  uint64_t us_per_tick = rtems_configuration_get_microseconds_per_tick();
  uint32_t timecounter_ticks_per_clock_tick =
            ( timer_get_frequency() * us_per_tick ) / USEC_PER_SEC;

  init_timer();
  printk("Rtems timer start value: %d\n", timecounter_ticks_per_clock_tick);
	timer_start(timecounter_ticks_per_clock_tick);
  clock_tc.tc_get_timecount = jetson_clock_get_timecount;
  clock_tc.tc_counter_mask = 0xffffffff;
  clock_tc.tc_frequency = 1000000U;
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
