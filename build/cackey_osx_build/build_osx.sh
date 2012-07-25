#!/bin/bash
# Shell Script to make Mac OS X Releases of CACKey
# Kenneth Van Alstyne
# kenneth.l.vanalstyne@usace.army.mil
CACKEY_VERSION=`cat configure.ac | grep AC_INIT | cut -d " " -f 2 | sed 's_)__'`

# Check to see if we're building on Mac OS X 10.7 "Lion"
if [ "`uname -r | cut -d . -f 1`" = "11" ]; then
	LIONBUILD=1
fi

# Usage function
usage() {
	echo "Usage: build_osx.sh <target>"
	echo Where target is one of:
	echo "    leopard  - (Builds Universal 10.5 Library for PPCG4/i386)"
	echo "    snowleopard  - (Builds Universal 10.6 Library for i386/x86_64)"
	echo "    lion  - (Builds Universal 10.7 Library for i386/x86_64)"
	echo "    sltoml - (Builds Universal 10.6/10.7/10.8 Library for i386/x86_64)"
	echo "    all - (Builds for all supported targets)"
	echo "    clean - (Cleans up)"
	echo "Run from CACKey Build Root."
	exit $?
}

# Clean up function
clean() {
	rm -f build/cackey_osx_build/cackey.dylib
	rm -rf build/cackey_osx_build/PKCS11.tokend
	rm -rf PKCS11.tokend
	rm -rf macbuild
	rm -rf build/cackey_osx_build/*.pmdoc
	make distclean
}

# Directory creation function
makedir() {
	if [ "`uname -r | cut -d . -f 1`" -lt "10" ]; then
		LIBTOOLDIR=/Developer/usr/share/libtool
	else
		LIBTOOLDIR=/Developer/usr/share/libtool/config
	fi
	if [ ! -d macbuild ]; then
		mkdir macbuild
		mkdir macbuild/Leopard
		mkdir macbuild/Snowleopard
		mkdir macbuild/Lion
		mkdir macbuild/Sltoml
		mkdir macbuild/pkg
	fi
	if [ ! -f config.guess ]; then
		cp ${LIBTOOLDIR}/config.guess .
	fi
	if [ ! -f config.sub ]; then
		cp ${LIBTOOLDIR}/config.sub .
	fi
	if [ ! -f install-sh ]; then
		cp ${LIBTOOLDIR}/install-sh .
	fi
}

# Build function for Leopard
leopard() {
	makedir
	HEADERS=/Developer/SDKs/MacOSX10.5.sdk/System/Library/Frameworks/PCSC.framework/Versions/A/Headers/
	LIBRARY=/Developer/SDKs/MacOSX10.5.sdk/System/Library/Frameworks/PCSC.framework/PCSC
	LIB=""
	ARCHLIST=""
	DLIB=""
	DARCHLIST=""
	OSX=Leopard
	PKTARGETOS=3
	NEXTOSXVER=10.6
	CUROSXVER=10.5
	for HOST in powerpc-apple-darwin9 i386-apple-darwin9; do
		genbuild
	done
	libbuild
	pkgbuild
}

# Build function for Snow Leopard
snowleopard() {
	makedir
	HEADERS=/Developer/SDKs/MacOSX10.6.sdk/System/Library/Frameworks/PCSC.framework/Versions/A/Headers/
	LIBRARY=/Developer/SDKs/MacOSX10.6.sdk/System/Library/Frameworks/PCSC.framework/PCSC
	LIB=""
	ARCHLIST=""
	DLIB=""
	DARCHLIST=""
	OSX=Snowleopard
	PKTARGETOS=3
	NEXTOSXVER=10.7
	CUROSXVER=10.6
	for HOST in i386-apple-darwin10 x86_64-apple-darwin10; do
		genbuild
	done
	libbuild
	pkgbuild
}

# Build function for Lion
lion() {
	makedir
	HEADERS=/Developer/SDKs/MacOSX10.7.sdk/System/Library/Frameworks/PCSC.framework/Versions/A/Headers/
	LIBRARY=/Developer/SDKs/MacOSX10.7.sdk/System/Library/Frameworks/PCSC.framework/PCSC
	LIB=""
	ARCHLIST=""
	DLIB=""
	DARCHLIST=""
	OSX=Lion
	PKTARGETOS=3
	NEXTOSXVER=10.8
	CUROSXVER=10.7
	for HOST in i386-apple-darwin11 x86_64-apple-darwin11; do
		genbuild
	done
	libbuild
	pkgbuild
}

# Build function for Snow Leopard/Lion/Mountain Lion
sltoml() {
	makedir
	HEADERS=/Developer/SDKs/MacOSX10.6.sdk/System/Library/Frameworks/PCSC.framework/Versions/A/Headers/
	LIBRARY=/Developer/SDKs/MacOSX10.6.sdk/System/Library/Frameworks/PCSC.framework/PCSC
	LIB=""
	ARCHLIST=""
	DLIB=""
	DARCHLIST=""
	OSX=Sltoml
	PKTARGETOS=3
	NEXTOSXVER=10.9
	CUROSXVER=10.6
	for HOST in i386-apple-darwin10 x86_64-apple-darwin10; do
		genbuild
	done
	libbuild
	pkgbuild
}

# Build function for Snow Leopard/Lion/Mountain Lion
sltoml() {
	makedir
	HEADERS=/Developer/SDKs/MacOSX10.6.sdk/System/Library/Frameworks/PCSC.framework/Versions/A/Headers/
	LIBRARY=/Developer/SDKs/MacOSX10.6.sdk/System/Library/Frameworks/PCSC.framework/PCSC
	LIB=""
	ARCHLIST=""
	DLIB=""
	DARCHLIST=""
	OSX=Sltoml
	PKTARGETOS=3
	NEXTOSXVER=10.9
	CUROSXVER=10.6
	for HOST in i386-apple-darwin10 x86_64-apple-darwin10; do
		genbuild
	done
	libbuild
	pkgbuild
}

# Generic build function
genbuild() {
	make distclean
	ARCH=`echo ${HOST} | cut -d "-" -f 1`
	if [ ${ARCH} == "powerpc" ]; then
		if [ ${OSX} == "Leopard" ]; then
			ARCH="ppc -mcpu=G4"
		else
			ARCH="ppc -mcpu=G3"
		fi
	fi
	if [ "${LIONBUILD}" = 1 ]; then
		if [ "${ARCH}" == "ppc -mcpu=G4" ]; then
			CC=powerpc-apple-darwin11-gcc-4.2.1 CPP=powerpc-apple-darwin11-cpp-4.2.1 CFLAGS="-m32 -I/Developer/Platforms/iPhoneOS.platform/Developer/usr/lib/gcc/powerpc-apple-darwin10/4.2.1/include -isysroot /Developer/SDKs/MacOSX10.5.sdk" CPPFLAGS="-D_LIBC_LIMITS_H_" ./configure --with-pcsc-headers=${HEADERS} --with-pcsc-libs=${LIBRARY} --host=${HOST} --enable-dod-certs-on-hw-slots
		else
			CFLAGS="-arch ${ARCH}" ./configure --with-pcsc-headers=${HEADERS} --with-pcsc-libs=${LIBRARY} --host=${HOST} --enable-dod-certs-on-hw-slots
		fi
	else
		CFLAGS="-arch ${ARCH}" ./configure --with-pcsc-headers=${HEADERS} --with-pcsc-libs=${LIBRARY} --host=${HOST} --enable-dod-certs-on-hw-slots
	fi
	make
	cp libcackey.dylib macbuild/${OSX}/libcackey.dylib.`echo ${ARCH} | cut -d ' ' -f 1`
	cp libcackey_g.dylib macbuild/${OSX}/libcackey_g.dylib.`echo ${ARCH} | cut -d ' ' -f 1` 
}

# Library build function
libbuild() {
	for LIB in macbuild/${OSX}/libcackey.dylib.*; do
		ARCHLIST="${ARCHLIST} `echo '-arch '` `echo ${LIB} | cut -d . -f 3` `echo ' '` `echo ${LIB}`"
	done
	lipo -create ${ARCHLIST} -output macbuild/${OSX}/libcackey.dylib
	for DLIB in macbuild/${OSX}/libcackey_g.dylib.*; do
		DARCHLIST="${DARCHLIST} `echo '-arch '` `echo ${DLIB} | cut -d . -f 3` `echo ' '` `echo ${DLIB}`"
	done
	lipo -create ${DARCHLIST} -output macbuild/${OSX}/libcackey_g.dylib
	rm macbuild/${OSX}/libcackey*.dylib.*
}

# Function to build Mac OS X Packages
pkgbuild() {
	if [ "`uname -r | cut -d . -f 1`" -lt "10" ]; then
		LIBCACKEYG=libcackeyg.pkg
	else
		LIBCACKEYG=libcackey_g.pkg
	fi
	rm -f build/cackey_osx_build/cackey.dylib
	ln macbuild/${OSX}/libcackey.dylib build/cackey_osx_build/cackey.dylib
	rm -rf build/cackey_osx_build/PKCS11.tokend
	if [ "${LIONBUILD}" = 1 ]; then
		TAR=gnutar
	else
		TAR=tar
	fi
	if [ "${OSX}" = "Leopard" ]; then
		curl http://devel.kvanals.org/PKCS11_Tokend/PKCS11_tokend-latest.tar.gz | ${TAR} --strip-components 3 --wildcards -x -z -f - "PKCS11_tokend-*/prebuilt/leopard/PKCS11.tokend"
		mv PKCS11.tokend build/cackey_osx_build/PKCS11.tokend
	else
		curl http://devel.kvanals.org/PKCS11_Tokend/PKCS11_tokend-latest.tar.gz | ${TAR} --strip-components 3 --wildcards -x -z -f - "PKCS11_tokend-*/prebuilt/snowleopard/PKCS11.tokend"
		mv PKCS11.tokend build/cackey_osx_build/PKCS11.tokend
	fi
	for PMDOC in build/cackey_osx_build/Template_pmbuild/*.in; do
		PMDOC="`echo "${PMDOC}" | sed 's|l.in|l|g' | sed 's|build/cackey_osx_build/Template_pmbuild/||g'`"
		UUID="`python -c 'import uuid; print uuid.uuid1()' | dd conv=ucase 2>/dev/null`"
		mkdir -p build/cackey_osx_build/${OSX}_pmbuild.pmdoc
		sed "s|@@BUILDROOTDIR@@|$(pwd)|g" build/cackey_osx_build/Template_pmbuild/${PMDOC}.in > build/cackey_osx_build/${OSX}_pmbuild.pmdoc/${PMDOC}
		sed "s|@@OSXVERSION@@|${OSX}|g" build/cackey_osx_build/${OSX}_pmbuild.pmdoc/${PMDOC} > build/cackey_osx_build/${OSX}_pmbuild.pmdoc/${PMDOC}.1
		sed "s|@@UUID@@|${UUID}|g" build/cackey_osx_build/${OSX}_pmbuild.pmdoc/${PMDOC}.1 > build/cackey_osx_build/${OSX}_pmbuild.pmdoc/${PMDOC}
		sed "s|@@TARGETOS@@|${PKTARGETOS}|g" build/cackey_osx_build/${OSX}_pmbuild.pmdoc/${PMDOC} > build/cackey_osx_build/${OSX}_pmbuild.pmdoc/${PMDOC}.1
		sed "s|@@NEXTOSXVER@@|${NEXTOSXVER}|g" build/cackey_osx_build/${OSX}_pmbuild.pmdoc/${PMDOC}.1 > build/cackey_osx_build/${OSX}_pmbuild.pmdoc/${PMDOC}
		sed "s|@@CUROSXVER@@|${CUROSXVER}|g" build/cackey_osx_build/${OSX}_pmbuild.pmdoc/${PMDOC} > build/cackey_osx_build/${OSX}_pmbuild.pmdoc/${PMDOC}.1
		sed "s|@@LIBCACKEYG@@|${LIBCACKEYG}|g" build/cackey_osx_build/${OSX}_pmbuild.pmdoc/${PMDOC}.1 > build/cackey_osx_build/${OSX}_pmbuild.pmdoc/${PMDOC}
		cp build/cackey_osx_build/${OSX}_pmbuild.pmdoc/${PMDOC} build/cackey_osx_build/${OSX}_pmbuild.pmdoc/${PMDOC}.1
		mv build/cackey_osx_build/${OSX}_pmbuild.pmdoc/${PMDOC}.1 build/cackey_osx_build/${OSX}_pmbuild.pmdoc/${PMDOC}
	done
	EXT=pkg
	if [ ${OSX} == "Snowleopard" ]; then
		cat build/cackey_osx_build/${OSX}_pmbuild.pmdoc/index.xml | sed 's|for Mac OS X Snowleopard|for Mac OS X SnowLeopard|g' > build/cackey_osx_build/${OSX}_pmbuild.pmdoc/index.xml.new
		mv build/cackey_osx_build/${OSX}_pmbuild.pmdoc/index.xml.new build/cackey_osx_build/${OSX}_pmbuild.pmdoc/index.xml
	fi
	if [ ${OSX} == "Sltoml" ]; then
		cat build/cackey_osx_build/${OSX}_pmbuild.pmdoc/index.xml | sed 's|for Mac OS X Sltoml|for Mac OS X SLtoML|g' > build/cackey_osx_build/${OSX}_pmbuild.pmdoc/index.xml.new
		mv build/cackey_osx_build/${OSX}_pmbuild.pmdoc/index.xml.new build/cackey_osx_build/${OSX}_pmbuild.pmdoc/index.xml
	fi
	/Developer/Applications/Utilities/PackageMaker.app/Contents/MacOS/PackageMaker -d build/cackey_osx_build/${OSX}_pmbuild.pmdoc -o macbuild/pkg/CACKey_${CACKEY_VERSION}_${OSX}.${EXT}
	tar --create --directory macbuild/pkg/ --file macbuild/pkg/CACKey_${CACKEY_VERSION}_${OSX}.${EXT}.tar CACKey_${CACKEY_VERSION}_${OSX}.${EXT}
	gzip -9 macbuild/pkg/CACKey_${CACKEY_VERSION}_${OSX}.${EXT}.tar
	rm -rf macbuild/pkg/CACKey_${CACKEY_VERSION}_${OSX}.${EXT}
	rm -f build/cackey_osx_build/cackey.dylib
	rm -rf build/cackey_osx_build/PKCS11.tokend
	echo "${OSX} build complete"
}

# Take command line arguments and execute
case "$1" in
	"")
		usage
		exit $?
	;;

	"leopard")
		./autogen.sh
		leopard
		exit $?
	;;

	"snowleopard")
		./autogen.sh
		snowleopard
		exit $?
	;;
	
	"lion")
		./autogen.sh
		lion
		exit $?
	;;
	
	"sltoml")
		./autogen.sh
		sltoml
		exit $?
	;;

	"all")
		./autogen.sh
		leopard
		sltoml
		echo ""
		echo "All builds complete."
		exit $?
	;;

	"clean")
		clean
		exit $?
	;;

	*)
		usage
		exit $?
	;;
esac 
