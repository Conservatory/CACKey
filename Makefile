CC = gcc
CFLAGS = -g -O2 -I/usr/include/PCSC 
DEBUGCFLAGS = -g -O2 -I/usr/include/PCSC  -DCACKEY_DEBUG=1
CPPFLAGS =  -I/usr/include/PCSC -DHAVE_CONFIG_H -I./pkcs11/
DEBUGCPPFLAGS =  -I/usr/include/PCSC -DCACKEY_DEBUG=1 -I./pkcs11/ -DHAVE_CONFIG_H
LDFLAGS =  -Wl,--version-script -Wl,libcackey.vers 
LIBS = -lz  -lpcsclite -lpthread
SHOBJFLAGS = -fPIC -DPIC
SHOBJLDFLAGS = -shared -rdynamic
AR = ar
RANLIB = ranlib
prefix = /usr/local
exec_prefix = ${prefix}
libdir = ${exec_prefix}/lib


all:
	$(MAKE) libcackey.so
	-$(MAKE) libcackey_g.so

cackey.o: cackey.c cackey_builtin_certs.h sha1.c sha1.h md5.c md5.h asn1-x509.c asn1-x509.h config.h
	$(CC) $(SHOBJFLAGS) $(CPPFLAGS) $(CFLAGS) -o cackey.o -c cackey.c

cackey_g.o: cackey.c cackey_builtin_certs.h sha1.c sha1.h md5.c md5.h asn1-x509.c asn1-x509.h config.h
	$(CC) $(SHOBJFLAGS) $(DEBUGCPPFLAGS) $(DEBUGCFLAGS) -o cackey_g.o -c cackey.c

libcackey.so: cackey.o
	$(CC) $(SHOBJFLAGS) $(CPPFLAGS) $(CFLAGS) $(SHOBJLDFLAGS) $(LDFLAGS) -o libcackey.so cackey.o $(LIBS)
	-objcopy --keep-global-symbols=libcackey.syms "libcackey.so"
	-objcopy --discard-all "libcackey.so"

libcackey_g.so: cackey_g.o
	$(CC) $(SHOBJFLAGS) $(DEBUGCPPFLAGS) $(DEBUGCFLAGS) $(SHOBJLDFLAGS) $(LDFLAGS) -o libcackey_g.so cackey_g.o $(LIBS)
	-objcopy --keep-global-symbols=libcackey.syms "libcackey_g.so"

test: test.c libcackey_g.so
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o test test.c -Wl,-R,. libcackey_g.so

test-afl.data: test
	tmpLogFile='log.$(shell openssl rand -hex 16)'; \
		./test 2> $${tmpLogFile}; \
		echo -ne "$$( \
			grep 'Returned Value:' $${tmpLogFile} | sed 's@^.*/@@;s@ = {@ @;s@})$$@@;s@,@@g;s@ @\\x@g;s@\\@ \\@' | while IFS=' ' read -r count string; do \
				printf '\\x%02x\\x%02x%s' $$[$${count} / 256] $$[$${count} % 256] "$${string}"; \
			done \
		)" > test-afl.data; rm -f $${tmpLogFile}

test-afl: test-afl.data test.c cackey.c cackey_builtin_certs.h sha1.c sha1.h md5.c md5.h asn1-x509.c asn1-x509.h config.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -DCACKEY_TEST_AFL -o test-afl test.c $(patsubst -lpcsclite,,$(LIBS))

splint-cackey.txt: cackey.c cackey_builtin_certs.h asn1-x509.c asn1-x509.h config.h
	splint $(DEBUGCPPFLAGS) -DCACKEY_PARANOID=1 -weak +posixlib -I/usr/include/PCSC -Ipkcs11 cackey.c > splint-cackey.txt

install: libcackey.so
	-mkdir -p "$(DESTDIR)$(libdir)"
	rm -f "$(DESTDIR)$(libdir)/libcackey.so"
	rm -f "$(DESTDIR)$(libdir)/libcackey_g.so"
	cp "libcackey.so" "$(DESTDIR)$(libdir)/"
	-cp "libcackey_g.so" "$(DESTDIR)$(libdir)/"

clean:
	rm -f libcackey.so libcackey_g.so
	rm -f libcackey.so.def libcackey_g.so.def
	rm -f libcackey.so.a libcackey_g.so.a
	rm -f cackey.o cackey_g.o
	rm -f test
	rm -f splint-cackey.txt

distclean: clean
	rm -f config.log config.status config.h Makefile libcackey.syms

mrproper: distclean
	rm -f configure config.h.in aclocal.m4 *~

.PHONY: all clean distclean mrproper install
