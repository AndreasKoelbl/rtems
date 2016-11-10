ACLOCAL_AMFLAGS = -I ../../../../aclocal

include $(top_srcdir)/../../../../automake/compile.am
include $(top_srcdir)/../../bsp.am

include_bspdir = $(includedir)/bsp

###############################################################################
#                  Data                                                       #
###############################################################################

dist_project_lib_DATA = bsp_specs

noinst_LIBRARIES = libbsp.a

include_HEADERS = include/bsp.h
include_HEADERS += ../../shared/include/tm27.h

nodist_include_HEADERS = include/bspopts.h
nodist_include_HEADERS += ../../shared/include/coverhd.h
nodist_include_bsp_HEADERS = ../../shared/include/bootcard.h

DISTCLEANFILES = include/bspopts.h
noinst_PROGRAMS =

dist_project_lib_DATA += startup/linkcmds

libbsp_a_SOURCES =

# startup
libbsp_a_SOURCES = ../../shared/bspclean.c ../../shared/bsppredriverhook.c
libbsp_a_SOURCES += startup/bspstart.c ../../shared/bootcard.c
libbsp_a_SOURCES += ../../shared/sbrk.c
libbsp_a_SOURCES += startup/setvec.c ../../shared/gnatinstallhandler.c
libbsp_a_SOURCES += ../../shared/bspgetworkarea.c

# clock
libbsp_a_SOURCES += clock/clockdrv.c
# console
libbsp_a_SOURCES += console/console.c
libbsp_a_SOURCES += ../../shared/console-termios.c

# timer
libbsp_a_SOURCES += timer/timer.c timer/timerisr.c

# shmsupp
libbsp_a_SOURCES += shmsupp/addrconv.c shmsupp/getcfg.c shmsupp/lock.c \
    shmsupp/mpisr.c

###############################################################################
#                  Special Rules                                              #
###############################################################################

EXTRA_DIST = times

include $(srcdir)/preinstall.am
include $(top_srcdir)/../../../../automake/local.am
