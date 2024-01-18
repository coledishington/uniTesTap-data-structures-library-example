INCLUDE_PATH = @abs_top_srcdir@/include
TAP_INCLUDE_PATH = @abs_top_srcdir@/uniTesTap/include/public

AM_CFLAGS = -Wall -Werror
AM_CPPFLAGS = -I $(INCLUDE_PATH) -I $(TAP_INCLUDE_PATH)

include_HEADERS = $(INCLUDE_PATH)/data_structures.h

lib_LTLIBRARIES = libdata_structures.la
libdata_structures_la_SOURCES = dynamic_array.c

check_PROGRAMS = dynamic_array.test

dynamic_array_test_SOURCES = test_dynamic_array.c
dynamic_array_test_LDADD = \
    @abs_top_builddir@/uniTesTap/tapcore/libuniTesTap.la \
    libdata_structures.la

TEST_LOG_DRIVER = \
    env AM_TAP_AWK='@AWK@' @SHELL@ \
    @abs_top_srcdir@/build/autotools/aux/tap-driver.sh

TESTS = $(check_PROGRAMS)
EXTRA_DIST = $(check_PROGRAMS)
