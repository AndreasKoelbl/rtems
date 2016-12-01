#include <bsp.h>
#include <bsp/irq.h>

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
  /* copy interrupts to (*(0x0)) */
  uint8_t i;

/*  for (i = 0; i < 0x40; i += 4)
  {
    mmio_write32((void*) i, mmio_read32((void*) INMATE_BASE + i));
  }
  */

  return RTEMS_SUCCESSFUL;
}
