
rtems_tests_PROGRAMS = interrupt_latency
interrupt_latency_SOURCES = init.c

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am


LINK_OBJS = $(interrupt_latency_OBJECTS)
LINK_LIBS = $(interrupt_latency_LDLIBS)

interrupt_latency$(EXEEXT): $(interrupt_latency_OBJECTS) $(interrupt_latency_DEPENDENCIES)
	@rm -f interrupt_latency$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
