# SPDX-License-Identifier: Apache-2.0
#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
#

AC_PREREQ([2.59])

AC_INIT([millicom],[0.02])

AC_CONFIG_AUX_DIR([../../auxdir])
AM_INIT_AUTOMAKE([1.8.5 foreign -Wall])
TE_SETUP_AR

AC_CONFIG_SRCDIR([millicom.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-ldl':
#AC_CHECK_LIB([dl], [main])
# FIXME: Replace `main' with a function in `-lm':
#AC_CHECK_LIB([m], [main])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h termio.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_TIME

# Checks for library functions.
AC_FUNC_SETVBUF_REVERSED
AC_DIAGNOSE([obsolete],[your code may safely assume C89 semantics that
RETSIGTYPE is void. Remove this warning and the `AC_CACHE_CHECK' when you
adjust the code.])dnl
AC_CACHE_CHECK([return type of signal handlers],[ac_cv_type_signal],
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([#include <sys/types.h>
#include <signal.h>
],
  [return *(signal (0, 0)) (0) == 1;])],
  [ac_cv_type_signal=int],
  [ac_cv_type_signal=void])])
AC_DEFINE_UNQUOTED([RETSIGTYPE],[$ac_cv_type_signal],
                   [Define as the return type of signal handlers
                    (`int' or `void').])


AC_CONFIG_FILES([Makefile])

AC_OUTPUT


CFLAGS=${CFLAGS}" "${TE_CFLAGS}
CPPFLAGS=${CPPFLAGS}" "${TE_CFLAGS}

if test "x$TE_BASE" != "x"
then
    CPPFLAGS=${CPPFLAGS}" "-I${TE_BASE}/include
fi

AC_HEADER_STDC

AC_OUTPUT
