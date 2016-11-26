#include <bsp.h>

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
