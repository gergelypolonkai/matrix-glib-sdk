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
	matrix-types.h \
	matrix-compacts.h \
	matrix-api.h \
	matrix-http-api.h \
	matrix-client.h \
	matrix-event-base.h \
	matrix-event-call-base.h \
	matrix-event-call-answer.h \
	matrix-event-call-hangup.h \
	matrix-event-call-invite.h \
	matrix-event-call-candidates.h \
	matrix-message-base.h \
	matrix-message-text.h \
	matrix-message-location.h \
	matrix-message-emote.h \
	matrix-message-file.h \
	matrix-message-image.h \
	matrix-message-audio.h \
	matrix-message-video.h \
	matrix-message-notice.h \
	matrix-event-room-base.h \
	matrix-event-state-base.h \
	matrix-event-tag.h \
	matrix-event-presence.h \
	matrix-event-room-member.h \
	matrix-event-room-topic.h \
	matrix-event-room-aliases.h \
	matrix-event-room-avatar.h \
	matrix-event-room-name.h \
	matrix-event-room-message.h \
	matrix-event-room-history-visibility.h \
	matrix-event-room-join-rules.h \
	matrix-event-room-canonical-alias.h \
	matrix-event-room-create.h \
	matrix-event-room-power-levels.h \
	matrix-event-room-guest-access.h \
	matrix-event-room-message-feedback.h \
	matrix-event-room-redaction.h \
	matrix-event-room-third-party-invite.h \
	matrix-event-typing.h \
	matrix-event-receipt.h \
	utils.h \
	matrix-profile.h \
	matrix-room.h \
	$(NULL)

INST_H_BUILT_FILES = \
	matrix-version.h \
	matrix-enumtypes.h \
	matrix-marshalers.h \
	$(NULL)

matrix_enum_headers = \
	matrix-types.h \
	$(NULL)

libmatrix_glib_0_0_la_SOURCES = \
	$(INST_H_BUILT_FILES) \
	matrix-marshalers.c \
	$(libmatrix_glib_0_0_la_VALA_SOURCES:.vala=.c) \
	matrix-event-types.c \
	matrix-version.c \
	matrix-api.c \
	matrix-http-api.c \
	matrix-client.c \
	matrix-types.c \
	matrix-compacts.c \
	matrix-event-base.c \
	matrix-event-call-base.c \
	matrix-event-call-answer.c \
	matrix-event-call-hangup.c \
	matrix-event-call-invite.c \
	matrix-event-call-candidates.c \
	matrix-message-base.c \
	matrix-message-text.c \
	matrix-message-location.c \
	matrix-message-emote.c \
	matrix-message-file.c \
	matrix-message-image.c \
	matrix-message-audio.c \
	matrix-message-video.c \
	matrix-message-notice.c \
	matrix-event-tag.c \
	matrix-event-presence.c \
	matrix-event-room-member.c \
	matrix-event-typing.c \
	matrix-event-room-base.c \
	matrix-event-state-base.c \
	matrix-event-receipt.c \
	matrix-event-room-topic.c \
	matrix-event-room-aliases.c \
	matrix-event-room-avatar.c \
	matrix-event-room-name.c \
	matrix-event-room-message.c \
	matrix-event-room-history-visibility.c \
	matrix-event-room-join-rules.c \
	matrix-event-room-canonical-alias.c \
	matrix-event-room-create.c \
	matrix-event-room-power-levels.c \
	matrix-event-room-message-feedback.c \
	matrix-event-room-redaction.c \
	matrix-event-room-guest-access.c \
	matrix-event-room-third-party-invite.c \
	matrix-profile.c \
	matrix-room.c \
	utils.c \
	matrix-enumtypes.c \
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

Matrix-0.0.vapi: libmatrix-glib-$(MATRIX_GLIB_API_VERSION).la
vapidir = $(datadir)/vala/vapi
dist_vapi_DATA = \
	Matrix-$(MATRIX_GLIB_API_VERSION).vapi \
	Matrix-$(MATRIX_GLIB_API_VERSION).deps \
	$(NULL)

BUILT_SOURCES += \
	matrix-enumtypes.c \
	matrix-enumtypes.h \
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

CLEANFILES += stamp-matrix-marshalers

limtatrix_glib_dlname = \
	`$(SED) -nE "s/^dlname='([A-Za-z0-9.+-]+)'/\1/p" libmatrix-glib-0.0.la`¬
INTROSPECTION_GIRS = Matrix-$(MATRIX_GLIB_API_VERSION).gir
INTROSPECTION_COMPILER_ARGS = --includedir=. $(libmatrix_glib_dlname)

Matrix-0.0.gir: libmatrix-glib-$(MATRIX_GLIB_API_VERSION).la
Matrix-0.0.typelib: $(INTROSPECTION_GIRS)
	$(INTROSPECTION_COMPILER) $(INTROSPECTION_COMPILER_ARGS) $< -o $@

girdir = $(INTROSPECTION_GIRDIR)
gir_DATA = $(INTROSPECTION_GIRS)
typelibdir = $(INTROSPECTION_TYPELIBDIR)
typelib_DATA = Matrix-$(MATRIX_GLIB_API_VERSION).typelib

CLEANFILES += $(gir_DATA) $(typelibs_DATA)


headerdir = $(includedir)/matrix-glib-$(MATRIX_GLIB_API_VERSION)
header_DATA = \
	$(PUBLIC_HEADER) \
	$(INST_H_SRC_FILES) \
	$(INST_H_BUILT_FILES) \
	$(NULL)

pkgconfig_in = matrix-glib-$(MATRIX_GLIB_API_VERSION).pc.in
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = $(pkgconfig_in:.in=)
EXTRA_DIST += $(pkgconfig_in)
