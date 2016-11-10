/**
 * @file
 *
 * @ingroup arm_jetson-tk1
 *
 * @brief Jetson-TK1 memory accessing functions
 *
 * A short description of the purpose of this file.
 */

/*
 * Copyright (c) OTH Regensburg, 2016
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifndef LIBBSP_ARM_JETONTK1_MEMACCESS_H
#define LIBBSP_ARM_JETONTK1_MEMACCESS_H

static inline uint32_t mmio_read32(void *address)
{
	return *(volatile uint32_t *)address;
}


static inline void mmio_write32(void *address, uint32_t value)
{
	*(volatile uint32_t *)address = value;
}

#endif /* LIBBSP_ARM_JETONTK1_MEMACCESS_H */
