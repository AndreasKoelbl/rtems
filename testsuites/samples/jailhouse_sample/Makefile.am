
rtems_tests_PROGRAMS = jailhouse_sample
jailhouse_sample_SOURCES = init.c

dist_rtems_tests_DATA = jailhouse_sample.scn
dist_rtems_tests_DATA += jailhouse_sample.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am


LINK_OBJS = $(jailhouse_sample_OBJECTS)
LINK_LIBS = $(jailhouse_sample_LDLIBS)

jailhouse_sample$(EXEEXT): $(jailhouse_sample_OBJECTS) $(jailhouse_sample_DEPENDENCIES)
	@rm -f jailhouse_sample$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
