#! /bin/bash

make distclean

cp "./build/cackey_win32_build/lib/winscard.dll" "./build/cackey_win32_build/lib/WinSCard.dll"

./configure --with-pcsc-headers="$(pwd)/build/cackey_win32_build/include" --with-pcsc-libs="-L$(pwd)/build/cackey_win32_build/lib -lwinscard" --host=i586-mingw32msvc  CPPFLAGS="-I$(pwd)/build/cackey_win32_build/include" || exit 1

make || exit 1

rm -f "./build/cackey_win32_build/lib/WinSCard.dll"

exit 0
