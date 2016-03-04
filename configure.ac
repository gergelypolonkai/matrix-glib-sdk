# Process this file with autoconf to produce a configure script

# Sections
#   Version numbers
#   Setup
#   Dependencies
#   Output

### Version numbers

# Release Version
m4_define([matrix_glib_major_version], [0])
m4_define([matrix_glib_minor_version], [0])
m4_define([matrix_glib_micro_version], [1])
m4_define([matrix_glib_nano_version], [0])

# LT_VERSION
#   If library source has changed since last release, increment revision
#   If public symbols have been added, removed or changed since last
#     release, increment current and set revision to 0
#   If public symbols have been added since last release, increment age
#   If public symbols have been removed since last release, set age to 0
m4_define([matrix_glib_lt_current], [0])
m4_define([matrix_glib_lt_revision], [0])
m4_define([matrix_glib_lt_age], [0])

# Combine numbers to various version variables (different uses)
#   Display the nano_version only if it's not '0'
m4_define([matrix_glib_base_version],
          matrix_glib_major_version.matrix_glib_minor_version.matrix_glib_micro_version)
m4_define([matrix_glib_full_version],
          [m4_if(matrix_glib_nano_version, [0],
                 matrix_glib_base_version,
                 matrix_glib_base_version.matrix_glib_nano_version)])

# You should set project_released to one in order to mark this as a
# released version and to avoid date on version numbers
m4_define(matrix_glib_released, [0])
m4_define([matrix_glib_maybe_datestamp],
          m4_if(matrix_glib_released, [1],
                [],
                [m4_esyscmd([date +.%Y%m%d | tr -d '\n\r'])]))

m4_define([matrix_glib_version], matrix_glib_full_version[]matrix_glib_maybe_datestamp)
m4_define([matrix_glib_major_minor_version], matrix_glib_major_version.matrix_glib_minor_version)
m4_define([matrix_glib_module_version], matrix_glib_lt_current)
m4_define([matrix_glib_api_version], [matrix_glib_major_version.0])

### Setup

# Initialise autoconf with project details, version
AC_INIT([MatrixGlib], matrix_glib_version,
        [gergely@polonkai.eu], [matrix-glib])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_PREREQ([2.65])
AC_COPYRIGHT([Copyright (C) 2015,2016 Gergely Polonkai])

# Configure various files and settings for autoconf/automake
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([configure.ac])
AM_INIT_AUTOMAKE([1.11 dist-xz no-define
                  no-dist-gzip tar-ustar -Wno-portability])
AM_MAINTAINER_MODE([enable])

AC_PROG_CC
AM_PROG_CC_C_O
AM_PROG_AR
AC_PROG_LIBTOOL

# Initialise libtool (handles library creation) with versions from above
LT_INIT(win32-dll)

PKG_PROG_PKG_CONFIG([0.21])

AC_SUBST([CFLAGS])
AC_SUBST([CPPFLAGS])
AC_SUBST([LDFLAGS])

AC_SUBST([LT_CURRENT], matrix_glib_lt_current)
AC_SUBST([LT_REVISION], matrix_glib_lt_revision)
AC_SUBST([LT_AGE], matrix_glib_lt_age)

AC_SUBST([MATRIX_GLIB_VERSION], matrix_glib_base_version)

### Dependencies

GLIB_REQUIRED=2.40.0
VALA_REQUIRED=0.30.0
LIBVALA_REQUIRED=0.30
GEE_REQUIRED=0.10.5
GIO_REQUIRED=2.22
SOUP_REQUIRED=2.44.2
JSON_REQUIRED=0.16.2
VALADOC_REQUIRED=0.3.1

# Check GLib
PKG_CHECK_MODULES([GLIB],
                  [glib-2.0 >= $GLIB_REQUIRED
                   gobject-2.0 >= $GLIB_REQUIRED])

# Check for vala
VALAC_CHECK

# Check for libgee
PKG_CHECK_MODULES([GEE], [gee-0.8 >= $GEE_REQUIRED])

# Check for GIO
PKG_CHECK_MODULES([GIO], [gio-2.0 >= $GIO_REQUIRED])

# Check for libSoup
PKG_CHECK_MODULES([SOUP], [libsoup-2.4 >= $SOUP_REQUIRED])

# Check for JSON-GLib
PKG_CHECK_MODULES([JSON], [json-glib-1.0 >= $JSON_REQUIRED])

# Check for GObject Introspection
GOBJECT_INTROSPECTION_CHECK([1.32.0])

# Documentation with Valadoc
AC_ARG_ENABLE([docs],
              AS_HELP_STRING([--enable-docs],
                             [Enable documentation generation]),
              [enable_docs=$enableval], [enable_docs=no])
AM_CONDITIONAL([ENABLE_DOCS], [test x$enable_docs = xyes])

have_valadoc=no
if test x$enable_docs = xyes; then
    # Make sure the library is new enough and the program exists
    AC_PATH_PROG([VALADOC], [valadoc], [no])
    if test "x$VALADOC" = "xno"; then
        AC_MSG_RESULT([valadoc documentation will not build because valadoc is not found])
    else
        VAPIDIR=`$PKG_CONFIG --variable=vapidir libvala-$LIBVALA_REQUIRED`
        have_valadoc="yes"
        AC_SUBST([VALADOC])
    fi
fi
AM_CONDITIONAL([HAVE_VALADOC], [test x$have_valadoc = xyes])

AC_ARG_ENABLE([gtk-doc],
              AS_HELP_STRING([--enable-gtk-doc],
                             [Enable GTK-Doc documentation generation]),
              [enable_gtk_docs=$enableval], [enable_gtk_docs=no])

have_gtkdoc=no
if test x$enable_gtk_docs = xyes; then
    if test x$have_valadoc = xyes; then
        PKG_CHECK_MODULES([GTKDOC], gtk-doc)
        have_gtkdoc=yes
    else
        AC_MSG_ERROR([valadoc is required to generate GTK-Doc documentation. Use --enable-valadoc])
    fi
fi

gtkdocs=no

if test x$enable_docs = xyes; then
    if test x$enable_gtk_docs = xyes; then
        gtkdocs=yes
    fi
fi

AM_CONDITIONAL([ENABLE_GTK_DOCS], [test "x$gtkdocs" = "xyes"])

debug=no
AC_ARG_ENABLE(debug,
              AS_HELP_STRING([--enable-debug],
                             [compile with debugging support]),
              [debug=$enableval], [debug=no])
AM_CONDITIONAL([DEBUG], [test $debug = yes])

if test x"$debug" = x"$enableval"; then
    AC_DEFINE([DEBUG], [1], [Define if debugging should be enabled])

    if test x"$cflags_set" != x"set"; then
        case " $CFLAGS " in
            *[[\ \	]]-g[[\ \	]]*) ;;
            *) CFLAGS="$CFLAGS -g" ;;
        esac
    fi
fi

AC_SUBST([MATRIX_GLIB_MAJOR_VERSION], matrix_glib_major_version)
AC_SUBST([MATRIX_GLIB_MINOR_VERSION], matrix_glib_minor_version)
AC_SUBST([MATRIX_GLIB_MICRO_VERSION], matrix_glib_micro_version)

# This is used to identify a set of API compatible between versions
# - If new versions are compatible with the actual one, just leave
#   this untouched
# - If new version breaks API change it in order to allow parallel
#   installations with old versions. Change name of pc files to use
#   a new API, too
AC_SUBST([MATRIX_GLIB_API_VERSION], matrix_glib_api_version)

AC_PATH_PROG([GLIB_GENMARSHAL], [glib-genmarshal])

AC_CONFIG_FILES([
    Makefile
    src/Makefile
    src/matrix-version.h
    docs/Makefile
    docs/valadoc/Makefile
    docs/valadoc/gtk-doc/Makefile
])
AC_OUTPUT

# Print configuration summary
echo ""
echo " Configuration summary for Matrix-$MATRIX_GLIB_VERSION"
echo "   Installation prefix:     $prefix"
echo "   Documentation:           ${enable_docs}"
echo "       Gtk-Doc:             `if test x${gtkdocs} = xyes; then echo yes; else echo no; fi`"
echo "   Debug:                   $debug"
echo ""
