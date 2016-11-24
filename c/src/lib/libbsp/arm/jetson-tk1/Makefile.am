ACLOCAL_AMFLAGS = -I ../../../../aclocal

include $(top_srcdir)/../../../../automake/compile.am

###############################################################################
#                  Data                                                       #
###############################################################################

dist_project_lib_DATA = bsp_specs

noinst_LIBRARIES = libbspstart.a

libbspstart_a_SOURCES =
libbspstart_a_SOURCES += ../shared/start/start.S

project_lib_DATA =
project_lib_DATA += start.$(OBJEXT)
#project_lib_DATA += startup/linkcmds
project_lib_DATA += startup/linkcmds

EXTRA_DIST = startup/linkcmds

include_libchipdir = $(includedir)/libchip
include_libchip_includedir = $(includedir)/libchip/include
include_libcpudir = $(includedir)/libcpu
include_somethin = $(includedir)/lib

###############################################################################
#                  Header                                                     #
###############################################################################

include_HEADERS =
include_HEADERS += ../../shared/include/tm27.h
include_HEADERS += ../shared/CMSIS/Include/core_cm7.h
include_HEADERS += ../shared/CMSIS/Include/core_cmInstr.h
include_HEADERS += ../shared/CMSIS/Include/core_cmFunc.h
include_HEADERS += ../shared/CMSIS/Include/core_cmSimd.h
include_HEADERS += ../shared/CMSIS/Include/cmsis_gcc.h
include_HEADERS += include/bsp.h
include_HEADERS += include/console.h

nodist_include_HEADERS =
nodist_include_HEADERS += ../../shared/include/coverhd.h
nodist_include_HEADERS += include/bspopts.h

include_bspdir = $(includedir)/bsp

nodist_include_bsp_HEADERS =
nodist_include_bsp_HEADERS += ../../shared/include/bootcard.h

include_bsp_HEADERS =
include_bsp_HEADERS += ../../shared/include/utility.h
include_bsp_HEADERS += ../../shared/include/irq-generic.h
include_bsp_HEADERS += ../../shared/include/irq-info.h
include_bsp_HEADERS += ../../shared/include/stackalloc.h
include_bsp_HEADERS += ../shared/include/start.h
include_bsp_HEADERS += ../shared/include/arm-gic.h
include_bsp_HEADERS += ../shared/include/arm-gic-regs.h
include_bsp_HEADERS += ../shared/armv7m/include/armv7m-irq.h
include_bsp_HEADERS += ../shared/include/arm-gic-irq.h

include_libcpu_HEADERS =
include_libcpu_HEADERS += ../../../libcpu/arm/shared/include/arm-cp15.h

nodist_include_bsp_HEADERS += include/irq.h
nodist_include_bsp_HEADERS += include/shm_driver.h
nodist_include_bsp_HEADERS += include/gic.h
nodist_include_bsp_HEADERS += include/jetson-tk1.h
nodist_include_bsp_HEADERS += include/memaccess.h


noinst_PROGRAMS =


# startup
libbsp_a_SOURCES = startup/bspstart.c
libbsp_a_SOURCES += startup/bspstarthooks.c
libbsp_a_SOURCES += startup/setvec.c
libbsp_a_SOURCES += startup/bspreset.c
libbsp_a_SOURCES += ../../shared/bootcard.c
libbsp_a_SOURCES += ../../shared/bspclean.c
libbsp_a_SOURCES += ../../shared/bsppredriverhook.c
libbsp_a_SOURCES += ../../shared/sbrk.c
libbsp_a_SOURCES += ../../shared/gnatinstallhandler.c
libbsp_a_SOURCES += ../../shared/bspgetworkarea.c

# Clock
libbsp_a_SOURCES += clock/clockdrv.c

# Console
libbsp_a_SOURCES += console/console.c
libbsp_a_SOURCES += console/debug-console.c
libbsp_a_SOURCES += ../../shared/console-termios.c

# Timer
libbsp_a_SOURCES += timer/timer.c timer/timerisr.c

# IRQ
libbsp_a_SOURCES += ../../shared/src/irq-default-handler.c
libbsp_a_SOURCES += ../../shared/src/irq-generic.c
libbsp_a_SOURCES += ../../shared/src/irq-info.c
libbsp_a_SOURCES += ../../shared/src/irq-legacy.c
libbsp_a_SOURCES += ../../shared/src/irq-server.c
libbsp_a_SOURCES += ../../shared/src/irq-shell.c
libbsp_a_SOURCES += ../shared/armv7m/irq/armv7m-irq.c
libbsp_a_SOURCES += ../shared/armv7m/irq/armv7m-irq-dispatch.c
libbsp_a_SOURCES += ../shared/arm-gic-irq.c

###############################################################################
#                  LibBSP                                                     #
###############################################################################

noinst_LIBRARIES += libbsp.a

###############################################################################
#                  Special Rules                                              #
###############################################################################

EXTRA_DIST += times
DISTCLEANFILES = include/bspopts.h

include $(srcdir)/preinstall.am
include $(top_srcdir)/../../../../automake/local.am
