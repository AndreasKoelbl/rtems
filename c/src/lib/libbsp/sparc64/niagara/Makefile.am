ACLOCAL_AMFLAGS = -I ../../../../aclocal
EXTRA_DIST =

include $(top_srcdir)/../../../../automake/compile.am
include $(top_srcdir)/../../bsp.am

dist_project_lib_DATA = startup/bsp_specs

DISTCLEANFILES = include/bspopts.h
noinst_PROGRAMS =



EXTRA_DIST += ../shared/start/start.S
start.$(OBJEXT): ../shared/start/start.S
	$(CPPASCOMPILE) -o $@ -c $<
project_lib_DATA = start.$(OBJEXT)

dist_project_lib_DATA += ../shared/startup/linkcmds

startup_SOURCES = startup/bspclean.c startup/m5op_sparc.S \
    ../../shared/bsppredriverhook.c \
    ../../shared/bootcard.c ../../shared/sbrk.c \
    ../../shared/getentropy-cpucounter.c \
    ../../shared/bspgetworkarea.c \
    ../../shared/bspstart.c \
    ../../shared/bspreset.c \
    ../shared/startup/setvec.c \
    ../../shared/gnatinstallhandler.c  \
    ../shared/helenos/boot/genarch/balloc.c \
    ../shared/helenos/boot/genarch/ofw.c \
    ../shared/helenos/boot/genarch/ofw_tree.c \
    ../shared/helenos/boot/generic/string.c \
    ../shared/helenos/boot/sparc64/loader/ofwarch.c \
    ../shared/helenos/boot/sparc64/loader/main.c \
    ../shared/helenos/boot/sparc64/loader/ofwasm.S \
    start/bspinit.S \
    ../shared/asm/asm.S

#clock_SOURCES = ../../shared/clock_driver_simidle.c
clock_SOURCES = ../shared/clock/ckinit.c

console_SOURCES = ../../shared/console.c  ../shared/console/conscfg.c \
    ../../shared/console_select.c ../../shared/console_control.c \
    ../../shared/console_read.c ../../shared/console_write.c

timer_SOURCES = ../../shared/timerstub.c

noinst_LIBRARIES = libbsp.a
libbsp_a_SOURCES = $(startup_SOURCES) $(clock_SOURCES) $(console_SOURCES) \
    $(timer_SOURCES)

libbsp_a_SOURCES += ../../../../../../bsps/shared/cache/nocache.c

libbsp_a_LIBADD = \
      ../../../libcpu/@RTEMS_CPU@/shared/shared-score.rel \
      ../../../libcpu/@RTEMS_CPU@/shared/sparc64-syscall.rel 

include $(top_srcdir)/../../../../automake/local.am
include $(srcdir)/../../../../../../bsps/sparc64/niagara/headers.am
