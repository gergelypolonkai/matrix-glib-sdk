dnl vapigen.m4
dnl
dnl Copyright 2014 Daniel Espinosa
dnl
dnl This library is free software; you can redistribute it and/or
dnl modify it under the terms of the GNU Lesser General Public
dnl License as published by the Free Software Foundation; either
dnl version 2.1 of the License, or (at your option) any later version.
dnl
dnl This library is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl Lesser General Public License for more details.
dnl
dnl You should have received a copy of the GNU Lesser General Public
dnl License along with this library; if not, write to the Free Software
dnl Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

# VALAC_CHECK([VERSION], [API_VERSION])
# --------------------------------------
# Check valac existence and version.
m4_define([_VALAC_CHECK_INTERNAL],
[
  AC_REQUIRE([PKG_PROG_PKG_CONFIG])
  AS_IF([ test "x$2" = "x" ], [
    AM_PROG_VALAC($1)
    AS_IF([ test "x$VALAC" = "xno"], [
      check_prog=no
      AC_MSG_ERROR([valac is not installed])
    ], [
      check_prog=yes
      VALAC_VERSION=`$VALAC --api-version`
      VAPIDIR=$datadir/vala-$VALAC_VERSION/vapi
      VAPIDIR_UNVERSIONED=$datadir/vala-$VALAC_VERSION/vapi
    ])
  ], [
    AS_IF([ test "x$1" = "x"], [
        vala_pkg="libvala-$2"
      ], [
        vala_pkg="libvala-$2 >= $1"
      ])
	  AC_MSG_CHECKING([for $vala_pkg])
	  PKG_CHECK_EXISTS([ $vala_pkg ], [
	          vala_pkg_found=yes
            VAPIGEN=`$PKG_CONFIG --variable=vapigen libvala-$2`
	          VAPIGEN_MAKEFILE=`$PKG_CONFIG --variable=datadir libvala-$2`/vala/Makefile.vapigen
	          VAPIDIR=`$PKG_CONFIG --variable=vapidir libvala-$2`
            GEN_INTROSPECT=`$PKG_CONFIG --variable=gen_introspect libvala-$2`
            vala_datadir=`$PKG_CONFIG --variable=datadir libvala-$2`
            VAPIDIR_UNVERSIONED=$vala_datadir/vala/vapi
            VALA_GEN_INTROSPECT=`$PKG_CONFIG --variable=vala_gen_introspect libvala-$2`
	    ], [
	          vala_pkg_found=no
	          AC_MSG_ERROR([no package libvala-$2 is installed])
	    ])
	  AC_MSG_RESULT([$vala_pkg_found])
  ])
	AC_SUBST([VAPIGEN])
	AC_SUBST([VAPIGEN_VAPIDIR])
	AC_SUBST([VAPIDIR])
	AC_SUBST([VAPIDIR_UNVERSIONED])
	AC_SUBST([VAPIGEN_MAKEFILE])
	AC_SUBST([GEN_INTROSPECT])
	AC_SUBST([VALA_GEN_INTROSPECT])
	AC_SUBST([VALAC_VERSION])
])

dnl Check VAPIGEN
# VALAC_CHECK([VERSION], [API_VERSION], [DEFAULT])
# --------------------------------------
# Check vapigen existence and version.
AC_DEFUN([VALAC_CHECK],
[
  _VALAC_CHECK_INTERNAL($1,$2,$3)
]
)
