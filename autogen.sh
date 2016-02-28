#! /bin/sh
# Run this to generate all the initial makefiles, etc.

srcdir=`dirname "$0"`
test -z "$srcdir" && srcdir=.

DIE=0
PKG_NAME="matrix-glib"

(test -f "$srcdir/configure.ac" \
 && test -f "$srcdir/src/matrix-client.vala") || {
    echo -n "**Error**: Directory "\`$srcdir\'" does not look like the"
    echo " top-level $PKG_NAME directory"

    exit 1
}

(autoconf --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "**Error**: You must have \`autoconf' installed."
    echo "Download the appropriate package for your distribution,"
    echo "or get the source tarball at ftp://ftp.gnu.org/pub/gnu/"

    DIE=1
}

(grep "^LT_INIT" $srcdir/configure.ac >/dev/null) && {
    (libtool --version) < /dev/null > /dev/null 2>&1 || {
        echo
        echo "**Error**: You must have \`libtool' installed."
        echo "You can get it from: ftp://ftp.gnu.org/pub/gnu/"

        DIE=1
    }
}

(automake --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "**Error**: You must have \`automake' installed."
    echo "You can get it from: ftp://ftp.gnu.org/pub/gnu/"

    DIE=1
    NO_AUTOMAKE=yes
}

# if no automake, don't bother testing for aclocal
test -n "$NO_AUTOMAKE" || (aclocal --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "**Error**: Missing \`aclocal'.  The version of \`automake'"
    echo "installed doesn't appear recent enough."
    echo "You can get automake from ftp://ftp.gnu.org/pub/gnu/"

    DIE=1
}

if test "$DIE" -eq 1; then
    exit 1
fi

if test -z "$*"; then
    echo "**Warning**: I am going to run \`configure' with no arguments."
    echo "If you wish to pass any to it, please specify them on the"
    echo \`$0\'" command line."
    echo
fi

case $CC in
xlc )
        am_opt=--include-deps;;
esac

coin="$srcdir/configure.ac"
dr=`dirname $coin`

cd $dr

m4dir=`grep '^AC_CONFIG_MACRO_DIR' configure.ac \
       | sed -n -e 's/AC_CONFIG_MACRO_DIR(\([^()]*\))/\1/p' \
       | sed -e 's/^\[\(.*\)\]$/\1/' \
       | sed -e 1q`
m4_dir=$m4dir

if test -n "$m4dir"]; then
    m4dir="-I $m4dir"
fi

aclocalinclude="$ACLOCAL_FLAGS"

if grep "^LT_INIT" "$coin" > /dev/null; then
    if test -z "$NO_LIBTOOLIZE"; then
        echo "Running libtoolize ..."
        libtoolize --force --copy || exit $?
    fi
fi

GTKDOCIZE=$(which gtkdocize 2> /dev/null)

if test -z "$GTKDOCIZE"; then
    echo "You don't have gtk-doc installed, and thus won't be able to generate documentation"
    rm -f gtk-doc.make
    rm -f $m4_dir/gtk-doc.m4
    cat > gtk-doc.make <<EOF
EXTRA_DIST =
CLEANFILES =
EOF
else
    echo "Running gtkdocize ..."
    gtkdocize --copy || exit $?
fi

echo "Running aclocal $aclocalinclude ..."
aclocal $m4dir $ACLOCAL_FLAGS $aclocalinclude || exit $?

if grep "^A[CM]_CONFIG_HEADER" "$coin" > /dev/null; then
    echo "Running autoheader ..."
    autoheader || exit $?
fi

echo "Running automake --gnu $am_opt ..."
automake --add-missing --copy --gnu $am_opt -Wno-portability || exit $?

echo "Running autoconf ..."
autoconf || exit $?

if test x$NOCONFIGURE = x; then
    echo Running "$srcdir/configure" "$@" ...
    "$srcdir/configure" "$@" \
        && echo Now type \`make\' to compile. || exit 1
else
    echo Skipping configure process.
fi
