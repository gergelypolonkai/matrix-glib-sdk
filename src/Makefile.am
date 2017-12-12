AM_CPPFLAGS = -DG_LOG_DOMAIN=\"Matrix-GLib\" -D__MATRIX_GLIB_SDK_COMPILATION

# Empty variables for elements to be added later
CLEANFILES =
BUILT_SOURCES =
EXTRA_DIST =

# Libraries to create
lib_LTLIBRARIES = libmatrix-glib-0.0.la

AM_CPPFLAGS += \
	-include $(CONFIG_HEADER) \
	-I$(top_srcdir) \
	-g \
	-Wall \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	$(SOUP_CFLAGS) \
	$(JSON_CFLAGS) \
	$(NULL)

# Binaries to create
bin_PROGRAMS = test-api-client test-client

EVENT_H_FILES = \
	matrix-event-base.h \
	matrix-event-call-base.h \
	matrix-event-call-answer.h \
	matrix-event-call-hangup.h \
	matrix-event-call-invite.h \
	matrix-event-call-candidates.h \
	$(NULL)

MESSAGE_H_FILES = \
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
	$(NULL)

INST_H_SRC_FILES = \
	matrix-types.h \
	matrix-compacts.h \
	matrix-api.h \
	matrix-http-api.h \
	matrix-client.h \
	matrix-http-client.h \
	$(EVENT_H_FILES) \
	$(MESSAGE_H_FILES) \
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
	matrix-event-types.c \
	matrix-version.c \
	matrix-api.c \
	matrix-http-api.c \
	matrix-client.c \
	matrix-http-client.c \
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

libmatrix_glib_dlname = \
	`$(SED) -nE "s/^dlname='([A-Za-z0-9.+-]+)'/\1/p" libmatrix-glib-0.0.la`¬

if HAVE_INTROSPECTION

include $(INTROSPECTION_MAKEFILE)

INTROSPECTION_GIRS = \
	Matrix-$(MATRIX_GLIB_API_VERSION).gir \
	MatrixEvent-$(MATRIX_GLIB_API_VERSION).gir \
	MatrixMessage-$(MATRIX_GLIB_API_VERSION).gir \
	$(NULL)

Matrix-$(MATRIX_GLIB_API_VERSION).gir: libmatrix-glib-0.0.la
Matrix_0_0_gir_FILES = $(INST_H_SRC_FILE) $(INST_H_BUILT_FILES) $(filter %.c,$(libmatrix_glib_0_0_la_SOURCES))
Matrix_0_0_gir_LIBS = libmatrix-glib-0.0.la
Matrix_0_0_gir_SCANNERFLAGS = --identifier-prefix=Matrix --symbol-prefix=matrix --warn-all
Matrix_0_0_gir_INCLUDES = GLib-2.0 GObject-2.0
Matrix_0_0_gir_CFLAGS = -D_MATRIX_GLIB_SDK_COMPILATION -I$(top_srcdir) -I$(srcdir) -I$(builddir) $(AM_CPPFLAGS)
Matrix_0_0_gir_EXPORT_PACKAGES = matrix-glib
MatrixEvent-$(MATRIX_GLIB_API_VERSION).gir: libmatrix-glib-0.0.la
MatrixEvent_0_0_gir_FILES = $(EVENT_H_FILES) $(libmatrix_glib_0_0_la_SOURCES)
MatrixEvent_0_0_gir_LIBS = libmatrix-glib-0.0.la
MatrixEvent_0_0_gir_SCANNERFLAGS = --identifier-prefix=MatrixEvent --symbol-prefix=matrix_event --warn-all
MatrixEvent_0_0_gir_INCLUDES = GLib-2.0 GObject-2.0
MatrixEvent_0_0_gir_CFLAGS = -D_MATRIX_GLIB_SDK_COMPILATION -I$(top_srcdir) -I$(srcdir) -I$(builddir) $(AM_CPPFLAGS)
MatrixEvent_0_0_gir_EXPORT_PACKAGES = matrix-event-glib
MatrixMessage-$(MATRIX_GLIB_API_VERSION).gir: libmatrix-glib-0.0.la
MatrixMessage_0_0_gir_FILES = $(filter matrix-message-%.h,$(Matrix_0_0_gir_FILES))
MatrixMessage_0_0_gir_LIBS = libmatrix-glib-0.0.la
MatrixMessage_0_0_gir_SCANNERFLAGS = --identifier-prefix=MatrixMessage --symbol-prefix=matrix_message --warn-all
MatrixMessage_0_0_gir_INCLUDES = GLib-2.0 GObject-2.0
MatrixMessage_0_0_gir_CFLAGS = -D_MATRIX_GLIB_SDK_COMPILATION -I$(top_srcdir) -I$(srcdir) -I$(builddir) $(AM_CPPFLAGS)
MatrixMessage_0_0_gir_EXPORT_PACKAGES = matrix-message-glib

girdir = $(INTROSPECTION_GIRDIR)
gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(INTROSPECTION_TYPELIBDIR)
typelib_DATA = Matrix-$(MATRIX_GLIB_API_VERSION).typelib

headerdir = $(includedir)/matrix-glib-$(MATRIX_GLIB_API_VERSION)
header_DATA = \
	$(INST_H_SRC_FILES) \
	$(INST_H_BUILT_FILES) \
	$(NULL)

CLEANFILES += $(gir_DATA) $(typelibs_DATA)

if HAVE_VAPIGEN

include $(VAPIGEN_MAKEFILE)

VAPIGEN_VAPIS = Matrix-$(MATRIX_GLIB_API_VERSION).vapi

Matrix_0_0_vapi_DEPS = \
	json-glib-1.0 \
	gio-2.0 \
	libsoup-2.4 \
	$(NULL)
Matrix_0_0_vapi_GIRS = $(INTROSPECTION_GIRS)
Matrix_0_0_vapi_FILES = $(INTROSPECTION_GIRS)

vapidir = $(datadir)/vala/vapi
vapi_DATA = $(VAPIGEN_VAPIS)

endif

endif

pkgconfig_in = matrix-glib-$(MATRIX_GLIB_API_VERSION).pc.in
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = $(pkgconfig_in:.in=)
EXTRA_DIST += $(pkgconfig_in)
