AC_PREREQ([2.60])
AC_INIT([libh2o], [0.2.1],
		[https://bitbucket.org/mgorny/libh2o/issues/],
		[libh2o],
		[https://bitbucket.org/mgorny/libh2o/])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.6 foreign dist-bzip2 subdir-objects])

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES])

AC_PROG_CC
AC_C_INLINE

LT_INIT([disable-static win32-dll])

AC_ARG_ENABLE([debug],
	[AS_HELP_STRING([--enable-debug],
		[Enable debugging assertions])])

AS_IF([test x"$enable_debug" != x"yes"], [
	AC_DEFINE([NDEBUG], [1], [define to disable assertions])
])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile libh2o.pc])
AC_OUTPUT
