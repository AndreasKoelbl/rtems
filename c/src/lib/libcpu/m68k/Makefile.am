ACLOCAL_AMFLAGS = -I ../../../aclocal

include $(top_srcdir)/../../../automake/compile.am

noinst_PROGRAMS =

if shared

## shared/misc

# gcc doesn't recognize difference between the cpu32 and cpu32+ so we have to
if mcpu32p
M68K_CPPFLAGS = -D__mcpu32p__
endif
if mcf5272
M68K_CPPFLAGS = -Dmcf5272
endif

noinst_PROGRAMS += shared/misc.rel
shared_misc_rel_SOURCES = shared/misc/m68kidle.c shared/misc/memProbe.c
shared_misc_rel_CPPFLAGS = $(AM_CPPFLAGS) $(M68K_CPPFLAGS)
shared_misc_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)
endif

if m68040
## m68040/fpsp
noinst_PROGRAMS += m68040/fpsp.rel
noinst_PROGRAMS += m68040/fpsp_tmp.rel
m68040_fpsp_tmp_rel_SOURCES = m68040/fpsp/rtems_fpsp.c m68040/fpsp/bindec.S \
    m68040/fpsp/binstr.S m68040/fpsp/bugfix.S m68040/fpsp/decbin.S \
    m68040/fpsp/do_func.S m68040/fpsp/gen_except.S m68040/fpsp/get_op.S \
    m68040/fpsp/kernel_ex.S m68040/fpsp/res_func.S m68040/fpsp/round.S \
    m68040/fpsp/rtems_skel.S m68040/fpsp/sacos.S m68040/fpsp/sasin.S \
    m68040/fpsp/satan.S m68040/fpsp/satanh.S m68040/fpsp/scale.S \
    m68040/fpsp/scosh.S m68040/fpsp/setox.S m68040/fpsp/sgetem.S \
    m68040/fpsp/sint.S m68040/fpsp/slog2.S m68040/fpsp/slogn.S \
    m68040/fpsp/smovecr.S m68040/fpsp/srem_mod.S m68040/fpsp/ssin.S \
    m68040/fpsp/ssinh.S m68040/fpsp/stan.S m68040/fpsp/stanh.S \
    m68040/fpsp/sto_res.S m68040/fpsp/stwotox.S m68040/fpsp/tbldo.S \
    m68040/fpsp/util.S m68040/fpsp/x_bsun.S m68040/fpsp/x_fline.S \
    m68040/fpsp/x_operr.S m68040/fpsp/x_ovfl.S m68040/fpsp/x_snan.S \
    m68040/fpsp/x_store.S m68040/fpsp/x_unfl.S m68040/fpsp/x_unimp.S \
    m68040/fpsp/x_unsupp.S
m68040_fpsp_tmp_rel_CPPFLAGS = $(AM_CPPFLAGS)
m68040_fpsp_tmp_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

# avoid name clashes and modifying the source by
# prefixing all (global) symbols in the binary
m68040/fpsp_tmp1.rel: m68040/fpsp_tmp.rel
	$(OBJCOPY) --prefix-symbols __fpsp_ $^ $@

# Then remove the prefix again on all global
# symbols we really need:
m68040/fpsp.rel: m68040/fpsp_tmp1.rel
	    $(OBJCOPY) --redefine-sym __fpsp_M68KFPSPInstallExceptionHandlers=M68KFPSPInstallExceptionHandlers --redefine-sym __fpsp_M68040FPSPUserExceptionHandlers=M68040FPSPUserExceptionHandlers --redefine-sym __fpsp__CPU_ISR_install_raw_handler=_CPU_ISR_install_raw_handler --redefine-sym __fpsp__FPSP_install_raw_handler=_FPSP_install_raw_handler $^ $@

noinst_HEADERS = m68040/fpsp/fpsp.defs
endif
EXTRA_DIST = m68040/fpsp/README

if mcf5206
# mcf5206/include
## mcf5206/clock
noinst_PROGRAMS += mcf5206/clock.rel
mcf5206_clock_rel_SOURCES = mcf5206/clock/ckinit.c
mcf5206_clock_rel_CPPFLAGS = $(AM_CPPFLAGS)
mcf5206_clock_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

## mcf5206/mcfuart
noinst_PROGRAMS += mcf5206/mcfuart.rel
mcf5206_mcfuart_rel_SOURCES = mcf5206/console/mcfuart.c
mcf5206_mcfuart_rel_CPPFLAGS = $(AM_CPPFLAGS)
mcf5206_mcfuart_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

## mcf5206/mbus
noinst_PROGRAMS += mcf5206/mbus.rel
mcf5206_mbus_rel_SOURCES = mcf5206/mbus/mcfmbus.c
mcf5206_mbus_rel_CPPFLAGS = $(AM_CPPFLAGS)
mcf5206_mbus_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

## mcf5206/timer
noinst_PROGRAMS += mcf5206/timer.rel
mcf5206_timer_rel_SOURCES = mcf5206/timer/timer.c mcf5206/timer/timerisr.S
mcf5206_timer_rel_CPPFLAGS = $(AM_CPPFLAGS)
mcf5206_timer_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)
endif

if mcf5272
## mcf5272/include
## clock
noinst_PROGRAMS += mcf5272/clock.rel
mcf5272_clock_rel_SOURCES = mcf5272/clock/ckinit.c
mcf5272_clock_rel_CPPFLAGS = $(AM_CPPFLAGS)
mcf5272_clock_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

## timer
noinst_PROGRAMS += mcf5272/timer.rel
mcf5272_timer_rel_SOURCES = mcf5272/timer/timer.c mcf5272/timer/timerisr.S
mcf5272_timer_rel_CPPFLAGS = $(AM_CPPFLAGS)
mcf5272_timer_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)
endif

if mcf548x
## mcf548x/include
## mcf548x/mcdma
noinst_PROGRAMS += mcf548x/mcdma.rel
mcf548x_mcdma_rel_SOURCES = mcf548x/mcdma/MCD_dmaApi.c mcf548x/mcdma/MCD_tasksInit.c \
                            mcf548x/mcdma/MCD_tasks.c  mcf548x/mcdma/mcdma_glue.c

mcf548x_mcdma_rel_CPPFLAGS = $(AM_CPPFLAGS)
mcf548x_mcdma_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)
endif

include $(top_srcdir)/../../../automake/local.am
