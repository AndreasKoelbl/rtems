
rtems_tests_PROGRAMS = timer_test
timer_test_SOURCES = init.c system.h

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(timer_test_OBJECTS)
LINK_LIBS = $(timer_test_LDLIBS)

timer_test$(EXEEXT): $(timer_test_OBJECTS) $(timer_test_DEPENDENCIES)
	@rm -f timer_test$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
