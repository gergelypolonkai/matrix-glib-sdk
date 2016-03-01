AM_CPPFLAGS = -DG_LOG_DOMAIN=\"Matrix-GLib\"

# Empty variables for elements to be added later
CLEANFILES =
BUILT_SOURCES =
EXTRA_DIST =

# Libraries to create
lib_LTLIBRARIES = libmatrix-glib-0.0.la

# Binaries to create
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
	$(INST_H_BUILT_FILES) \
	matrix-marshalers.c \
	matrix-version.c \
	matrix-types.c \
	matrix-api.c \
	matrix-http-api.c \
	matrix-enumtypes.c \
	utils.c \
	matrix-client.c \
	matrix-http-client.c \
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

BUILT_SOURCES += \
	matrix-enumtypes.c \
	matrix-enumtypes.h \
	matrix-marshalers.c \
	matrix-marshalers.h \
	$(NULL)

test_api_client_SOURCES = test-api-client.c
test_api_client_CFLAGS = $(libmatrix_glib_0_0_la_CFLAGS)
test_api_client_LDADD = \
	$(libmatrix_glib_0_0_la_LIBADD) \
	libmatrix-glib-$(MATRIX_GLIB_API_VERSION).la \
	$(NULL)

CLEANFILES += $(BUILT_SOURCES)
EXTRA_DIST += \
	matrix-enumtypes.h.template \
	matrix-enumtypes.c.template \
	matrix-marshalers.list \
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

matrix-marshalers.h: stamp-matrix-marshalers
	@true

stamp-matrix-marshalers: matrix-marshalers.list
	$(AM_V_GEN) $(GLIB_GENMARSHAL) --prefix=_matrix_marshal \
	                               $(srcdir)/matrix-marshalers.list \
	                               --header \
	                               --valist-marshallers >> xgen-gmlh \
	&& (cmp -s xgen-gmlh matrix-marshalers.h \
	    || cp xgen-gmlh matrix-marshalers.h) \
	&& rm -f xgen-gmlh \
	&& echo timestamp > $(@F)

matrix-marshalers.c: matrix-marshalers.h
	$(AM_V_GEN) (echo "#include \"matrix-marshalers.h\""; \
	             echo "#undef G_ENABLE_DEBUG"; \
	             $(GLIB_GENMARSHAL) --prefix=_matrix_marshal \
	                                $(srcdir)/matrix-marshalers.list \
	                                --body \
	                                --valist-marshallers) >> xgen-gmlc \
	&& cp xgen-gmlc matrix-marshalers.c \
	&& rm -f xgen-gmlc

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
	$(INST_H_SRC_FILES) \
	$(INST_H_BUILT_FILES) \
	$(NULL)

CLEANFILES += $(gir_DATA) $(typelibs_DATA)
