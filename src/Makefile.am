AM_CPPFLAGS = -DG_LOG_DOMAIN=\"Matrix-GLib\"

lib_LTLIBRARIES = libmatrix-glib-0.0.la
bin_PROGRAMS = test-api-client

INST_H_SRC_FILES = \
	matrix-types.h \
	matrix-api.h \
	matrix-http-api.h \
	matrix-client.h \
	matrix-http-client.h \
	$(NULL)

INST_H_BUILT_FILES = \
	matrix-version.h \
	matrix-enumtypes.h \
	$(NULL)

matrix_enum_headers = \
	matrix-types.h \
	matrix-api.h \
	$(NULL)

libmatrix_glib_0_0_la_SOURCES = \
	matrix-version.c \
	matrix-types.c \
	matrix-api.c \
	matrix-http-api.c \
	matrix-enumtypes.c \
	utils.c \
	matrix-marshalers.c \
	matrix-client.c \
	matrix-http-client.c \
	$(INST_H_SRC_FILES) \
	$(INST_H_BUILT_FILES) \
	$(NULL)

libmatrix_glib_0_0_la_CFLAGS = $(GLIB_CFLAGS) $(GOBJECT_CFLAGS) $(SOUP_CFLAGS) $(JSON_CFLAGS)
libmatrix_glib_0_0_la_LIBADD = $(GLIB_LIBS) $(GOBJECT_LIBS) $(SOUP_LIBS) $(JSON_LIBS)
libmatrix_glib_0_0_la_DEPENDENCIES =

BUILT_SOURCES = \
	matrix-enumtypes.c \
	matrix-enumtypes.h \
	matrix-marshalers.c \
	matrix-marshalers.h \
	$(NULL)

test_api_client_SOURCES = test-api-client.c
test_api_client_CFLAGS = $(libmatrix_glib_0_0_la_CFLAGS)
test_api_client_LDADD = $(libmatrix_glib_0_0_la_LIBADD) libmatrix-glib-0.0.la

CLEANFILES = $(BUILT_SOURCES)
EXTRA_DIST = \
	matrix-enumtypes.h.template \
	matrix-enumtypes.c.template \
	matrix-marshalers.list \
	$(INST_H_SRC_FILES) \
	$(NULL)

matrix-enumtypes.h: $(matrix_enum_headers) matrix-enumtypes.h.template
	$(AM_V_GEN) $(GLIB_MKENUMS) --template $(filter %.template,$^) $(filter-out %.template,$^) > $@.tmp \
	&& mv $@.tmp $@

matrix-enumtypes.c: $(matrix_enum_headers) matrix-enumtypes.h matrix-enumtypes.c.template
	$(AM_V_GEN) $(GLIB_MKENUMS) --template $(filter %.template,$^) $(filter-out %.template,$^) > $@.tmp \
	&& mv $@.tmp $@

matrix-marshalers.h: matrix-marshalers.list
	$(AM_V_GEN) $(GLIB_GENMARSHAL) --prefix=_matrix_marshal $(srcdir)/matrix-marshalers.list --header --valist-marshallers >> xgen-gmlh \
	&& (cmp -s xgen-gmlh matrix-marshalers.h || cp xgen-gmlh matrix-marshalers.h) \
	&& rm -f xgen-gmlh

matrix-marshalers.c: matrix-marshalers.list
	$(AM_V_GEN) (echo "#include \"matrix-marshalers.h\""; \
	$(GLIB_GENMARSHAL) --prefix=_matrix_marshal $(srcdir)/matrix-marshalers.list --body --valist-marshallers) >> xgen-gmlc \
	&& cp xgen-gmlc matrix-marshalers.c \
	&& rm -f xgen-gmlc

include $(INTROSPECTION_MAKEFILE)
Matrix-$(MATRIX_GLIB_API_VERSION).gir: libmatrix-glib-0.0.la
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
