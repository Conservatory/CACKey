AC_INIT(cackey, 0.6.6) 
AC_CONFIG_HEADERS(config.h)

dnl Locate standard tools
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_AIX
AC_LANG(C)

dnl Determine this platform
DC_CHK_OS_INFO

dnl Determine how to create shared objects on this platform
DC_GET_SHOBJFLAGS

ACX_PTHREAD(, [
	AC_MSG_WARN([Can not figure out how to compile with pthreads support, run-time will likely fail.])
])

dnl Determine how to create static archives on this platform
AC_CHECK_TOOL(AR, ar)
AC_CHECK_TOOL(RANLIB, ranlib)

dnl Determine how to strip executables and remove symbols
DC_FIND_STRIP_AND_REMOVESYMS(libcackey.syms)

dnl Check for all required headers
AC_CHECK_HEADERS(arpa/inet.h inttypes.h stdarg.h stdint.h stdio.h stdlib.h string.h sys/socket.h sys/types.h sys/un.h time.h unistd.h pthread.h zlib.h limits.h,,[
	AC_WARN([Required header missing, compilation will likely fail.])
], [
#ifdef HAVE_ARPA_INET_H
#  include <arpa/inet.h>
#endif
#ifdef HAVE_INTTYPES_H
#  include <inttypes.h>
#endif
#ifdef HAVE_STDARG_H
#  include <stdarg.h>
#endif
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#endif
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#ifdef HAVE_STDLIB_H
#  include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#  include <string.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#  include <sys/socket.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#  include <sys/types.h>
#endif
#ifdef HAVE_SYS_UN_H
#  include <sys/un.h>
#endif
#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif
#ifdef HAVE_TIME_H
#  include <time.h>
#endif
#ifdef HAVE_PTHREAD_H
#  include <pthread.h>
#endif
#ifdef HAVE_LIMITS_H
#  include <limits.h>
#endif
#ifdef HAVE_ZLIB_H
#  include <zlib.h>
#endif
])

dnl Check for PC/SC headers and libraries
DC_PCSC

dnl Check for ZLIB libraries
AC_CHECK_LIB(z, uncompress)

dnl Verify that a basic program will compile
AC_MSG_CHECKING([if basic PC/SC program works])
AC_LINK_IFELSE(
	AC_LANG_PROGRAM([[
#ifdef HAVE_WINTYPES_H
#  include <wintypes.h>
#endif
#ifdef HAVE_PCSCLITE_H
#  include <pcsclite.h>
#endif
#ifdef HAVE_WINSCARD_H
#  include <winscard.h>
#endif
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#endif
#ifdef HAVE_INTTYPES_H
#  include <inttypes.h>
#endif
#ifdef HAVE_STDLIB_H
#  include <stdlib.h>
#endif
#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif
#ifdef HAVE_STRING_H
#  include <string.h>
#endif
#ifdef HAVE_PTHREAD_H
#  include <pthread.h>
#endif
#ifdef HAVE_LIMITS_H
#  include <limits.h>
#endif
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#ifdef HAVE_ZLIB_H
#  ifdef HAVE_LIBZ
#    include <zlib.h>
#  endif
#endif
	]], [[
		LPSCARDHANDLE hCard;
		SCARDCONTEXT hContext;
		DWORD dwActiveProtocol;
		LONG scard_conn_ret, scard_est_context_ret;

		scard_est_context_ret = SCardEstablishContext(SCARD_SCOPE_SYSTEM, NULL, NULL, &hContext);

		hCard = NULL;
		scard_conn_ret = SCardConnect(hContext, "Reader X", SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0, hCard, &dwActiveProtocol);
	]]), [
		AC_MSG_RESULT(okay)
	], [
		AC_MSG_RESULT(failed)

		AC_MSG_FAILURE([simple PC/SC program failed])
	]
)

dnl Option to enable DoD certs on hardware slot
AC_ARG_ENABLE(dod-certs-on-hw-slots, AC_HELP_STRING([--enable-dod-certs-on-hw-slots], [Specify that DoD certificates should be made available on hardware token slots]), [
	dodcertsonhwslots=$enableval
], [
	dodcertsonhwslots=no
])

if ! test "${dodcertsonhwslots}" = 'no'; then
	AC_DEFINE(CACKEY_CARD_SLOT_INCLUDE_EXTRA_CERTS, [1], [Specify that DoD certificates should be made available on hardware token slots])
fi

dnl Set version script, to limit the scope of symbols
DC_SETVERSIONSCRIPT(libcackey.vers, libcackey.syms)

dnl Upate LDFLAGS to include setting the run-time linker path to the same as our compile-time linker
DC_SYNC_RPATH

dnl If we updated LIBOBJS, update SHLIBOBJS -- must be last.
DC_SYNC_SHLIBOBJS

dnl Produce Makefile
AC_OUTPUT(Makefile libcackey.syms)
