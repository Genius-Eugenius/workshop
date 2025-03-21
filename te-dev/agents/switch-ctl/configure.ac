# SPDX-License-Identifier: Apache-2.0
#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
#

AC_PREREQ([2.59])
AC_INIT([ta-switch-ctl],[0.1])
AC_CONFIG_SRCDIR([main.c])

AC_CONFIG_AUX_DIR([../../auxdir])
AC_CONFIG_MACRO_DIR([../../auxdir])

AM_INIT_AUTOMAKE([1.8.5 foreign -Wall])
TE_SETUP_AR

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

AC_CHECK_TOOL(NM, nm)

AC_CANONICAL_HOST

TE_APP_SET

AC_ARG_WITH([sniffers],
            AS_HELP_STRING([--with-sniffers=yes],
                           [enable sniffers (default is NO)]),
            [with_sniffers=sniffers_dummy], [with_sniffers=""])
AM_CONDITIONAL(CFG_SNIFFERS_DUMMY, test -n "$with_sniffers")

if test -z "$with_tad" ; then
    with_tad=tad
fi
AC_SUBST([TAD], [$with_tad])

AC_SUBST([TAD_LIBS])

AC_CONFIG_HEADERS([config.h])

AC_HEADER_STDC
AC_CHECK_HEADERS([\
time.h \
sys/time.h \
netinet/in.h \
assert.h \
semaphore.h \
])
AC_C_INLINE

export PKG_CONFIG_PATH=$DESTDIR/$prefix/../$TE_PLATFORM/lib/pkgconfig/

PKG_CHECK_MODULES(TAD, te-$with_tad)

AC_CONFIG_FILES([Makefile])

TE_APP_RESTORE

AC_OUTPUT
