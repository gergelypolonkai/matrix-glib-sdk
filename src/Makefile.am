AM_CPPFLAGS = -DG_LOG_DOMAIN=\"Matrix-GLib\"

lib_LTLIBRARIES = libmatrix-glib-0.0.la
bin_PROGRAMS = test-client

INST_H_SRC_FILES = \
	matrix-client.h \
	matrix-api.h \
	matrix-http-api.h \
	$(NULL)

INST_H_BUILT_FILES = matrix-version.h

libmatrix_glib_0_0_la_SOURCES = \
	matrix-client.c \
	matrix-version.c \
	matrix-api.c \
	matrix-http-api.c \
	$(INST_H_SRC_FILES) \
	$(INST_H_BUILT_FILES) \
	$(NULL)

libmatrix_glib_0_0_la_CFLAGS = $(GLIB_CFLAGS) $(GOBJECT_CFLAGS) $(SOUP_CFLAGS) $(JSON_CFLAGS)
libmatrix_glib_0_0_la_LIBADD = $(GLIB_LIBS) $(GOBJECT_LIBS) $(SOUP_LIBS) $(JSON_LIBS)
libmatrix_glib_0_0_la_DEPENDENCIES =

test_client_SOURCES = test-client.c $(libmatrix_glib_0_0_la_SOURCES)
test_client_CFLAGS = $(libmatrix_glib_0_0_la_CFLAGS)
test_client_LDADD = $(libmatrix_glib_0_0_la_LIBADD)

CLEANFILES =
EXTRA_DIST = $(INST_H_SRC_FILES)

include $(INTROSPECTION_MAKEFILE)
MatrixGlib-$(MATRIX_GLIB_API_VERSION).gir: libmatrix-glib-0.0.la
Matrix_@MATRIX_GLIB_API_VERSION_U@_gir_FILES = $(INST_H_SRC_FILES) $(INST_H_BUILT_FILES) $(filter %.c,$(libmatrix_glib_0_0_la_SOURCES))
Matrix_@MATRIX_GLIB_API_VERSION_U@_gir_LIBS = libmatrix-glib-0.0.la
Matrix_@MATRIX_GLIB_API_VERSION_U@_gir_SCANNERFLAGS = --identifier-prefix=Matrix --symbol-prefix=matrix --warn-all
Matrix_@MATRIX_GLIB_API_VERSION_U@_gir_INCLUDES = GLib-2.0 GObject-2.0 Soup-2.4 Json-1.0
Matrix_@MATRIX_GLIB_API_VERSION_U@_gir_CFLAGS = -D__MATRIX_GLIB_BUILDING__ -I$(top_srcdir) -I$(srcdir) -I$(builddir)
Matrix_@MATRIX_GLIB_API_VERSION_U@_gir_EXPORT_PACKAGES = matrix-glib
INTROSPECTION_GIRS = Matrix-$(MATRIX_GLIB_API_VERSION).gir

girdir = $(datadir)/gir-1.0
gir_DATA = Matrix-$(MATRIX_GLIB_API_VERSION).gir

typelibsdir = $(libdir)/girepository-1.0
typelibs_DATA = Matrix-$(MATRIX_GLIB_API_VERSION).typelib

headerdir = $(includedir)/swe-glib
header_DATA = \
	$(INST_H_SRC_FILES) \
	$(INST_H_BUILT_FILES) \
	$(NULL)

CLEANFILES += $(gir_DATA) $(typelibs_DATA)
