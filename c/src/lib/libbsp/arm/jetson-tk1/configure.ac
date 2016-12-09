AC_PREREQ([2.68])
AC_INIT([rtems-c-src-lib-libbsp-arm-jetson-tk1],[_RTEMS_VERSION],[https://devel.rtems.org/newticket])
AC_CONFIG_SRCDIR([bsp_specs])
RTEMS_TOP(../../../../../..)

RTEMS_CANONICAL_TARGET_CPU
AM_INIT_AUTOMAKE([no-define nostdinc foreign 1.12.2])
RTEMS_BSP_CONFIGURE

RTEMS_PROG_CC_FOR_TARGET
RTEMS_CANONICALIZE_TOOLS
RTEMS_PROG_CCAS

RTEMS_BSPOPTS_SET([NS8250_CONSOLE_USE_INTERRUPTS],[*],[0])
RTEMS_BSPOPTS_HELP([NS8250_CONSOLE_USE_INTERRUPTS],[Use interrupt driven mode for console devices])

RTEMS_BSPOPTS_SET([NS8250_USE_SECONDARY_CONSOLE],[*],[0])
RTEMS_BSPOPTS_HELP([NS8250_USE_SECONDARY_CONSOLE],[Enable a secondary console called /dev/ttyS1 using the UART A which is located at Pin 41 (TX) and Pin 44 (RX) on the J3A2])

RTEMS_BSPOPTS_SET([JAILHOUSE_ENABLE],[*],[0])
RTEMS_BSPOPTS_HELP([JAILHOUSE_ENABLE],[Enable the jailhouse hypervisor])

JETSON_BASE="0x90000000"
AS_IF([test "${JAILHOUSE_ENABLE}" == "1"],
      [JETSON_BASE="0x00000000"])

AC_DEFUN([JETSON_LINKCMD],[
AC_ARG_VAR([$1],[$2; default $3])dnl
[$1]=[$]{[$1]:-[$3]}
])

JETSON_LINKCMD([JETSON_BASE],[Begin of RAM region],[${JETSON_BASE}])

AM_CONDITIONAL(HAS_SMP,[test "$rtems_cv_HAS_SMP" = "yes"])

RTEMS_BSP_CLEANUP_OPTIONS(0, 0)

AC_CONFIG_FILES([
Makefile
startup/linkcmds])
AC_OUTPUT
