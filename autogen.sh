#! /bin/sh

srcdir=`dirname "$0"`
test -z "$srcdir" && srcdir=.

PKG_NAME="matrix-client-glib"

(test -f "$srcdir/configure.ac" \
 && test -f "$srcdir/src/matrix-client.c") || {
    echo "**Error**: Directory "\`$srcdir\'" does not look like the"
    echo "           top-level $PKG_NAME directory"
    exit 1
}

cd "$srcdir"

m4dir=`grep '^AC_CONFIG_MACRO_DIR' configure.ac \
       | sed -n -e 's/AC_CONFIG_MACRO_DIR(\([^()]*\))/\1/p' \
       | sed -e 's/^\[\(.*\)\]$/\1/' \
       | sed -e 1q`
m4_dir=$m4dir

if test -n "$m4dir"]; then
    m4dir="-I $m4dir"
fi

echo "Running libtoolize"
libtoolize --force --copy || exit $?

GTKDOCIZE=$(which gtkdocize 2>/dev/null)

if test -z "$GTKDOCIZE"; then
    echo "You don't have gtk-doc installed, and thus won't be able to generate documentation"
    rm -f gtk-doc.make
    rm -f $m4_dir/gtk-doc.m4
    cat > gtk-doc.make <<EOF
EXTRA_DIST =
CLEANFILES =
EOF
else
    echo "Running gtkdocize"
    gtkdocize --copy || exit $?
fi

echo "Running aclocal"
aclocal $m4dir $ACLOCAL_FLAGS || exit $?

echo "Running autoconf"
autoconf || exit $?

if grep '^A[CM]_CONFIG_HEADER' configure.ac > /dev/null; then
    echo "Running autoheader"
    autoheader || exit $?
    test -f config.h.in && touch config.h.in
fi

echo "Running automake"
automake --gnu --add-missing --copy -Wno-portability || exit $?

"$srcdir/configure" "$@"
