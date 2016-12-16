/**
 * @file
 *
 * @ingroup jetson-tk1_irq
 *
 * @brief Jetson-TK1 register definitions
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
#include <bsp/memory.h>
#include <bsp/jetson-tk1.h>

/* begin copy of jailhouse */
/* TODO: Use rtems functions */

typedef void (*irq_handler_t)(uint32_t);
static irq_handler_t irq_handler = (irq_handler_t)NULL;

void gic_init(void)
{
	mmio_write32(GICC_V2_BASE + GICC_CTLR, GICC_CTLR_GRPEN1);
	mmio_write32(GICC_V2_BASE + GICC_PMR, GICC_PMR_DEFAULT);
}

uint32_t gic_read_ack(void)
{
	return mmio_read32(GICC_V2_BASE + GICC_IAR);
}

void gic_write_eoi(uint32_t irqn)
{
	mmio_write32(GICC_V2_BASE + GICC_EOIR, irqn);
}

static inline void gic_setup_irq_stack(void)
{
	static __attribute__((aligned(0x1000))) uint32_t irq_stack[1024];

	asm volatile (".arch_extension virt\n");
	asm volatile ("msr	SP_irq, %0\n" : : "r" (irq_stack));
	asm volatile ("cpsie	i\n");
}

void vector_irq(void)
{
	uint32_t irqn;

	while (1) {
		irqn = gic_read_ack();
		if (irqn == 0x3ff)
			break;

		if (irq_handler)
			irq_handler(irqn);

		gic_write_eoi(irqn);
	}
}

void gic_setup(irq_handler_t handler)
{
	gic_init();
	irq_handler = handler;

	gic_setup_irq_stack();
}


/* end copy of jailhouse */

void bsp_interrupt_handler_default(rtems_vector_number vector)
{
}

void bsp_interrupt_dispatch(void)
{
}

rtems_status_code bsp_interrupt_vector_enable(rtems_vector_number vector)
{
	mmio_write32(GICD_V2_BASE + GICD_ISENABLER + ((vecotr >> 3) & ~0x3),
		     1 << (vector & 0x1f));
  return RTEMS_SUCCESSFUL;
}

rtems_status_code bsp_interrupt_vector_disable(rtems_vector_number vector)
{
  return RTEMS_SUCCESSFUL;
}

rtems_status_code bsp_interrupt_facility_initialize(void)
{
  gic_setup(bsp_interrupt_handler_default);

  return RTEMS_SUCCESSFUL;
}
