#! /bin/sh

srcdir=`dirname "$0"`
test -z "$srcdir" && srcdir=.

ORIGDIR=`pwd`
cd "$srcdir"

autoreconf --force -v --install || exit 1
cd "$ORIGDIR" || exit $?

FLAGS_CPU="-march=native -mcpu=native -mtune=native"
FLAGS="-O2 -ftree-vectorize -fomit-frame-pointer -fno-strict-aliasing \
	-Wno-stringop-overflow -Wno-redundant-decls -Wno-unused-variable \
	-Wno-unused-function -Wno-unused-but-set-variable -DNDEBUG -pipe"

if test -z "$NOCONFIGURE"; then
    exec "$srcdir"/configure \
		CFLAGS="$FLAGS_CPU $FLAGS -Werror-implicit-function-declaration" \
		CXXFLAGS="$FLAGS_CPU $FLAGS" \
		--prefix=/usr/local "$@"
fi
