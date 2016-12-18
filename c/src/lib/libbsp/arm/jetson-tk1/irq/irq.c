#include <bsp.h>
#include <bsp/irq.h>
#include <console.h>

extern void hypervisor_putc(char c);

inline uint32_t mmio_read32(void *address)
{
	return *(volatile uint32_t *)address;
}

inline void mmio_write32(void *address, uint32_t value)
{
	*(volatile uint32_t *)address = value;
}
void bsp_interrupt_dispatch(void)
{
}

rtems_status_code bsp_interrupt_vector_enable(rtems_vector_number vector)
{
  return RTEMS_SUCCESSFUL;
}

rtems_status_code bsp_interrupt_vector_disable(rtems_vector_number vector)
{
  return RTEMS_SUCCESSFUL;
}

void bsp_interrupt_handler_default(rtems_vector_number vector)
{
}

rtems_status_code bsp_interrupt_facility_initialize(void)
{
  return RTEMS_SUCCESSFUL;
}
