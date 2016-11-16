ACLOCAL_AMFLAGS = -I ../../../../aclocal

include $(top_srcdir)/../../../../automake/compile.am

###############################################################################
#                  Data                                                       #
###############################################################################

#include $(top_srcdir)/../../bsp.am

libbspstart_a_SOURCES = ../shared/start/start.S

dist_project_lib_DATA =
dist_project_lib_DATA = bsp_specs
dist_project_lib_DATA += start.$(OBJEXT)
dist_project_lib_DATA += startup/linkcmds

include_bspdir = $(includedir)/bsp
include_libchipdir = $(includedir)/libchip
include_libchip_includedir = $(includedir)/libchip/include
include_libcpudir = $(includedir)/libcpu
include_somethin = $(includedir)/lib

###############################################################################
#                  Header                                                     #
###############################################################################

include_HEADERS = include/bsp.h
include_HEADERS += ../../shared/include/tm27.h

nodist_include_HEADERS = ../../shared/include/coverhd.h include/bspopts.h

nodist_include_bsp_HEADERS =
nodist_include_bsp_HEADERS += ../../shared/include/bootcard.h

nodist_include_bsp_HEADERS += ../../shared/include/irq-generic.h
nodist_include_bsp_HEADERS += ../shared/include/start.h
nodist_include_bsp_HEADERS += include/irq.h
nodist_include_bsp_HEADERS += include/shm_driver.h
nodist_include_bsp_HEADERS += include/gic.h
nodist_include_bsp_HEADERS += include/jetson-tk1.h
nodist_include_bsp_HEADERS += include/memaccess.h


noinst_PROGRAMS =


# startup
libbsp_a_SOURCES = ../../shared/bspclean.c ../../shared/bsppredriverhook.c
libbsp_a_SOURCES += startup/bspstart.c ../../shared/bootcard.c
libbsp_a_SOURCES += ../../shared/sbrk.c
libbsp_a_SOURCES += startup/setvec.c ../../shared/gnatinstallhandler.c
libbsp_a_SOURCES += ../../shared/bspgetworkarea.c

# Clock
libbsp_a_SOURCES += clock/clockdrv.c

# Console
libbsp_a_SOURCES += console/console.c
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

###############################################################################
#                  LibBSP                                                     #
###############################################################################

noinst_LIBRARIES = libbsp.a

###############################################################################
#                  Special Rules                                              #
###############################################################################

EXTRA_DIST = times
DISTCLEANFILES = include/bspopts.h

include $(srcdir)/preinstall.am
include $(top_srcdir)/../../../../automake/local.am
