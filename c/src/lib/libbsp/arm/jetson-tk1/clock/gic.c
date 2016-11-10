#include <bsp/gic.h>
#include <bsp/memaccess.h>

#define _arm_write_sysreg(size, ...) arm_write_sysreg_ ## size(__VA_ARGS__)
#define arm_write_sysreg(...) _arm_write_sysreg(__VA_ARGS__)

#define _arm_read_sysreg(size, ...) arm_read_sysreg_ ## size(__VA_ARGS__)
#define arm_read_sysreg(...) _arm_read_sysreg(__VA_ARGS__)

#ifndef __ASSEMBLY__
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

uint64_t timer_get_ticks(void)
{
	uint64_t pct64;

	arm_read_sysreg(CNTPCT_EL0, pct64);
	return pct64;
}

void gic_enable(unsigned int irqn)
{
	mmio_write32(GICD_BASE + GICD_ISENABLER, 1 << irqn);
}

int gic_init(void)
{
	mmio_write32(GICC_BASE + GICC_CTLR, GICC_CTLR_GRPEN1);
	mmio_write32(GICC_BASE + GICC_PMR, GICC_PMR_DEFAULT);

	return 0;
}

void gic_write_eoi(uint32_t irqn)
{
	mmio_write32(GICC_BASE + GICC_EOIR, irqn);
}

uint32_t gic_read_ack(void)
{
	return mmio_read32(GICC_BASE + GICC_IAR);
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

void gic_enable_irq(unsigned int irq)
{
	gic_enable(irq);
}

unsigned long timer_get_frequency(void)
{
	unsigned long freq;

	arm_read_sysreg(CNTFRQ_EL0, freq);
	return freq;
}

static unsigned long emul_division(u64 val, u64 div)
{
	unsigned long cnt = 0;

	while (val > div) {
		val -= div;
		cnt++;
	}
	return cnt;
}

uint64_t timer_ticks_to_ns(uint64_t ticks)
{
  return emul_division(ticks * 1000,
    timer_get_frequency() / 1000 / 1000);
}
