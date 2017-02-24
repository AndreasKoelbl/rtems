rtems_tests_PROGRAMS = task_test
task_test_SOURCES = init.c system.h
task_test_LDLIBS = -lm

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am


LINK_OBJS = $(task_test_OBJECTS)
LINK_LIBS = $(task_test_LDLIBS)

AM_CPPFLAGS += -I$(top_srcdir)/../support/include

task_test$(EXEEXT): $(task_test_OBJECTS) $(task_test_DEPENDENCIES)
	@rm -f task_test$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
