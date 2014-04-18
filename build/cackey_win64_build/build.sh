#! /bin/bash

make distclean

./configure --with-pcsc-headers="$(pwd)/build/cackey_win64_build/include" --with-pcsc-libs="-L$(pwd)/build/cackey_win64_build/lib -lwinscard" --host=x86_64-w64-mingw32 CPPFLAGS="-I$(pwd)/build/cackey_win64_build/include" || exit 1

make || exit 1

exit 0
