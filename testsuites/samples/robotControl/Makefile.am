
rtems_tests_PROGRAMS = robotControl
robotControl_SOURCES = init.c

dist_rtems_tests_DATA = robotControl.scn
dist_rtems_tests_DATA += robotControl.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am


LINK_OBJS = $(robotControl_OBJECTS)
LINK_LIBS = $(robotControl_LDLIBS)

robotControl$(EXEEXT): $(robotControl_OBJECTS) $(robotControl_DEPENDENCIES)
	@rm -f robotControl$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
