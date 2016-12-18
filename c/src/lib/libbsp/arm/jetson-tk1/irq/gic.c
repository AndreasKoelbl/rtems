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
#include <bsp/gic.h>
#include <bsp/memory.h>

/* begin copy of jailhouse */
/* TODO: Migrate to rtems internalinternal functions */

/* architecture specific gic-v2 - gic-v3 */
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


