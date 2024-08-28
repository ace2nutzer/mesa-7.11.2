#! /bin/sh

srcdir=`dirname "$0"`
test -z "$srcdir" && srcdir=.

SRCDIR=`(cd "$srcdir" && pwd)`
ORIGDIR=`pwd`

if test "x$SRCDIR" != "x$ORIGDIR"; then
	echo "Mesa cannot be built when srcdir != builddir" 1>&2
	exit 1
fi

MAKEFLAGS=""

autoreconf --force -v --install || exit 1

FLAGS_CPU="-march=core2 -mcpu=core2 -mtune=core2"
FLAGS="-O2 -ftree-vectorize -fomit-frame-pointer -fno-strict-aliasing \
	-Wno-stringop-overflow -Wno-redundant-decls -Wno-unused-variable \
	-Wno-unused-function -Wno-unused-but-set-variable -DNDEBUG -pipe"

"$srcdir"/configure \
		CFLAGS="$FLAGS_CPU $FLAGS -Werror-implicit-function-declaration" \
		CXXFLAGS="$FLAGS_CPU $FLAGS" \
		--prefix=/usr "$@"
