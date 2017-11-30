ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

# Set up subdirectories to traverse
SUBDIRS = src

if ENABLE_GTK_DOC
    SUBDIRS += docs/reference/matrix-glib-sdk
endif

# Specify files to include in dist

# in share/doc/
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

# Lists of generated files you want to be able to clean

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

.PHONY: ChangeLog .dir-locals.el

.dir-locals.el:
	@echo -n ""; \
	includes=`echo -I$(top_srcdir) $(GLIB_CFLAGS) $(GOBJECT_CFLAGS) $(SOUP_CFLAGS) $(JSON_CFLAGS) $(VALA_CFLAGS) | tr " " "\n" | grep '^-I' | sed 's/^-I//'`; \
	flycheck_path='('; \
	company_path='('; \
	for incl in $$includes; \
	do \
	    if [ "$$incl" == "." ]; \
	    then \
	        incl="$(abs_top_srcdir)"; \
	    fi; \
	    flycheck_path="$$flycheck_path \"$$incl\""; \
	    company_path="$$company_path \"-I$$incl\""; \
	done; \
	flycheck_path="$$flycheck_path)"; \
	company_path="$$company_path)"; \
	echo "((c-mode . ((flycheck-clang-include-path . $$flycheck_path) (company-clang-arguments . $$company_path))))" > $@
