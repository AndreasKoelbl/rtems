ACLOCAL_AMFLAGS = -I ../../../aclocal

include $(top_srcdir)/../../../automake/compile.am

EXTRA_DIST =

noinst_PROGRAMS = new-exceptions/rtems-cpu.rel
new_exceptions_rtems_cpu_rel_SOURCES = new-exceptions/cpu.c \
    new-exceptions/cpu_asm.S
new_exceptions_rtems_cpu_rel_CPPFLAGS = $(AM_CPPFLAGS)
new_exceptions_rtems_cpu_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)


if !mpc5xx
noinst_PROGRAMS += new-exceptions/exc_bspsupport.rel
new_exceptions_exc_bspsupport_rel_SOURCES = \
    new-exceptions/bspsupport/ppc_exc_async_normal.S \
    new-exceptions/bspsupport/ppc_exc_fatal.S \
    new-exceptions/bspsupport/ppc_exc_print.c
if !qoriq
new_exceptions_exc_bspsupport_rel_SOURCES += \
    new-exceptions/bspsupport/ppc-code-copy.c \
    new-exceptions/bspsupport/ppc_exc.S \
    new-exceptions/bspsupport/ppc_exc_naked.S \
    new-exceptions/bspsupport/ppc_exc_hdl.c \
    new-exceptions/bspsupport/ppc_exc_initialize.c \
    new-exceptions/bspsupport/ppc_exc_global_handler.c \
    new-exceptions/bspsupport/ppc_exc_categories.c \
    new-exceptions/bspsupport/ppc_exc_address.c \
    new-exceptions/bspsupport/ppc_exc_alignment.c \
    new-exceptions/bspsupport/ppc_exc_prologue.c
endif

new_exceptions_exc_bspsupport_rel_CPPFLAGS = $(AM_CPPFLAGS)
new_exceptions_exc_bspsupport_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

noinst_PROGRAMS += new-exceptions/irq_bspsupport.rel

new_exceptions_irq_bspsupport_rel_SOURCES = new-exceptions/bspsupport/irq.c
new_exceptions_irq_bspsupport_rel_CPPFLAGS = $(AM_CPPFLAGS)
new_exceptions_irq_bspsupport_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)
endif

EXTRA_DIST += new-exceptions/bspsupport/README
EXTRA_DIST += new-exceptions/bspsupport/ppc_exc_test.c

# shared/include
if shared
noinst_PROGRAMS += shared/cpuIdent.rel
shared_cpuIdent_rel_SOURCES = shared/include/cpuIdent.c shared/include/cpuIdent.h
shared_cpuIdent_rel_CPPFLAGS = $(AM_CPPFLAGS)
shared_cpuIdent_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# shared/stack
noinst_PROGRAMS += shared/stack.rel
shared_stack_rel_SOURCES = shared/src/stack.c shared/include/spr.h shared/src/stackTrace.h
shared_stack_rel_CPPFLAGS = $(AM_CPPFLAGS)
shared_stack_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

endif

EXTRA_DIST += ppc403/README ppc403/vectors/README
if ppc4xx
# ppc403/clock
noinst_PROGRAMS += ppc403/clock.rel
ppc403_clock_rel_SOURCES = ppc403/clock/clock.c
ppc403_clock_rel_CPPFLAGS = $(AM_CPPFLAGS)
ppc403_clock_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# ppc403/console
noinst_PROGRAMS += ppc403/console.rel
if ppc405
ppc403_console_rel_SOURCES = ppc403/console/console405.c
else
ppc403_console_rel_SOURCES = ppc403/console/console.c
endif
ppc403_console_rel_CPPFLAGS = $(AM_CPPFLAGS)
ppc403_console_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

shared_cpuIdent_rel_SOURCES = shared/include/cpuIdent.c
shared_cpuIdent_rel_CPPFLAGS = $(AM_CPPFLAGS)
shared_cpuIdent_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# ppc403/timer
noinst_PROGRAMS += ppc403/timer.rel
ppc403_timer_rel_SOURCES = ppc403/timer/timer.c
ppc403_timer_rel_CPPFLAGS = $(AM_CPPFLAGS)
ppc403_timer_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# ppc403/tty_drv
if ppc405
noinst_PROGRAMS += ppc403/tty_drv.rel
ppc403_tty_drv_rel_SOURCES = ppc403/tty_drv/tty_drv.c ppc403/tty_drv/tty_drv.h
ppc403_tty_drv_rel_CPPFLAGS = $(AM_CPPFLAGS)
ppc403_tty_drv_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)
endif

endif # ppc4xx

if ppc405

## ppc4xx/include
endif # ppc405


## mpc5xx
EXTRA_DIST += mpc5xx/README
if mpc5xx
# mpc5xx/clock
noinst_PROGRAMS += mpc5xx/clock.rel
mpc5xx_clock_rel_SOURCES = mpc5xx/clock/clock.c
mpc5xx_clock_rel_CPPFLAGS = $(AM_CPPFLAGS)
mpc5xx_clock_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# mpc5xx/console-generic
noinst_PROGRAMS += mpc5xx/console-generic.rel
mpc5xx_console_generic_rel_SOURCES = mpc5xx/console-generic/console-generic.c
mpc5xx_console_generic_rel_CPPFLAGS = $(AM_CPPFLAGS)
mpc5xx_console_generic_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# mpc5xx/exceptions
noinst_PROGRAMS += mpc5xx/exceptions.rel
mpc5xx_exceptions_rel_SOURCES = mpc5xx/exceptions/raw_exception.c
mpc5xx_exceptions_rel_CPPFLAGS = $(AM_CPPFLAGS)
mpc5xx_exceptions_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# mpc5xx/irq
noinst_PROGRAMS += mpc5xx/irq.rel
mpc5xx_irq_rel_SOURCES = mpc5xx/irq/irq.c mpc5xx/irq/irq_init.c mpc5xx/irq/irq_asm.S
mpc5xx_irq_rel_CPPFLAGS = $(AM_CPPFLAGS)
mpc5xx_irq_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# mpc5xx/timer
noinst_PROGRAMS += mpc5xx/timer.rel
mpc5xx_timer_rel_SOURCES = mpc5xx/timer/timer.c
mpc5xx_timer_rel_CPPFLAGS = $(AM_CPPFLAGS)
mpc5xx_timer_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# mpc5xx/vectors
noinst_PROGRAMS += mpc5xx/vectors.rel
mpc5xx_vectors_rel_SOURCES = mpc5xx/vectors/vectors_init.c mpc5xx/vectors/vectors.S \
    new-exceptions/bspsupport/ppc_exc_print.c
mpc5xx_vectors_rel_CPPFLAGS = $(AM_CPPFLAGS)
mpc5xx_vectors_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)
endif

if mpc505
# mpc505/ictrl
noinst_PROGRAMS += mpc505/ictrl.rel
mpc505_ictrl_rel_SOURCES = mpc505/ictrl/ictrl.c
mpc505_ictrl_rel_CPPFLAGS = $(AM_CPPFLAGS)
mpc505_ictrl_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# mpc505/timer
noinst_PROGRAMS += mpc505/timer.rel
mpc505_timer_rel_SOURCES = mpc505/timer/timer.c
mpc505_timer_rel_CPPFLAGS = $(AM_CPPFLAGS)
mpc505_timer_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# mpc505/vector
noinst_PROGRAMS += mpc505/vectors.rel
mpc505_vectors_rel_SOURCES = mpc505/vectors/vectors.S
mpc505_vectors_rel_CPPFLAGS = $(AM_CPPFLAGS)
mpc505_vectors_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)
endif

if mpc6xx

# mpc6xx/mmu
noinst_PROGRAMS += mpc6xx/mmu.rel
mpc6xx_mmu_rel_SOURCES = mpc6xx/mmu/bat.c mpc6xx/mmu/bat.h \
    mpc6xx/mmu/pte121.c mpc6xx/mmu/pte121.h \
    mpc6xx/mmu/mmuAsm.S
mpc6xx_mmu_rel_CPPFLAGS = $(AM_CPPFLAGS)
mpc6xx_mmu_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# mpc6xx/clock
noinst_PROGRAMS += mpc6xx/clock.rel
mpc6xx_clock_rel_SOURCES = mpc6xx/clock/c_clock.c mpc6xx/clock/c_clock.h
mpc6xx_clock_rel_CPPFLAGS = $(AM_CPPFLAGS)
mpc6xx_clock_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# mpc6xx/timer
noinst_PROGRAMS += mpc6xx/timer.rel
mpc6xx_timer_rel_SOURCES = mpc6xx/timer/timer.c
mpc6xx_timer_rel_CPPFLAGS = $(AM_CPPFLAGS)
mpc6xx_timer_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# mpc6xx/altivec
noinst_PROGRAMS += mpc6xx/altivec.rel
mpc6xx_altivec_rel_SOURCES = mpc6xx/altivec/vec_sup.c mpc6xx/altivec/vec_sup_asm.S
mpc6xx_altivec_rel_CPPFLAGS = $(AM_CPPFLAGS)
mpc6xx_altivec_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)
endif
EXTRA_DIST      += mpc6xx/altivec/README

# e500/clock
if e500_clock
noinst_PROGRAMS += e500/clock.rel
e500_clock_rel_SOURCES = mpc6xx/clock/c_clock.c mpc6xx/clock/c_clock.h
e500_clock_rel_CPPFLAGS = $(AM_CPPFLAGS)
e500_clock_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)
endif

# e500/timer
if e500_timer
noinst_PROGRAMS += e500/timer.rel
e500_timer_rel_SOURCES = mpc6xx/timer/timer.c
e500_timer_rel_CPPFLAGS = $(AM_CPPFLAGS)
e500_timer_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)
endif

# e500/mmu
if e500_mmu
noinst_PROGRAMS += e500/mmu.rel
e500_mmu_rel_SOURCES = e500/mmu/mmu.c e500/mmu/e500_mmu.h
e500_mmu_rel_CPPFLAGS = $(AM_CPPFLAGS)
e500_mmu_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)
endif

EXTRA_DIST += mpc8xx/README
if mpc8xx
# mpc8xx/clock
noinst_PROGRAMS += mpc8xx/clock.rel
mpc8xx_clock_rel_SOURCES = mpc8xx/clock/clock.c
mpc8xx_clock_rel_CPPFLAGS = $(AM_CPPFLAGS)
mpc8xx_clock_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# mpc8xx/console-generic
noinst_PROGRAMS += mpc8xx/console-generic.rel
mpc8xx_console_generic_rel_SOURCES = mpc8xx/console-generic/console-generic.c
mpc8xx_console_generic_rel_CPPFLAGS = $(AM_CPPFLAGS)
mpc8xx_console_generic_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# mpc8xx/cpm
noinst_PROGRAMS += mpc8xx/cpm.rel
mpc8xx_cpm_rel_SOURCES = mpc8xx/cpm/cp.c mpc8xx/cpm/dpram.c
mpc8xx_cpm_rel_CPPFLAGS = $(AM_CPPFLAGS)
mpc8xx_cpm_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# mpc8xx/mmu
noinst_PROGRAMS += mpc8xx/mmu.rel
mpc8xx_mmu_rel_SOURCES = mpc8xx/mmu/mmu.c
mpc8xx_mmu_rel_CPPFLAGS = $(AM_CPPFLAGS)
mpc8xx_mmu_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# mpc8xx/timer
noinst_PROGRAMS += mpc8xx/timer.rel
mpc8xx_timer_rel_SOURCES = mpc8xx/timer/timer.c
mpc8xx_timer_rel_CPPFLAGS = $(AM_CPPFLAGS)
mpc8xx_timer_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)
endif

EXTRA_DIST += mpc8260/README
if mpc8260
# mpc8260/console-generic
noinst_PROGRAMS += mpc8260/console-generic.rel
mpc8260_console_generic_rel_SOURCES = mpc8260/console-generic/console-generic.c \
    mpc8260/include/console.h
mpc8260_console_generic_rel_CPPFLAGS = $(AM_CPPFLAGS)
mpc8260_console_generic_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# mpc8260/cpm
noinst_PROGRAMS += mpc8260/cpm.rel
mpc8260_cpm_rel_SOURCES = mpc8260/cpm/cp.c mpc8260/cpm/dpram.c mpc8260/cpm/brg.c \
    mpc8260/include/cpm.h
mpc8260_cpm_rel_CPPFLAGS = $(AM_CPPFLAGS)
mpc8260_cpm_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# mpc8260/mmu
noinst_PROGRAMS += mpc8260/mmu.rel
mpc8260_mmu_rel_SOURCES = mpc8260/mmu/mmu.c \
                          mpc8260/include/mmu.h
mpc8260_mmu_rel_CPPFLAGS = $(AM_CPPFLAGS)
mpc8260_mmu_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# mpc8260/timer
noinst_PROGRAMS += mpc8260/timer.rel
mpc8260_timer_rel_SOURCES = mpc8260/timer/timer.c
mpc8260_timer_rel_CPPFLAGS = $(AM_CPPFLAGS)
mpc8260_timer_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)
endif

##############################################################################
# START: MPC83XX                                                             #
##############################################################################
if mpc83xx

# Includes

# Network
if HAS_NETWORKING
noinst_PROGRAMS += mpc83xx/tsec.rel
mpc83xx_tsec_rel_SOURCES = mpc83xx/network/tsec.c
mpc83xx_tsec_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)
endif

# i2c
noinst_PROGRAMS += mpc83xx/i2c.rel
mpc83xx_i2c_rel_SOURCES = mpc83xx/i2c/mpc83xx_i2cdrv.c \
	mpc83xx/i2c/mpc83xx_i2cdrv.h
mpc83xx_i2c_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# SPI
noinst_PROGRAMS += mpc83xx/spi.rel
mpc83xx_spi_rel_SOURCES = mpc83xx/spi/mpc83xx_spidrv.c \
	mpc83xx/spi/mpc83xx_spidrv.h
mpc83xx_spi_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# GTM
noinst_PROGRAMS += mpc83xx/gtm.rel
mpc83xx_gtm_rel_SOURCES = mpc83xx/gtm/gtm.c \
	mpc83xx/include/gtm.h
mpc83xx_gtm_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

endif
##############################################################################
# END: MPC83XX                                                               #
##############################################################################

##############################################################################
# START: MPC55XX                                                             #
##############################################################################
if mpc55xx

# IRQ
noinst_PROGRAMS += mpc55xx/irq.rel
mpc55xx_irq_rel_SOURCES = mpc55xx/irq/irq.c
mpc55xx_irq_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# mpc6xx/timer
noinst_PROGRAMS += mpc55xx/timer.rel
mpc55xx_timer_rel_SOURCES = mpc6xx/timer/timer.c
mpc55xx_timer_rel_CPPFLAGS = $(AM_CPPFLAGS)
mpc55xx_timer_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# FEC
noinst_PROGRAMS += mpc55xx/fec.rel
mpc55xx_fec_rel_SOURCES = mpc55xx/fec/fec.c
mpc55xx_fec_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# eDMA
noinst_PROGRAMS += mpc55xx/edma.rel
mpc55xx_edma_rel_SOURCES = mpc55xx/edma/edma.c
mpc55xx_edma_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# eMIOS
noinst_PROGRAMS += mpc55xx/emios.rel
mpc55xx_emios_rel_SOURCES = mpc55xx/emios/emios.c
mpc55xx_emios_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# SIU
noinst_PROGRAMS += mpc55xx/siu.rel
mpc55xx_siu_rel_SOURCES = mpc55xx/siu/siu.c
mpc55xx_siu_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# DSPI
noinst_PROGRAMS += mpc55xx/dspi.rel
mpc55xx_dspi_rel_SOURCES = mpc55xx/dspi/dspi.c
mpc55xx_dspi_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# Misc
noinst_PROGRAMS += mpc55xx/misc.rel
mpc55xx_misc_rel_SOURCES = mpc55xx/misc/copy.S \
	mpc55xx/misc/flash_support.c
mpc55xx_misc_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

endif
##############################################################################
# END: MPC55XX                                                               #
##############################################################################

##############################################################################
# START: QorIQ                                                               #
##############################################################################
if qoriq

# Network
if HAS_NETWORKING
noinst_PROGRAMS += tsec.rel
tsec_rel_SOURCES = mpc83xx/network/tsec.c
tsec_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)
endif

endif
##############################################################################
# END: QorIQ                                                                 #
##############################################################################

include $(top_srcdir)/../../../automake/local.am
