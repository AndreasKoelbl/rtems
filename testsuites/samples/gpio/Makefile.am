
rtems_tests_PROGRAMS = gpio
gpio_SOURCES = init.c

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am


LINK_OBJS = $(gpio_OBJECTS)
LINK_LIBS = $(gpio_LDLIBS)

gpio$(EXEEXT): $(gpio_OBJECTS) $(gpio_DEPENDENCIES)
	@rm -f gpio$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
