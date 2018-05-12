add_global_arguments('-DG_LOG_DOMAIN="Matrix-GLib"', language : 'c')
add_global_arguments('-D__MATRIX_GLIB_SDK_COMPILATION', language : 'c')
add_global_arguments('-Wall', language : 'c')

enums = gnome.mkenums_simple('matrix-enumtypes',
                             install_header : true,
                             sources : 'matrix-types.h',
                             install_dir : 'matrix-glib-sdk')

marshalers = gnome.genmarshal('matrix-marshalers',
                              prefix : '_matrix_marshal',
                              sources : 'matrix-marshalers.list',
                              valist_marshallers : true)

event_h_files = [
    'matrix-event-base.h',
    'matrix-event-call-base.h',
    'matrix-event-call-answer.h',
    'matrix-event-call-hangup.h',
    'matrix-event-call-invite.h',
    'matrix-event-call-candidates.h',
    'matrix-event-room-base.h',
    'matrix-event-state-base.h',
    'matrix-event-tag.h',
    'matrix-event-presence.h',
    'matrix-event-room-member.h',
    'matrix-event-room-topic.h',
    'matrix-event-room-aliases.h',
    'matrix-event-room-avatar.h',
    'matrix-event-room-name.h',
    'matrix-event-room-message.h',
    'matrix-event-room-history-visibility.h',
    'matrix-event-room-join-rules.h',
    'matrix-event-room-canonical-alias.h',
    'matrix-event-room-create.h',
    'matrix-event-room-power-levels.h',
    'matrix-event-room-guest-access.h',
    'matrix-event-room-message-feedback.h',
    'matrix-event-room-redaction.h',
    'matrix-event-room-third-party-invite.h',
    'matrix-event-typing.h',
    'matrix-event-receipt.h'
]

message_h_files = [
    'matrix-message-base.h',
    'matrix-message-text.h',
    'matrix-message-location.h',
    'matrix-message-emote.h',
    'matrix-message-file.h',
    'matrix-message-image.h',
    'matrix-message-audio.h',
    'matrix-message-video.h',
    'matrix-message-notice.h'
]

inst_h_src_files = [
    'matrix-types.h',
    'matrix-compacts.h',
    'matrix-api.h',
    'matrix-http-api.h',
    'matrix-client.h',
    'matrix-http-client.h',
    'utils.h',
    'matrix-profile.h',
    'matrix-room.h',
    event_h_files,
    message_h_files,
    enums[1],
    marshalers[1]
]

version_conf = configuration_data()
version_conf.set('MATRIX_GLIB_MAJOR_VERSION', matrix_glib_major_version)
version_conf.set('MATRIX_GLIB_MINOR_VERSION', matrix_glib_minor_version)
version_conf.set('MATRIX_GLIB_MICRO_VERSION', matrix_glib_micro_version)

version_file = configure_file(input : 'matrix-version.h.in',
                              output : 'matrix-version.h',
                              configuration : version_conf,
                              install : false)

sources = [
    version_file,
    enums,
    marshalers,
    inst_h_src_files,
    'matrix-event-types.c',
    'matrix-version.c',
    'matrix-api.c',
    'matrix-http-api.c',
    'matrix-client.c',
    'matrix-http-client.c',
    'matrix-types.c',
    'matrix-compacts.c',
    'matrix-event-base.c',
    'matrix-event-call-base.c',
    'matrix-event-call-answer.c',
    'matrix-event-call-hangup.c',
    'matrix-event-call-invite.c',
    'matrix-event-call-candidates.c',
    'matrix-message-base.c',
    'matrix-message-text.c',
    'matrix-message-location.c',
    'matrix-message-emote.c',
    'matrix-message-file.c',
    'matrix-message-image.c',
    'matrix-message-audio.c',
    'matrix-message-video.c',
    'matrix-message-notice.c',
    'matrix-event-tag.c',
    'matrix-event-presence.c',
    'matrix-event-room-member.c',
    'matrix-event-typing.c',
    'matrix-event-room-base.c',
    'matrix-event-state-base.c',
    'matrix-event-receipt.c',
    'matrix-event-room-topic.c',
    'matrix-event-room-aliases.c',
    'matrix-event-room-avatar.c',
    'matrix-event-room-name.c',
    'matrix-event-room-message.c',
    'matrix-event-room-history-visibility.c',
    'matrix-event-room-join-rules.c',
    'matrix-event-room-canonical-alias.c',
    'matrix-event-room-create.c',
    'matrix-event-room-power-levels.c',
    'matrix-event-room-message-feedback.c',
    'matrix-event-room-redaction.c',
    'matrix-event-room-guest-access.c',
    'matrix-event-room-third-party-invite.c',
    'matrix-profile.c',
    'matrix-room.c',
    'utils.c',
]

enum_dep = declare_dependency(sources : enums[1])
marshaler_dep = declare_dependency(sources : marshalers[1])

deps = [glib, gobject, soup, json, enum_dep, marshaler_dep]

mapfile = 'matrix-glib.map'
matrixglib = library(
    'matrix-glib', sources,
    soversion : lt_current,
    version : matrix_glib_api_version,
    dependencies : deps,
    include_directories : config_inc,
    link_depends : mapfile,
    install : true)

if get_option('test-clients')
    test_api_client = executable('test-api-client', 'test-api-client.c',
                                 dependencies : [glib, json, soup],
                                 link_with : matrixglib)
    test_client = executable('test-client', 'test-client.c',
                             dependencies : [glib, json, enum_dep],
                             link_with : matrixglib)
endif

if get_option('introspection')
    matrix_gir = gnome.generate_gir(matrixglib,
                                    sources : sources,
                                    nsversion : matrix_glib_api_version,
                                    namespace : 'Matrix',
                                    symbol_prefix : 'matrix_',
                                    identifier_prefix : 'Matrix',
                                    export_packages : 'matrix-glib',
                                    dependencies : deps,
                                    includes : [
                                      'GObject-2.0',
                                      'Gio-2.0',
                                      'Soup-2.4',
                                      'Json-1.0'
                                    ],
                                    link_with : matrixglib,
                                    install : true)
endif

pkg.generate(name : 'libmatrix-glib',
             description : 'GObject API for communicating with a Matrix.org Homeserver',
             version : matrix_glib_version,
             filebase : 'matrix-glib-' + matrix_glib_api_version,
             libraries : '-L${libdir} -lmatrix-glib-' + matrix_glib_api_version,
             subdirs : 'matrix-glib-' + matrix_glib_api_version,
             requires : ['glib-2.0', 'gobject-2.0', 'gio-2.0', 'json-glib-1.0', 'libsoup-2.4'],
             url : 'https://gergely.polonkai.eu/matrix-glib-sdk/')
