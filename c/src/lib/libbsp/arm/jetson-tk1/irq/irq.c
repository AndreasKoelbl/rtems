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
#include <bsp/console.h>

void bsp_interrupt_handler_default(rtems_vector_number vector)
{
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

rtems_status_code bsp_interrupt_facility_initialize(void)
{
  return RTEMS_SUCCESSFUL;
}
