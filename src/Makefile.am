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
	namespace-info.vala \
	matrix-api.vala \
	matrix-client.vala \
	matrix-types.vala \
	matrix-http-api.vala \
	matrix-http-client.vala \
	matrix-compacts.vala \
	matrix-event-base.vala \
	matrix-event-room-base.vala \
	matrix-event-state-base.vala \
	matrix-event-presence.vala \
	matrix-event-room-member.vala \
	matrix-event-room-message.vala \
	matrix-event-room-topic.vala \
	matrix-event-typing.vala \
	matrix-event-room-aliases.vala \
	matrix-event-receipt.vala \
	matrix-event-room-history-visibility.vala \
	matrix-event-room-join-rules.vala \
	matrix-event-room-name.vala \
	matrix-event-tag.vala \
	matrix-event-room-canonical-alias.vala \
	matrix-event-room-create.vala \
	matrix-event-room-power-levels.vala \
	matrix-event-room-avatar.vala \
	matrix-event-room-message-feedback.vala \
	matrix-event-room-guest-access.vala \
	matrix-event-room-redaction.vala \
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
	$(GEE_CFLAGS) \
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
	--vapidir=$(top_srcdir)/vapi \
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
bin_PROGRAMS = test-api-client test-client

INST_H_SRC_FILES = \
	$(NULL)

INST_H_BUILT_FILES = \
	matrix-version.h \
	matrix-marshalers.h \
	$(NULL)

libmatrix_glib_0_0_la_SOURCES = \
	$(INST_H_BUILT_FILES) \
	matrix-marshalers.c \
	$(libmatrix_glib_0_0_la_VALA_SOURCES:.vala=.c) \
	matrix-event-types.c \
	matrix-version.c \
	$(INST_H_SRC_FILES) \
	$(NULL)

libmatrix_glib_0_0_la_CFLAGS = \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	$(SOUP_CFLAGS) \
	$(JSON_CFLAGS) \
	$(GEE_CFLAGS) \
	$(NULL)

libmatrix_glib_0_0_la_LIBADD = \
	$(GLIB_LIBS) \
	$(GOBJECT_LIBS) \
	$(SOUP_LIBS) \
	$(JSON_LIBS) \
	$(GEE_LIBS) \
	$(NULL)

libmatrix_glib_0_0_la_LDFLAGS = \
	-version-info "$(LT_CURRENT)":"$(LT_REVISION)":"$(LT_AGE)"

$(PUBLIC_HEADER): vala-stamp

Matrix-0.0.vapi: libmatrix-glib-$(MATRIX_GLIB_API_VERSION).la
vapidir = $(datadir)/vala/vapi
dist_vapi_DATA = \
	Matrix-$(MATRIX_GLIB_API_VERSION).vapi \
	Matrix-$(MATRIX_GLIB_API_VERSION).deps \
	$(NULL)

BUILT_SOURCES += \
	matrix-marshalers.c \
	matrix-marshalers.h \
	$(NULL)

test_api_client_SOURCES = test-api-client.c
test_api_client_CFLAGS = $(libmatrix_glib_0_0_la_CFLAGS) $(AM_CFLAGS)
test_api_client_LDADD = \
	$(libmatrix_glib_0_0_la_LIBADD) \
	libmatrix-glib-$(MATRIX_GLIB_API_VERSION).la \
	$(NULL)

test_client_SOURCES = test-client.c
test_client_CFLAGS = $(libmatrix_glib_0_0_la_CFLAGS) $(AM_CFLAGS)
test_client_LDADD = \
	$(libmatrix_glib_0_0_la_LIBADD) \
	libmatrix-glib-$(MATRIX_GLIB_API_VERSION).la \
	$(NULL)

CLEANFILES += $(BUILT_SOURCES)
EXTRA_DIST += \
	matrix-marshalers.list \
	$(INST_H_SRC_FILES) \
	$(NULL)

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

CLEANFILES += stamp-matrix-marshalers

include $(INTROSPECTION_MAKEFILE)
Matrix-0.0.gir: libmatrix-glib-$(MATRIX_GLIB_API_VERSION).la

typelibsdir = $(libdir)/girepository-1.0
typelibs_DATA = Matrix-$(MATRIX_GLIB_API_VERSION).typelib

headerdir = $(includedir)/matrix-glib-$(MATRIX_GLIB_API_VERSION)
header_DATA = \
	$(PUBLIC_HEADER) \
	$(INST_H_SRC_FILES) \
	$(INST_H_BUILT_FILES) \
	$(NULL)

CLEANFILES += $(typelibs_DATA)
