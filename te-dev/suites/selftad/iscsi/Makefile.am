# SPDX-License-Identifier: Apache-2.0

tetestdir=$(prefix)/@PACKAGE_NAME@/$(subdir)

dist_tetest_DATA = package.xml

AM_CPPFLAGS = @TE_CPPFLAGS@ -I$(DESTDIR)/$(includedir)
AM_CFLAGS = @TE_CFLAGS@ @NET_SNMP_CFLAGS@
AM_LDFLAGS = @TE_LDFLAGS@ -L$(DESTDIR)/$(libdir)


tetest_PROGRAMS = simple double


LDADD = -ltapi_rpc -lrpc_types -ltapi_tad -lndn -lasn -ltapi -lrcfrpc \
		-lrcfapi -lrpcxdr -lconfapi -lconf_oid -llogger_ten -lipc \
		-ltools -llogger_core -lpthread @NET_SNMP_LIBS@
