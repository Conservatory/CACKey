#! /bin/sh

find . -type f -name '.*.sw?' | xargs rm -f
find . -type f -name '.nfs*' | xargs rm -f
find . -type f -name '*~' | xargs rm -f

if [ "${SNAPSHOT}" = "1" ]; then
	sed "s@\(AC_INIT([^)]*\))@\1.${VERS})@" configure.ac > configure.ac.new
	cat configure.ac.new > configure.ac
	rm -f configure.ac.new
fi

./autogen.sh || exit 1

if [ ! -x configure ]; then
	exit 1
fi

if [ "${SNAPSHOT}" != "1" ]; then
	mv build build_delete

	mkdir build || exit 1
	cp -rp build_delete/cackey_win32_build build/
	cp -rp build_delete/cackey_win64_build build/
fi

exit 0
