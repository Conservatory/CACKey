#! /bin/bash

make distclean

./configure --with-pcsc-headers="$(pwd)/build/cackey_win32_build/include" --with-pcsc-libs="-L$(pwd)/build/cackey_win32_build/lib -lwinscard" --host=i586-mingw32msvc  CPPFLAGS="-I$(pwd)/build/cackey_win32_build/include" || exit 1

make || exit 1

exit 0
