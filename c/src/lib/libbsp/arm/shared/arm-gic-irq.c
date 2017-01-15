/*
 * Copyright (c) 2013 embedded brains GmbH.  All rights reserved.
 *
 *  embedded brains GmbH
 *  Dornierstr. 4
 *  82178 Puchheim
 *  Germany
 *  <info@embedded-brains.de>
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#include <bsp/arm-gic.h>

#include <rtems/score/armv4.h>

#include <libcpu/arm-cp15.h>

#include <bsp/irq.h>
#include <bsp/irq-generic.h>
#include <bsp/start.h>
#include <bsp/memory.h>

#define GICD_CTLR			0x0000
# define GICD_CTLR_ARE_NS		(1 << 4)
#define GICD_TYPER			0x0004
#define GICD_IIDR			0x0008
#define GICD_IGROUPR			0x0080
#define GICD_ISENABLER			0x0100
#define GICD_ICENABLER			0x0180
#define GICD_ISPENDR			0x0200
#define GICD_ICPENDR			0x0280
#define GICD_ISACTIVER			0x0300
#define GICD_ICACTIVER			0x0380
#define GICD_IPRIORITYR			0x0400
#define GICD_ITARGETSR			0x0800
#define GICD_ICFGR			0x0c00
#define GICD_NSACR			0x0e00
#define GICD_SGIR			0x0f00
#define GICD_CPENDSGIR			0x0f10
#define GICD_SPENDSGIR			0x0f20
#define GICD_IROUTER			0x6000


#define GIC_CPUIF ((volatile gic_cpuif *) BSP_ARM_GIC_CPUIF_BASE)

#define PRIORITY_DEFAULT 127

#define GICC_CTLR		0x0000
#define GICC_PMR		0x0004
#define GICC_IAR		0x000c
#define GICC_EOIR		0x0010

#define GICC_CTLR_GRPEN1	(1 << 0)

#define GICC_PMR_DEFAULT	0xf0

#define UART_IER 0x4

void bsp_interrupt_dispatch(void)
{  /*
  volatile gic_cpuif *cpuif = GIC_CPUIF;
  uint32_t icciar = cpuif->icciar;
  rtems_vector_number vector = GIC_CPUIF_ICCIAR_ACKINTID_GET(icciar);
  rtems_vector_number spurious = 1023;

  printk("Interrupt: %u\n", vector);

  if (vector != spurious) {
    uint32_t psr = _ARMV4_Status_irq_enable();

    bsp_interrupt_handler_dispatch(vector);

    _ARMV4_Status_restore(psr);

    cpuif->icceoir = icciar;
  }
  */
  uint32_t vector = mmio_read32(BSP_ARM_GIC_CPUIF_BASE + GICC_IAR);
  if (vector == 1023)
    return;
//  uint32_t psr = _ARMV4_Status_irq_enable();
  mmio_write32(((void*)0x70006300) + UART_IER, (1<<0));
//  printk("v: %u\n", vector);
  bsp_interrupt_handler_dispatch(vector);
//  _ARMV4_Status_restore(psr);
	mmio_write32(BSP_ARM_GIC_CPUIF_BASE + GICC_EOIR, vector);

  /*
  volatile gic_cpuif *cpuif = GIC_CPUIF;
  uint32_t icciar = cpuif->icciar;
  rtems_vector_number vector = GIC_CPUIF_ICCIAR_ACKINTID_GET(icciar);
  rtems_vector_number spurious = 1023;

  if (vector != spurious) {
    uint32_t psr = _ARMV4_Status_irq_enable();

    bsp_interrupt_handler_dispatch(vector);

    _ARMV4_Status_restore(psr);

    cpuif->icceoir = icciar;
  }
  */
}

rtems_status_code bsp_interrupt_vector_enable(rtems_vector_number vector)
{
  rtems_status_code sc = RTEMS_SUCCESSFUL;

  printk("enabling: %u\n", vector);
  if (bsp_interrupt_is_valid_vector(vector)) {
    volatile gic_dist *dist = ARM_GIC_DIST;

    gic_id_enable(dist, vector);
  } else {
    sc = RTEMS_INVALID_ID;
  }

  return sc;
}

rtems_status_code bsp_interrupt_vector_disable(rtems_vector_number vector)
{
  rtems_status_code sc = RTEMS_SUCCESSFUL;
  printk("disable\n");

  if (bsp_interrupt_is_valid_vector(vector)) {
    volatile gic_dist *dist = ARM_GIC_DIST;

    gic_id_disable(dist, vector);
  } else {
    sc = RTEMS_INVALID_ID;
  }

  return sc;
}

static inline uint32_t get_id_count(volatile gic_dist *dist)
{
  uint32_t id_count = GIC_DIST_ICDICTR_IT_LINES_NUMBER_GET(dist->icdictr);

  id_count = 32 * (id_count + 1);
  id_count = id_count <= 1020 ? id_count : 1020;

  return id_count;
}

rtems_status_code bsp_interrupt_facility_initialize(void)
{
  volatile gic_cpuif *cpuif = GIC_CPUIF;
  volatile gic_dist *dist = ARM_GIC_DIST;
  uint32_t id_count = get_id_count(dist);
  uint32_t id;

  arm_cp15_set_exception_handler(
    ARM_EXCEPTION_IRQ,
    _ARMV4_Exception_interrupt
  );
  mmio_write32((void*) BSP_ARM_GIC_CPUIF_BASE + GICC_CTLR, GICC_CTLR_GRPEN1);
  mmio_write32((void*) BSP_ARM_GIC_CPUIF_BASE + GICC_PMR, GICC_PMR_DEFAULT);
/*
  for (id = 0; id < id_count; ++id) {
    if (id == 90 || id == 122) {
      continue;
    }
    gic_id_set_priority(dist, id, PRIORITY_DEFAULT);
  }

  for (id = 32; id < id_count; ++id) {
    print_hex(id);
    gic_id_set_targets(dist, id, 0x01);
  }

  cpuif->iccpmr = GIC_CPUIF_ICCPMR_PRIORITY(0xff);
  cpuif->iccbpr = GIC_CPUIF_ICCBPR_BINARY_POINT(0x0);
  cpuif->iccicr = GIC_CPUIF_ICCICR_ENABLE;
  */
  /*
  dist->icddcr = GIC_DIST_ICDDCR_ENABLE;
  jailhouse_debug_console_write("id_count would have been: ");
  print_hex(id_count);
  jailhouse_debug_console_write("\n");
  */

  return RTEMS_SUCCESSFUL;
}

#ifdef RTEMS_SMP
#error TODO add SMP
BSP_START_TEXT_SECTION void arm_gic_irq_initialize_secondary_cpu(void)
{
  volatile gic_cpuif *cpuif = GIC_CPUIF;
  volatile gic_dist *dist = ARM_GIC_DIST;

  while ((dist->icddcr & GIC_DIST_ICDDCR_ENABLE) == 0) {
    /* Wait */
  }

  cpuif->iccpmr = GIC_CPUIF_ICCPMR_PRIORITY(0xff);
  cpuif->iccbpr = GIC_CPUIF_ICCBPR_BINARY_POINT(0x0);
  cpuif->iccicr = GIC_CPUIF_ICCICR_ENABLE;
}
#endif

rtems_status_code arm_gic_irq_set_priority(
  rtems_vector_number vector,
  uint8_t priority
)
{
  rtems_status_code sc = RTEMS_SUCCESSFUL;
  printk("set prio %u on %u\n", priority, vector);

  if (bsp_interrupt_is_valid_vector(vector)) {
    volatile gic_dist *dist = ARM_GIC_DIST;

    gic_id_set_priority(dist, vector, priority);
  } else {
    sc = RTEMS_INVALID_ID;
  }

  return sc;
}

rtems_status_code arm_gic_irq_get_priority(
  rtems_vector_number vector,
  uint8_t *priority
)
{
  rtems_status_code sc = RTEMS_SUCCESSFUL;

  printk("get prio on %u\n", vector);
  if (bsp_interrupt_is_valid_vector(vector)) {
    volatile gic_dist *dist = ARM_GIC_DIST;

    *priority = gic_id_get_priority(dist, vector);
  } else {
    sc = RTEMS_INVALID_ID;
  }

  return sc;
}

rtems_status_code arm_gic_irq_set_affinity(
  rtems_vector_number vector,
  uint8_t targets
)
{
  rtems_status_code sc = RTEMS_SUCCESSFUL;

  printk("affinity: %u on vector %u\n", targets, vector);

  if (bsp_interrupt_is_valid_vector(vector)) {
    volatile gic_dist *dist = ARM_GIC_DIST;

    gic_id_set_targets(dist, vector, targets);
  } else {
    sc = RTEMS_INVALID_ID;
  }

  return sc;
}
