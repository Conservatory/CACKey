This script is intended to be used to parse the "debugging" output produced on
stderr when CACKey is compiled with debugging flags enabled.

Sample usage:
	$ ./test 2> cackey-debug.log
	Testing libcackey...
	PKCS#11 Client Version: 2.30, Library Version 0.0
	...
	Testing libcackey... DONE. Status = 0
	$ ./leakcheck/leakcheck cackey-debug.log
	Unfreed memory 0x804d010:
	    cackey_mutex_create():2017: MALLOC() = 0x804d010


(Note that the leak from cackey_mutex_create() is normal -- there's no safe way
to clean up that mutex)

This functionality exists to check for leaks in libcackey independently of
dependent libraries, and the application hosting the library.
