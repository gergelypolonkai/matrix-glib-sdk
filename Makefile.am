ACLOCAL_AMFLAGS = -I m4
SUBDIRS = src

if ENABLE_GTK_DOC
SUBDIRS += docs/reference/matrix-glib
endif

matrixglibdocdir = ${prefix}/doc/matrix-glib
matrixglibdoc_DATA = \
	README \
	COPYING \
	AUTHORS \
	ChangeLog \
	INSTALL \
	NEWS

# with make dist

EXTRA_DIST = $(matrixglibdoc_DATA) \
	autogen.sh \
	config.rpath

MAINTAINERCLEANFILES = \
	$(srcdir)/aclocal.m4 \
	$(srcdir)/compile \
	$(srcdir)/config.guess \
	$(srcdir)/config.h.in \
	$(srcdir)/config.rpath \
	$(srcdir)/config.sub \
	$(srcdir)/depcomp \
	$(srcdir)/install-sh \
	$(srcdir)/ltmain.sh \
	$(srcdir)/missing \
	$(srcdir)/mkinstalldirs \
	$(srcdir)/ChangeLog \
	`find "$(srcdir)" -type f -name Makefile.in -print` \
	$(NULL)

DISTCLEANFILES = \
	ChangeLog

# Automatically generate ChangeLog from git

ChangeLog:
	@echo Creating $@
	@touch ChangeLog
	@if test -d "$(srcdir)/.git"; then \
	  (GIT_DIR=$(top_srcdir)/.git ./missing --run git log --stat) | fmt > $@.tmp \
	  && mv -f $@.tmp $@ \
	  || ($(RM) $@.tmp; \
	      echo Failed to generate ChangeLog, your ChangeLog may be outdated >&2; \
	      (test -f $@ || echo git-log is required to generate this file >> $@)); \
	else \
	  test -f $@ || \
	  (echo A git checkout and git-log is required to generate ChangeLog >&2 && \
	  echo A git checkout and git-log is required to generate this file >> $@); \
	fi

.PHONY: ChangeLog
