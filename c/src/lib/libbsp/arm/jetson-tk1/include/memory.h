/*
 * Copyright (c) OTH Regensburg, 2017
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifndef LIBBSP_ARM_JETSONTK1_MEMORY_H
#define LIBBSP_ARM_JETSONTK1_MEMORY_H

#define SYSREG_32(...) 32, __VA_ARGS__
#define SYSREG_64(...) 64, __VA_ARGS__

#define _arm_read_sysreg(size, ...) arm_read_sysreg_ ## size(__VA_ARGS__)
#define arm_read_sysreg(...) _arm_read_sysreg(__VA_ARGS__)
asm(".arch_extension virt\n");

#define arm_write_sysreg_32(op1, crn, crm, op2, val) \
	asm volatile ("mcr	p15, "#op1", %0, "#crn", "#crm", "#op2"\n" \
			: : "r"((uint32_t)(val)))
#define arm_write_sysreg_64(op1, crm, val) \
	asm volatile ("mcrr	p15, "#op1", %Q0, %R0, "#crm"\n" \
			: : "r"((uint64_t)(val)))

#define arm_read_sysreg_32(op1, crn, crm, op2, val) \
	asm volatile ("mrc	p15, "#op1", %0, "#crn", "#crm", "#op2"\n" \
			: "=r"((uint32_t)(val)))
#define arm_read_sysreg_64(op1, crm, val) \
	asm volatile ("mrrc	p15, "#op1", %Q0, %R0, "#crm"\n" \
			: "=r"((uint64_t)(val)))

static inline uint32_t mmio_read32(void *address)
{
	return *(volatile uint32_t *)address;
}

static inline void mmio_write32(void *address, uint32_t value)
{
	*(volatile uint32_t *)address = value;
}

static inline unsigned char mmio_read8(void *address)
{
	return *(volatile unsigned char *)address;
}

static inline void mmio_write8(void *address, unsigned char value)
{
	*(volatile unsigned char *)address = value;
}

static inline void mmio_write16(void *address, unsigned short value)
{
	*(volatile unsigned short *)address = value;
}

#endif /* LIBBSP_ARM_JETSONTK1_MEMORY_H */
