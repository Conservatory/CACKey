CC = x86_64-w64-mingw32-gcc
CFLAGS = -g -O2 -mms-bitfields -I/home/magnus/p/cackey-github/build/cackey_win64_build/include 
DEBUGCFLAGS = -g -O2 -mms-bitfields -I/home/magnus/p/cackey-github/build/cackey_win64_build/include  -DCACKEY_DEBUG=1
CPPFLAGS = -I/home/magnus/p/cackey-github/build/cackey_win64_build/include -mms-bitfields -I/home/magnus/p/cackey-github/build/cackey_win64_build/include -DHAVE_CONFIG_H -I./pkcs11/
DEBUGCPPFLAGS = -I/home/magnus/p/cackey-github/build/cackey_win64_build/include -mms-bitfields -I/home/magnus/p/cackey-github/build/cackey_win64_build/include -DCACKEY_DEBUG=1 -I./pkcs11/ -DHAVE_CONFIG_H
LDFLAGS =  -Wl,--version-script -Wl,libcackey.vers 
LIBS = -lz  -L/home/magnus/p/cackey-github/build/cackey_win64_build/lib -lwinscard 
SHOBJFLAGS = -DPIC
SHOBJLDFLAGS = -shared -Wl,--dll -Wl,--enable-auto-image-base -Wl,--output-def,$@.def,--out-implib,$@.a
AR = x86_64-w64-mingw32-ar
RANLIB = x86_64-w64-mingw32-ranlib
prefix = /usr/local
exec_prefix = ${prefix}
libdir = ${exec_prefix}/lib


all:
	$(MAKE) libcackey.dll
	-$(MAKE) libcackey_g.dll

cackey.o: cackey.c cackey_builtin_certs.h sha1.c sha1.h md5.c md5.h asn1-x509.c asn1-x509.h config.h
	$(CC) $(SHOBJFLAGS) $(CPPFLAGS) $(CFLAGS) -o cackey.o -c cackey.c

cackey_g.o: cackey.c cackey_builtin_certs.h sha1.c sha1.h md5.c md5.h asn1-x509.c asn1-x509.h config.h
	$(CC) $(SHOBJFLAGS) $(DEBUGCPPFLAGS) $(DEBUGCFLAGS) -o cackey_g.o -c cackey.c

libcackey.dll: cackey.o
	$(CC) $(SHOBJFLAGS) $(CPPFLAGS) $(CFLAGS) $(SHOBJLDFLAGS) $(LDFLAGS) -o libcackey.dll cackey.o $(LIBS)
	-x86_64-w64-mingw32-objcopy --keep-global-symbols=libcackey.syms "libcackey.dll"
	-x86_64-w64-mingw32-objcopy --discard-all "libcackey.dll"

libcackey_g.dll: cackey_g.o
	$(CC) $(SHOBJFLAGS) $(DEBUGCPPFLAGS) $(DEBUGCFLAGS) $(SHOBJLDFLAGS) $(LDFLAGS) -o libcackey_g.dll cackey_g.o $(LIBS)
	-x86_64-w64-mingw32-objcopy --keep-global-symbols=libcackey.syms "libcackey_g.dll"

test: test.c libcackey_g.dll
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o test test.c -Wl,-R,. libcackey_g.dll

splint-cackey.txt: cackey.c cackey_builtin_certs.h asn1-x509.c asn1-x509.h config.h
	splint $(DEBUGCPPFLAGS) -DCACKEY_PARANOID=1 -weak +posixlib -I/usr/include/PCSC -Ipkcs11 cackey.c > splint-cackey.txt

install: libcackey.dll
	-mkdir -p "$(DESTDIR)$(libdir)"
	rm -f "$(DESTDIR)$(libdir)/libcackey.dll"
	rm -f "$(DESTDIR)$(libdir)/libcackey_g.dll"
	cp "libcackey.dll" "$(DESTDIR)$(libdir)/"
	-cp "libcackey_g.dll" "$(DESTDIR)$(libdir)/"

clean:
	rm -f libcackey.dll libcackey_g.dll
	rm -f libcackey.dll.def libcackey_g.dll.def
	rm -f libcackey.dll.a libcackey_g.dll.a
	rm -f cackey.o cackey_g.o
	rm -f test
	rm -f splint-cackey.txt

distclean: clean
	rm -f config.log config.status config.h Makefile libcackey.syms

mrproper: distclean
	rm -f configure config.h.in aclocal.m4 *~

.PHONY: all clean distclean mrproper install
