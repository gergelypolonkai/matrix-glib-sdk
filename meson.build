project('matrix-glib', 'c', version : '0.0.1')

gnome = import('gnome')
pkg = import('pkgconfig')

sdk_version = meson.project_version()
ver_arr = sdk_version.split('.')

matrix_glib_major_version = ver_arr[0]
matrix_glib_minor_version = ver_arr[1]
matrix_glib_micro_version = ver_arr[2]
matrix_glib_nano_version = '0'

lt_current = ver_arr[0]
matrix_glib_lt_revision = '0'
matrix_glib_lt_age = '0'

matrix_glib_base_version = meson.project_version()
matrix_glib_full_version = meson.project_version()

if matrix_glib_nano_version != '0'
    matrix_glib_full_version += '.' + matrix_glib_nano_version
endif

matrix_glib_released = '0'

if matrix_glib_released == '1'
    matrix_glib_maybe_datestamp = ''
else
    datestamp = run_command('date', '+.%Y%m%d')
    matrix_glib_maybe_datestamp = datestamp.stdout().strip()
endif

matrix_glib_version = matrix_glib_full_version + matrix_glib_maybe_datestamp
matrix_glib_major_minor_version = matrix_glib_major_version + '.' + matrix_glib_minor_version
matrix_glib_module_version = lt_current
matrix_glib_api_version = matrix_glib_major_version + '.0'


glib_required = '>= 2.40.0'
vala_required = '>= 0.30.0'
libvala_required = '>= 0.30'
gio_required = '>= 2.22'
soup_required = '>= 2.44.2'
json_required = '>= 0.16.2'

glib = dependency('glib-2.0', version : glib_required)
gobject = dependency('gobject-2.0', version : glib_required)
gio = dependency('gio-2.0', version : gio_required)
soup = dependency('libsoup-2.4', version : soup_required)
json = dependency('json-glib-1.0', version : json_required)

conf = configuration_data()

if get_option('debug')
    conf.set('DEBUG', 1)
    add_global_arguments('-g', language : 'c')

    if get_option('maintainer-mode')
        add_global_arguments('-O0', language : 'c')
    endif
else
    conf.set('DEBUG', 0)
endif

configure_file(output : 'config.h',
              configuration : conf)

config_inc = include_directories('.')

subdir('src')
subdir('docs')
