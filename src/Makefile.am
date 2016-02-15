include $(top_srcdir)/vala-globals.mk

AM_CPPFLAGS = -DG_LOG_DOMAIN=\"Matrix-GLib\"

# Empty variables for elements to be added later
CLEANFILES =
BUILT_SOURCES =
EXTRA_DIST =
AM_VALAFLAGS =

# Name of the public header file
PUBLIC_HEADER = matrix-glib.h

# Libraries to create
lib_LTLIBRARIES = libmatrix-glib-0.0.la

# Vala source files
libmatrix_glib_0_0_la_VALA_SOURCES = \
	matrix-api.vala \
	matrix-client.vala \
	matrix-enums.vala \
	matrix-http-api.vala \
	matrix-http-client.vala \
	$(NULL)

AM_CPPFLAGS += \
	-include $(CONFIG_HEADER) \
	-I$(top_srcdir) \
	-g \
	-Wall \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	$(SOUP_CFLAGS) \
	$(JSON_CFLAGS) \
	$(VALA_CFLAGS) \
	$(NULL)

AM_VALAFLAGS += \
	--library=Matrix-$(MATRIX_GLIB_API_VERSION) \
	$(VALA_PKG_LIST) \
	--vapi=Matrix-$(MATRIX_GLIB_API_VERSION).vapi \
	-H $(PUBLIC_HEADER) \
	-C \
	--use-header \
	--gir=Matrix-$(MATRIX_GLIB_API_VERSION).gir \
	$(top_srcdir)/src/c-api.vapi \
	$(NULL)

vala-stamp: $(libmatrix_glib_0_0_la_VALA_SOURCES)
	@rm -f vala-temp
	@touch vala-temp
	$(AM_V_GEN) $(VALAC) $(AM_VALAFLAGS) $^
	@mv -f vala-temp $@

CLEANFILES += \
	$(PUBLIC_HEADER) \
	$(libmatrix_glib_0_0_la_VALA_SOURCES:.vala=.c) \
	$(NULL)

$(libmatrix_glib_0_0_la_VALA_SOURCES:.vala=.c): vala-stamp
	@if test -f $@; then :; else \
	    trap 'rm -rf vala-lock vala-stamp' 1 2 13 15; \
	    if mkdir vala-lock 2> /dev/null; then \
	        rm -f vala-stamp; \
	        $(MAKE) $(AM_MAKEFLAGS) vala-stamp; \
	        rmdir vala-lock; \
	    else \
	        while test -d vala-lock; do sleep 1; done; \
	        test -f vala-stamp; exit $$?; \
	    fi \
	fi

# Binaries to create
bin_PROGRAMS = test-api-client

INST_H_SRC_FILES = \
	matrix-types.h \
	$(NULL)

INST_H_BUILT_FILES = \
	matrix-version.h \
	matrix-enumtypes.h \
	$(NULL)

matrix_enum_headers = \
	matrix-types.h \
	$(NULL)

libmatrix_glib_0_0_la_SOURCES = \
	$(INST_H_BUILT_FILES) \
	$(libmatrix_glib_0_0_la_VALA_SOURCES:.vala=.c) \
	matrix-version.c \
	matrix-types.c \
	matrix-enumtypes.c \
	utils.c \
	$(INST_H_SRC_FILES) \
	$(NULL)

libmatrix_glib_0_0_la_CFLAGS = \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	$(SOUP_CFLAGS) \
	$(JSON_CFLAGS) \
	$(NULL)

libmatrix_glib_0_0_la_LIBADD = \
	$(GLIB_LIBS) \
	$(GOBJECT_LIBS) \
	$(SOUP_LIBS) \
	$(JSON_LIBS) \
	$(NULL)

libmatrix_glib_0_0_la_LDFLAGS = \
	-version-info "$(LT_CURRENT)":"$(LT_REVISION)":"$(LT_AGE)"

$(PUBLIC_HEADER): vala-stamp

BUILT_SOURCES += \
	matrix-enumtypes.c \
	matrix-enumtypes.h \
	$(NULL)

test_api_client_SOURCES = test-api-client.c
test_api_client_CFLAGS = $(libmatrix_glib_0_0_la_CFLAGS) $(AM_CFLAGS)
test_api_client_LDADD = \
	$(libmatrix_glib_0_0_la_LIBADD) \
	libmatrix-glib-$(MATRIX_GLIB_API_VERSION).la \
	$(NULL)

CLEANFILES += $(BUILT_SOURCES)
EXTRA_DIST += \
	matrix-enumtypes.h.template \
	matrix-enumtypes.c.template \
	$(INST_H_SRC_FILES) \
	$(NULL)

matrix-enumtypes.h: $(matrix_enum_headers) matrix-enumtypes.h.template
	$(AM_V_GEN) $(GLIB_MKENUMS) --template $(filter %.template,$^) \
	                            $(filter-out %.template,$^) > $@.tmp \
	&& mv $@.tmp $@

matrix-enumtypes.c: $(matrix_enum_headers) matrix-enumtypes.h matrix-enumtypes.c.template
	$(AM_V_GEN) $(GLIB_MKENUMS) --template $(filter %.template,$^) \
	                            $(filter-out %.template,$^) > $@.tmp \
	&& mv $@.tmp $@

include $(INTROSPECTION_MAKEFILE)
Matrix-0.0.gir: libmatrix-glib-$(MATRIX_GLIB_API_VERSION).la

Matrix_0_0_gir_FILES = \
	$(INST_H_SRC_FILES) \
	$(INST_H_BUILT_FILES) \
	$(filter %.c,$(libmatrix_glib_0_0_la_SOURCES)) \
	$(NULL)

Matrix_0_0_gir_LIBS = libmatrix-glib-$(MATRIX_GLIB_API_VERSION).la

Matrix_0_0_gir_SCANNERFLAGS = \
	--identifier-prefix=Matrix \
	--symbol-prefix=matrix \
	--warn-all \
	$(NULL)

Matrix_0_0_gir_INCLUDES = GLib-2.0 GObject-2.0 Soup-2.4 Json-1.0

Matrix_0_0_gir_CFLAGS = \
	-D__MATRIX_GLIB_BUILDING__ \
	-I$(top_srcdir) \
	-I$(srcdir) \
	-I$(builddir) \
	$(NULL)

Matrix_0_0_gir_EXPORT_PACKAGES = matrix-glib
INTROSPECTION_GIRS = Matrix-$(MATRIX_GLIB_API_VERSION).gir

girdir = $(datadir)/gir-1.0
gir_DATA = Matrix-$(MATRIX_GLIB_API_VERSION).gir

typelibsdir = $(libdir)/girepository-1.0
typelibs_DATA = Matrix-$(MATRIX_GLIB_API_VERSION).typelib

headerdir = $(includedir)/matrix-glib-$(MATRIX_GLIB_API_VERSION)
header_DATA = \
	$(PUBLIC_HEADER) \
	$(INST_H_SRC_FILES) \
	$(INST_H_BUILT_FILES) \
	$(NULL)

CLEANFILES += $(gir_DATA) $(typelibs_DATA)
