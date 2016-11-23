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

#ifndef LIBBSP_ARM_JETSONTK1_H
#define LIBBSP_ARM_JETSONTK1_H

#include <bspopts.h>
#include <stdint.h>
/* #include <bsp/utility.h> */
#include <rtems/termiostypes.h>

#define UART0 (Uart*) (0x70006000)
#define UART0_IRQn 0
#define ID_UART0 0

typedef struct {
  rtems_termios_device_context base;
  const char *device_name;
} jetsontk1_driver_context;

typedef struct {
  uint32_t TX;    /* same as UART_DLL, depends on value in LCR */
  uint64_t DLM;
  uint64_t LSR;
  void *location;
} Uart;

/* extern const rtems_termios_handler my_driver_handler_polled; */

/**
 * @defgroup jetson-tk1_reg Register Definitions
 *
 * @ingroup arm_jetson-tk1
 *
 * @brief Register Definitions
 *
 * @{
 */

/**
 * @name Register Macros
 *
 * @{
 */

#define TEGRAK1_REG(x)           (*(volatile uint32_t *)(x))
#define TEGRAK1_BIT(n)           (1 << (n))

/** @} */

/**
 * @name Peripheral Base Register Address
 *
 * @{
 */
#define TEGRAK1_PERIPHERAL_BASE      0x01001000
#define TEGRAK1_PERIPHERAL_SIZE      0x7C00AFFF

#define TEGRAK1_BSP_FATAL_CONSOLE_DEVICE_INSTALL 0x55

/**
 * @name Internal ARM Timer Registers
 *
 * @{
 */
#define TEGRAK1_CLOCK_FREQ       12000000

/* not needed in jailhouse, gic is used for this
#define TEGRAK1_TIMER_BASE       (TEGRAK1_PERIPHERAL_BASE + 0xB400)

#define TEGRAK1_TIMER_LOD        (TEGRAK1_TIMER_BASE + 0x00)
#define TEGRAK1_TIMER_VAL        (TEGRAK1_TIMER_BASE + 0x04)
#define TEGRAK1_TIMER_CTL        (TEGRAK1_TIMER_BASE + 0x08)
#define TEGRAK1_TIMER_CLI        (TEGRAK1_TIMER_BASE + 0x0C)
#define TEGRAK1_TIMER_RIS        (TEGRAK1_TIMER_BASE + 0x10)
#define TEGRAK1_TIMER_MIS        (TEGRAK1_TIMER_BASE + 0x14)
#define TEGRAK1_TIMER_RLD        (TEGRAK1_TIMER_BASE + 0x18)
#define TEGRAK1_TIMER_DIV        (TEGRAK1_TIMER_BASE + 0x1C)
#define TEGRAK1_TIMER_CNT        (TEGRAK1_TIMER_BASE + 0x20)

#define TEGRAK1_TIMER_PRESCALE    0xF9
*/

/** @} */

/**
 * @name Power Management and Watchdog Registers
 *
 * @{
 */

/*
#define TEGRAK1_PM_PASSWD_MAGIC  0x5a000000

#define TEGRAK1_PM_BASE          (TEGRAK1_PERIPHERAL_BASE + 0x100000)

#define TEGRAK1_PM_GNRIC         (TEGRAK1_PM_BASE + 0x00)
#define TEGRAK1_PM_GNRIC_POWUP   0x00000001
#define TEGRAK1_PM_GNRIC_POWOK   0x00000002
#define TEGRAK1_PM_GNRIC_ISPOW   0x00000004
#define TEGRAK1_PM_GNRIC_MEMREP  0x00000008
#define TEGRAK1_PM_GNRIC_MRDONE  0x00000010
#define TEGRAK1_PM_GNRIC_ISFUNC  0x00000020
#define TEGRAK1_PM_GNRIC_RSTN    0x00000fc0
#define TEGRAK1_PM_GNRIC_ENAB    0x00001000
#define TEGRAK1_PM_GNRIC_CFG     0x007f0000

#define TEGRAK1_PM_AUDIO         (TEGRAK1_PM_BASE + 0x04)
#define TEGRAK1_PM_AUDIO_APSM    0x000fffff
#define TEGRAK1_PM_AUDIO_CTRLEN  0x00100000
#define TEGRAK1_PM_AUDIO_RSTN    0x00200000

#define TEGRAK1_PM_STATUS        (TEGRAK1_PM_BASE + 0x18)

#define TEGRAK1_PM_RSTC          (TEGRAK1_PM_BASE + 0x1c)
#define TEGRAK1_PM_RSTC_DRCFG    0x00000003
#define TEGRAK1_PM_RSTC_WRCFG    0x00000030
#define TEGRAK1_PM_RSTC_WRCFG_FULL   0x00000020
#define TEGRAK1_PM_RSTC_SRCFG    0x00000300
#define TEGRAK1_PM_RSTC_QRCFG    0x00003000
#define TEGRAK1_PM_RSTC_FRCFG    0x00030000
#define TEGRAK1_PM_RSTC_HRCFG    0x00300000

#define TEGRAK1_PM_RSTS          (TEGRAK1_PM_BASE + 0x20)
#define TEGRAK1_PM_RSTS_HADDRQ   0x00000001
#define TEGRAK1_PM_RSTS_HADDRF   0x00000002
#define TEGRAK1_PM_RSTS_HADDRH   0x00000004
#define TEGRAK1_PM_RSTS_HADWRQ   0x00000010
#define TEGRAK1_PM_RSTS_HADWRF   0x00000020
#define TEGRAK1_PM_RSTS_HADWRH   0x00000040
#define TEGRAK1_PM_RSTS_HADSRQ   0x00000100
#define TEGRAK1_PM_RSTS_HADSRF   0x00000200
#define TEGRAK1_PM_RSTS_HADSRH   0x00000400
#define TEGRAK1_PM_RSTS_HADPOR   0x00001000

#define TEGRAK1_PM_WDOG          (TEGRAK1_PM_BASE + 0x24)

*/
/** @} */

/**
 * @name GPIO Registers
 *
 * @{
 */
/*
#define TEGRAK1_GPIO_REGS_BASE   (TEGRAK1_PERIPHERAL_BASE + 0x200000)

#define TEGRAK1_GPIO_GPFSEL1     (TEGRAK1_GPIO_REGS_BASE + 0x04)
#define TEGRAK1_GPIO_GPSET0      (TEGRAK1_GPIO_REGS_BASE + 0x1C)
#define TEGRAK1_GPIO_GPCLR0      (TEGRAK1_GPIO_REGS_BASE + 0x28)
#define TEGRAK1_GPIO_GPLEV0      (TEGRAK1_GPIO_REGS_BASE + 0x34)
#define TEGRAK1_GPIO_GPEDS0      (TEGRAK1_GPIO_REGS_BASE + 0x40)
#define TEGRAK1_GPIO_GPREN0      (TEGRAK1_GPIO_REGS_BASE + 0x4C)
#define TEGRAK1_GPIO_GPFEN0      (TEGRAK1_GPIO_REGS_BASE + 0x58)
#define TEGRAK1_GPIO_GPHEN0      (TEGRAK1_GPIO_REGS_BASE + 0x64)
#define TEGRAK1_GPIO_GPLEN0      (TEGRAK1_GPIO_REGS_BASE + 0x70)
#define TEGRAK1_GPIO_GPAREN0     (TEGRAK1_GPIO_REGS_BASE + 0x7C)
#define TEGRAK1_GPIO_GPAFEN0     (TEGRAK1_GPIO_REGS_BASE + 0x88)
#define TEGRAK1_GPIO_GPPUD       (TEGRAK1_GPIO_REGS_BASE + 0x94)
#define TEGRAK1_GPIO_GPPUDCLK0   (TEGRAK1_GPIO_REGS_BASE + 0x98)
*/
/** @} */

/**
 * @name UART 0 (8250) Registers
 *
 * @{
 */

#define TEGRAK1_UART0_BASE       (TEGRAK1_PERIPHERAL_BASE + 0x201000)

#define TEGRAK1_UART0_DR         (TEGRAK1_UART0_BASE + 0x00)
#define TEGRAK1_UART0_RSRECR     (TEGRAK1_UART0_BASE + 0x04)
#define TEGRAK1_UART0_FR         (TEGRAK1_UART0_BASE + 0x18)
#define TEGRAK1_UART0_ILPR       (TEGRAK1_UART0_BASE + 0x20)
#define TEGRAK1_UART0_IBRD       (TEGRAK1_UART0_BASE + 0x24)
#define TEGRAK1_UART0_FBRD       (TEGRAK1_UART0_BASE + 0x28)
#define TEGRAK1_UART0_LCRH       (TEGRAK1_UART0_BASE + 0x2C)
#define TEGRAK1_UART0_CR         (TEGRAK1_UART0_BASE + 0x30)
#define TEGRAK1_UART0_IFLS       (TEGRAK1_UART0_BASE + 0x34)
#define TEGRAK1_UART0_IMSC       (TEGRAK1_UART0_BASE + 0x38)
#define TEGRAK1_UART0_RIS        (TEGRAK1_UART0_BASE + 0x3C)
#define TEGRAK1_UART0_MIS        (TEGRAK1_UART0_BASE + 0x40)
#define TEGRAK1_UART0_ICR        (TEGRAK1_UART0_BASE + 0x44)
#define TEGRAK1_UART0_DMACR      (TEGRAK1_UART0_BASE + 0x48)
#define TEGRAK1_UART0_ITCR       (TEGRAK1_UART0_BASE + 0x80)
#define TEGRAK1_UART0_ITIP       (TEGRAK1_UART0_BASE + 0x84)
#define TEGRAK1_UART0_ITOP       (TEGRAK1_UART0_BASE + 0x88)
#define TEGRAK1_UART0_TDR        (TEGRAK1_UART0_BASE + 0x8C)

#define TEGRAK1_UART0_MIS_RX    0x10
#define TEGRAK1_UART0_MIS_TX    0x20
#define TEGRAK1_UART0_IMSC_RX   0x10
#define TEGRAK1_UART0_IMSC_TX   0x20
#define TEGRAK1_UART0_FR_RXFE   0x10
#define TEGRAK1_UART0_FR_TXFF   0x20
#define TEGRAK1_UART0_ICR_RX    0x10
#define TEGRAK1_UART0_ICR_TX    0x20

/** @} */

/**
 * @name I2C (BSC) Registers
 *
 * @{
 */

#define TEGRAK1_I2C_BASE           (TEGRAK1_PERIPHERAL_BASE + 0x804000)

#define TEGRAK1_I2C_C              (TEGRAK1_I2C_BASE + 0x00)
#define TEGRAK1_I2C_S              (TEGRAK1_I2C_BASE + 0x04)
#define TEGRAK1_I2C_DLEN           (TEGRAK1_I2C_BASE + 0x08)
#define TEGRAK1_I2C_A              (TEGRAK1_I2C_BASE + 0x0C)
#define TEGRAK1_I2C_FIFO           (TEGRAK1_I2C_BASE + 0x10)
#define TEGRAK1_I2C_DIV            (TEGRAK1_I2C_BASE + 0x14)
#define TEGRAK1_I2C_DEL            (TEGRAK1_I2C_BASE + 0x18)
#define TEGRAK1_I2C_CLKT           (TEGRAK1_I2C_BASE + 0x1C)

/** @} */

/**
 * @name SPI Registers
 *
 * @{
 */

#define TEGRAK1_SPI_BASE           (TEGRAK1_PERIPHERAL_BASE + 0x204000)

#define TEGRAK1_SPI_CS             (TEGRAK1_SPI_BASE + 0x00)
#define TEGRAK1_SPI_FIFO           (TEGRAK1_SPI_BASE + 0x04)
#define TEGRAK1_SPI_CLK            (TEGRAK1_SPI_BASE + 0x08)
#define TEGRAK1_SPI_DLEN           (TEGRAK1_SPI_BASE + 0x0C)
#define TEGRAK1_SPI_LTOH           (TEGRAK1_SPI_BASE + 0x10)
#define TEGRAK1_SPI_DC             (TEGRAK1_SPI_BASE + 0x14)

/** @} */

/**
 * @name I2C/SPI slave BSC Registers
 *
 * @{
 */

#define TEGRAK1_I2C_SPI_BASE       (TEGRAK1_PERIPHERAL_BASE + 0x214000)

#define TEGRAK1_I2C_SPI_DR         (TEGRAK1_I2C_SPI_BASE + 0x00)
#define TEGRAK1_I2C_SPI_RSR        (TEGRAK1_I2C_SPI_BASE + 0x04)
#define TEGRAK1_I2C_SPI_SLV        (TEGRAK1_I2C_SPI_BASE + 0x08)
#define TEGRAK1_I2C_SPI_CR         (TEGRAK1_I2C_SPI_BASE + 0x0C)
#define TEGRAK1_I2C_SPI_FR         (TEGRAK1_I2C_SPI_BASE + 0x10)
#define TEGRAK1_I2C_SPI_IFLS       (TEGRAK1_I2C_SPI_BASE + 0x14)
#define TEGRAK1_I2C_SPI_IMSC       (TEGRAK1_I2C_SPI_BASE + 0x18)
#define TEGRAK1_I2C_SPI_RIS        (TEGRAK1_I2C_SPI_BASE + 0x1C)
#define TEGRAK1_I2C_SPI_MIS        (TEGRAK1_I2C_SPI_BASE + 0x20)
#define TEGRAK1_I2C_SPI_ICR        (TEGRAK1_I2C_SPI_BASE + 0x24)
#define TEGRAK1_I2C_SPI_DMACR      (TEGRAK1_I2C_SPI_BASE + 0x28)
#define TEGRAK1_I2C_SPI_TDR        (TEGRAK1_I2C_SPI_BASE + 0x2C)
#define TEGRAK1_I2C_SPI_GPUSTAT    (TEGRAK1_I2C_SPI_BASE + 0x30)
#define TEGRAK1_I2C_SPI_HCTRL      (TEGRAK1_I2C_SPI_BASE + 0x34)

/** @} */

/**
 * @name IRQ Registers
 *
 * @{
 */

#define TEGRAK1_BASE_INTC         (TEGRAK1_PERIPHERAL_BASE + 0xB200)

#define TEGRAK1_IRQ_BASIC         (TEGRAK1_BASE_INTC + 0x00)
#define TEGRAK1_IRQ_PENDING1      (TEGRAK1_BASE_INTC + 0x04)
#define TEGRAK1_IRQ_PENDING2      (TEGRAK1_BASE_INTC + 0x08)
#define TEGRAK1_IRQ_FIQ_CTRL      (TEGRAK1_BASE_INTC + 0x0C)
#define TEGRAK1_IRQ_ENABLE1       (TEGRAK1_BASE_INTC + 0x10)
#define TEGRAK1_IRQ_ENABLE2       (TEGRAK1_BASE_INTC + 0x14)
#define TEGRAK1_IRQ_ENABLE_BASIC  (TEGRAK1_BASE_INTC + 0x18)
#define TEGRAK1_IRQ_DISABLE1      (TEGRAK1_BASE_INTC + 0x1C)
#define TEGRAK1_IRQ_DISABLE2      (TEGRAK1_BASE_INTC + 0x20)
#define TEGRAK1_IRQ_DISABLE_BASIC (TEGRAK1_BASE_INTC + 0x24)

/** @} */

/**
 * @name GPU Timer Registers
 *
 * @{
 */

/**
 * NOTE: The GPU uses Compare registers 0 and 2 for
 *       it's own RTOS. 1 and 3 are available for use in
 *       RTEMS.
 */
#define TEGRAK1_GPU_TIMER_BASE    (TEGRAK1_PERIPHERAL_BASE + 0x3000)

#define TEGRAK1_GPU_TIMER_CS      (TEGRAK1_GPU_TIMER_BASE + 0x00)
#define TEGRAK1_GPU_TIMER_CS_M0   0x00000001
#define TEGRAK1_GPU_TIMER_CS_M1   0x00000002
#define TEGRAK1_GPU_TIMER_CS_M2   0x00000004
#define TEGRAK1_GPU_TIMER_CS_M3   0x00000008
#define TEGRAK1_GPU_TIMER_CLO     (TEGRAK1_GPU_TIMER_BASE + 0x04)
#define TEGRAK1_GPU_TIMER_CHI     (TEGRAK1_GPU_TIMER_BASE + 0x08)
#define TEGRAK1_GPU_TIMER_C0      (TEGRAK1_GPU_TIMER_BASE + 0x0C)
#define TEGRAK1_GPU_TIMER_C1      (TEGRAK1_GPU_TIMER_BASE + 0x10)
#define TEGRAK1_GPU_TIMER_C2      (TEGRAK1_GPU_TIMER_BASE + 0x14)
#define TEGRAK1_GPU_TIMER_C3      (TEGRAK1_GPU_TIMER_BASE + 0x18)

/** @} */

/**
 * @name EMMC Registers
 *
 * @{
 */

/**
 * NOTE: Since the SD controller follows the SDHCI standard,
 *       the rtems-libbsd tree already provides the remaining registers.
 */

#define TEGRAK1_EMMC_BASE              (TEGRAK1_PERIPHERAL_BASE + 0x300000)

/** @} */

/**
* @name Mailbox Registers
*
* @{
*/

#define TEGRAK1_MBOX_BASE   (TEGRAK1_PERIPHERAL_BASE+0xB880)

#define TEGRAK1_MBOX_PEEK   (TEGRAK1_MBOX_BASE+0x10)
#define TEGRAK1_MBOX_READ   (TEGRAK1_MBOX_BASE+0x00)
#define TEGRAK1_MBOX_WRITE  (TEGRAK1_MBOX_BASE+0x20)
#define TEGRAK1_MBOX_STATUS (TEGRAK1_MBOX_BASE+0x18)
#define TEGRAK1_MBOX_SENDER (TEGRAK1_MBOX_BASE+0x14)
#define TEGRAK1_MBOX_CONFIG (TEGRAK1_MBOX_BASE+0x1C)

#define TEGRAK1_MBOX_FULL   0x80000000
#define TEGRAK1_MBOX_EMPTY  0x40000000

/** @} */

/**
* @name Mailbox Channels
*
* @{
*/

/* Power Manager channel */
#define TEGRAK1_MBOX_CHANNEL_PM         0
/* Framebuffer channel */
#define TEGRAK1_MBOX_CHANNEL_FB         1
 /* Virtual UART channel */
#define TEGRAK1_MBOX_CHANNEL_VUART      2
 /* VCHIQ channel */
#define TEGRAK1_MBOX_CHANNEL_VCHIQ      3
 /* LEDs channel */
#define TEGRAK1_MBOX_CHANNEL_LED        4
 /* Button channel */
#define TEGRAK1_MBOX_CHANNEL_BUTTON     5
 /* Touch screen channel */
#define TEGRAK1_MBOX_CHANNEL_TOUCHS     6
/* Property tags (ARM <-> VC) channel */
#define TEGRAK1_MBOX_CHANNEL_PROP_AVC   8
 /* Property tags (VC <-> ARM) channel */
#define TEGRAK1_MBOX_CHANNEL_PROP_VCA   9

/** @} */

/**
* @name USB Registers
*
* @{
*/

#define TEGRAK1_USB_BASE	(TEGRAK1_PERIPHERAL_BASE + 0x980000) /* DTC_OTG USB controller */

#endif /* LIBBSP_ARM_JETSONTK1_H */
