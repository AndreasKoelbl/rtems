
rtems_tests_PROGRAMS = gpio_test
gpio_test_SOURCES = init.c

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am


LINK_OBJS = $(gpio_test_OBJECTS)
LINK_LIBS = $(gpio_test_LDLIBS)

gpio_test$(EXEEXT): $(gpio_test_OBJECTS) $(gpio_test_DEPENDENCIES)
	@rm -f gpio_test$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
