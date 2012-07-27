#! /bin/sh

find . -type f -name '.*.sw?' | xargs rm -f
find . -type f -name '.nfs*' | xargs rm -f

if [ "${SNAPSHOT}" = "1" ]; then
	sed "s@\(AC_INIT([^)]*\))@\1.${VERS})@" configure.ac > configure.ac.new
	cat configure.ac.new > configure.ac
	rm -f configure.ac.new
fi

./autogen.sh || exit 1

if [ ! -x configure ]; then
	exit 1
fi

for basefile in install-sh config.sub config.guess; do
	for path in /usr/share/automake-*; do
		file="${path}/${basefile}"
		if [ -f "${file}" ]; then
			cp "${file}" .
			chmod 755 "./${basefile}"

			break
		fi
	done
done

if [ "${SNAPSHOT}" != "1" ]; then
	mv build build_delete

	mkdir build || exit 1
	cp -rp build_delete/cackey_win32_build build/
	cp -rp build_delete/cackey_win64_build build/
fi

exit 0
