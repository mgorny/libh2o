ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(top_srcdir)/src

# ensure that assertions will be thrown in distcheck
DISTCHECK_CONFIGURE_FLAGS = --enable-debug

h2odir = $(includedir)/h2o
pkgconfigdir = $(libdir)/pkgconfig

lib_LTLIBRARIES = libh2o.la

libh2o_la_SOURCES = src/boundaries/boundaries.c \
	src/boundaries/b23.c src/boundaries/b13hs.c \
	src/boundaries/b14hs.c src/boundaries/b23hs.c \
	src/boundaries/b3a4hs.c src/boundaries/b2ab4hs.c \
	src/boundaries/b2c3b4hs.c \
	\
	src/region1/region1.c src/region1/region1ph.c \
	src/region1/region1ps.c src/region1/region1hs.c \
	\
	src/region2/region2.c src/region2/region2meta.c \
	src/region2/region2sub.c src/region2/region2ph.c \
	src/region2/region2hs.c \
	src/region2/region2pha.c src/region2/region2phb.c \
	src/region2/region2phc.c \
	src/region2/region2psa.c src/region2/region2psb.c \
	src/region2/region2psc.c \
	src/region2/region2hsa.c src/region2/region2hsb.c \
	src/region2/region2hsc.c \
	\
	src/region3/region3.c src/region3/region3sub.c \
	src/region3/region3ph.c \
	src/region3/region3Tpha.c src/region3/region3Tphb.c \
	src/region3/region3vpha.c src/region3/region3vphb.c \
	src/region3/region3Tpsa.c src/region3/region3Tpsb.c \
	src/region3/region3vpsa.c src/region3/region3vpsb.c \
	src/region3/region3psath.c src/region3/region3psats.c \
	src/region3/region3hsa.c src/region3/region3hsb.c \
	src/region3/pT/region3pTa.c src/region3/pT/region3pTb.c \
	src/region3/pT/region3pTc.c src/region3/pT/region3pTd.c \
	src/region3/pT/region3pTe.c src/region3/pT/region3pTf.c \
	src/region3/pT/region3pTg.c src/region3/pT/region3pTh.c \
	src/region3/pT/region3pTi.c src/region3/pT/region3pTj.c \
	src/region3/pT/region3pTk.c src/region3/pT/region3pTl.c \
	src/region3/pT/region3pTm.c src/region3/pT/region3pTn.c \
	src/region3/pT/region3pTo.c src/region3/pT/region3pTp.c \
	src/region3/pT/region3pTq.c src/region3/pT/region3pTr.c \
	src/region3/pT/region3pTs.c src/region3/pT/region3pTt.c \
	src/region3/pT/region3pTu.c src/region3/pT/region3pTv.c \
	src/region3/pT/region3pTw.c src/region3/pT/region3pTx.c \
	src/region3/pT/region3pTy.c src/region3/pT/region3pTz.c \
	src/region3/pT-boundaries/region3ab.c \
	src/region3/pT-boundaries/region3cd.c \
	src/region3/pT-boundaries/region3ef.c \
	src/region3/pT-boundaries/region3gh.c \
	src/region3/pT-boundaries/region3ij.c \
	src/region3/pT-boundaries/region3jk.c \
	src/region3/pT-boundaries/region3mn.c \
	src/region3/pT-boundaries/region3op.c \
	src/region3/pT-boundaries/region3qu.c \
	src/region3/pT-boundaries/region3rx.c \
	src/region3/pT-boundaries/region3uv.c \
	src/region3/pT-boundaries/region3wx.c \
	src/region3/region3pT.c \
	\
	src/region4/region4.c src/region4/region4hs.c \
	\
	src/region5/region5.c \
	\
	src/h2o.c \
	\
	src/xmath.c src/consts.h src/xmath.h
libh2o_la_LIBADD = -lm
libh2o_la_LDFLAGS = -version-info 1:0:1 -no-undefined

h2o_HEADERS = src/boundaries.h src/h2o.h \
	src/region1.h src/region2.h src/region3.h src/region4.h src/region5.h

pkgconfig_DATA = libh2o.pc

TESTS = tests/if97-test-values tests/h2o-t-tests
check_PROGRAMS = $(TESTS)

tests_if97_test_values_SOURCES = tests/if97-test-values.c
tests_if97_test_values_CPPFLAGS = -I$(top_srcdir)/src
tests_if97_test_values_LDADD = libh2o.la

tests_h2o_t_tests_SOURCES = tests/h2o-t-tests.c
tests_h2o_t_tests_CPPFLAGS = -I$(top_srcdir)/src
tests_h2o_t_tests_LDADD = libh2o.la

EXTRA_DIST = NEWS
NEWS: configure.ac Makefile.am
	git for-each-ref refs/tags --sort '-*committerdate' \
		--format '# %(tag) (%(*committerdate:short))%0a%(contents:body)' \
		> NEWS
