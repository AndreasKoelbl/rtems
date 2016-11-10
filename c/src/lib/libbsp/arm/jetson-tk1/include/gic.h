/**
 * @file
 *
 * @ingroup jetson-tk1_interrupt
 *
 * @brief Jetson-TK1 generic interrupt controller definitions
 */

/*
 * Copyright (c) OTH Regensburg, 2016
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifndef LIBBSP_ARM_JETSONTK1_GIC_H
#define LIBBSP_ARM_JETSONTK1_GIC_H

/* Arm GIC Distributor interface */
#define GICD_BASE	((void *)0x50041000)
#define GICD_ISENABLER			0x0100

/* Arm GIC per-CPU interface */
#define GICC_BASE	((void *)0x50042000)
#define GICC_CTLR		0x0000
#define GICC_PMR		0x0004
#define GICC_IAR		0x000c
#define GICC_EOIR		0x0010

#define GICC_CTLR_GRPEN1	(1 << 0)
#define GICC_PMR_DEFAULT	0xf0


#define TIMER_IRQ 27
#define TEGRA_FATAL_IRQ_INSTALL 0xdeebdead

#define SYSREG_32(...) 32, __VA_ARGS__
#define SYSREG_64(...) 64, __VA_ARGS__

#define CNTPCT_EL0	SYSREG_64(0, c14)
#define CNTFRQ_EL0	SYSREG_32(0, c14, c0, 0)

#define is_sgi_ppi(irqn)		((irqn) < 32)
#define is_spi(irqn)			((irqn) > 31 && (irqn) < 1020)

#ifndef __ASSEMBLY__

typedef void (*irq_handler_t)(unsigned int);

static inline void gic_setup_irq_stack(void)
{
	static __attribute__((aligned(0x1000))) uint32_t irq_stack[1024];

	asm volatile (".arch_extension virt\n");
	asm volatile ("msr	SP_irq, %0\n" : : "r" (irq_stack));
	asm volatile ("cpsie	i\n");
}

int32_t gic_init(void);
uint32_t gic_read_ack(void);
uint64_t timer_get_ticks(void);
void gic_enable(unsigned int irqn);
void gic_write_eoi(uint32_t irqn);
void gic_setup(irq_handler_t handler);
void gic_enable_irq(unsigned int irq);

#endif
#endif /* LIBBSP_ARM_JETSONTK1_GIC_H */
