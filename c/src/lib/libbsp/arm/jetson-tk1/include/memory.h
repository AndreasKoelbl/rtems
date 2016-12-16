/**
 * @file
 *
 * @ingroup jetson-tk1_reg
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

#ifndef LIBBSP_ARM_JETSONTK1_MEMORY_H
#define LIBBSP_ARM_JETSONTK1_MEMORY_H

static inline uint32_t mmio_read32(void *address)
{
	return *(volatile uint32_t *)address;
}

static inline void mmio_write32(void *address, uint32_t value)
{
	*(volatile uint32_t *)address = value;
}

#endif /* LIBBSP_ARM_JETSONTK1_MEMORY_H */
