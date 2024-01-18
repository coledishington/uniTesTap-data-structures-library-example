AC_INIT([datastructures],[0.0.1],[coledishington@gmail.com])
AC_CONFIG_SRCDIR([src/dynamic_array.c])
AC_CONFIG_AUX_DIR([build/autotools/aux])
AC_CONFIG_MACRO_DIR([build/autotools/m4])
AC_CONFIG_MACRO_DIRS([build/autotools/m4])

# TAP driver
AC_REQUIRE_AUX_FILE([tap-driver.sh])

AM_PROG_AR
AC_PROG_AWK
AC_PROG_CC

AC_DEFINE([_POSIX_C_SOURCE], [200809L], [Support newer posix definitions with glibc])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])
LT_INIT

AC_CONFIG_FILES([
    Makefile
    src/Makefile
])
AC_CONFIG_SUBDIRS([uniTesTap])
AC_OUTPUT
