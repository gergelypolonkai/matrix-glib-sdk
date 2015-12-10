m4_define([matrix_glib_minor_version], [0])
m4_define([matrix_glib_major_version], [0])
m4_define([matrix_glib_micro_version], [1])
m4_define([matrix_glib_version], [matrix_glib_major_version.matrix_glib_minor_version.matrix_glib_micro_version])
m4_define([matrix_glib_api_version], [matrix_glib_major_version.0])
m4_define([matrix_api_version], [0.5.2])

AC_INIT([MatrixGlib], [matrix_glib_version], [gergely@polonkai.eu], [matrix-glib])
AM_INIT_AUTOMAKE([-Wall foreign])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_SILENT_RULES([yes])

AC_ARG_ENABLE(debug,
  AC_HELP_STRING([--enable-debug],
                 [compile with debugging support]),
  [enable_debug=$enableval], [enable_debug=no])

if test x"$enable_debug" = x"$enableval"; then
    AC_DEFINE([DEBUG], [1], [Define if debugging should be enabled])

    if test x"$cflags_set" != x"set"; then
        case " $CFLAGS " in
            *[[\ \	]]-g[[\ \	]]*) ;;
            *) CFLAGS="$CFLAGS -g" ;;
        esac
    fi
fi

AC_SUBST([MATRIX_GLIB_MAJOR_VERSION], [matrix_glib_major_version])
AC_SUBST([MATRIX_GLIB_MINOR_VERSION], [matrix_glib_minor_version])
AC_SUBST([MATRIX_GLIB_MICRO_VERSION], [matrix_glib_micro_version])
AC_SUBST([MATRIX_GLIB_VERSION], [matrix_glib_version])
AC_SUBST([MATRIX_GLIB_API_VERSION], [matrix_glib_api_version])
AC_SUBST([MATRIX_GLIB_API_VERSION_U], [AS_TR_SH([matrix_glib_api_version])])
AC_SUBST([MATRIX_API_VERSION], [matrix_api_version])

AC_PROG_CC
AM_PROG_CC_C_O
AM_PROG_AR
AC_PROG_LIBTOOL
LT_INIT

m4_ifdef([GTK_DOC_CHECK], [
GTK_DOC_CHECK([1.20], [--flavour no-tmpl])
], [
AM_CONDITIONAL([ENABLE_GTK_DOC], false)
])

GOBJECT_INTROSPECTION_CHECK([1.0])

PKG_PROG_PKG_CONFIG
AC_PATH_PROG([GLIB_MKENUMS], [glib-mkenums])

PKG_CHECK_MODULES([GLIB], [glib-2.0 >= 2.38])
PKG_CHECK_MODULES([GOBJECT], [gobject-2.0 >= 2.38])
PKG_CHECK_MODULES([SOUP], [libsoup-2.4])

LIBMATRIX_GLIB_LIBS='$(top_builddir)/src/libmatrix-glib-$(MATRIX_GLIB_API_VERSION).la'
AC_SUBST(LIBMATRIX_GLIB_LIBS)

AC_CONFIG_FILES([
    Makefile
    src/Makefile
    src/matrix-version.h
])
AM_COND_IF([ENABLE_GTK_DOC], [
  AC_CONFIG_FILES([
    docs/reference/matrix-glib/version.xml
    docs/reference/matrix-glib/Makefile
  ])
])
AC_OUTPUT
