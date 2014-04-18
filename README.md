CACKey
======

CACKey provides a standard interface (PKCS#11) for smartcards connected
to a PC/SC compliant reader.  It performs a similar function to
"CoolKey", but only supports Government Smartcards.  It supports all
Government Smartcards that implement the Government Smartcard
Interoperability Specification (GSC-IS) v2.1 or newer.  

### Release information:
- pkg: CACKey version 20140418
- date: Fri Apr 18 13:19:40 EDT 2014
- contact: Daniel.R.Risacher.CIV@mail.mil

### Author information:
- pkg: CACKey version 201207191606
- date: Thu Jul 19 16:06:56 UTC 2012
- author: US Army Corps of Engineers
- contact: Roy Keene <email no longer valid>
           US Army Corps of Engineers
           Information Technology Laboratory
           Vicksburg, MS 39180

--------------------------------------------------------------------------

Portions of this software were developed under contract to the
U.S. Government, and are made available pursuant to the DFARS clauses
cited below.

Copyright 2012, Lockheed Martin Corporation

The U.S. Government has Unlimited Rights in this computer software
pursuant to the clause DFARS 252.227-7014 of contract No.
W91WMC-07-D-0001. Any reproduction of this computer software, or
portions thereof, marked with this legend must also reproduce these
markings.  

See LICENCE for more details.

### Compiling:

```
	$ ./configure
	$ make
	# make install
```
	This will install two libraries (libcackey.so, and libcackey_g.so) into
	"/usr/local/lib".

### Compiling for Windows:

	32-bit and 64-bit Windows libraries can be compiled on POSIX systems using the
	mingw toolchain.  The 32-bit binaries will require extracting winscard.dll and 
	winscard.lib from the Microsoft Platform SDK to 'build/cackey_win32_build/lib/'.

```
        $ sh build/cackey_win64_build/build.sh
```        
        This will create 64-bit libraries named libcackey.dll and libcackey_g.dll.  


### Usage:
	The libraries "libcackey.so" and "libcackey_g.so" are RSA PKCS#11
	Providers.  They are meant to be linked into any application that
	requires a PKCS#11 provider.

	The library "libcackey.so" is meant for general purpose use.

	The library "libcackey_g.so" is for debugging purposes.  It has
	debugging symbols compiled in and generates debugging information on
	stderr.

### Testing:

```
	$ make test
	$ ./test
	 - or -
	$ ./test 2>cackey_debug.log
```
