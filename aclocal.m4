dnl Available from the GNU Autoconf Macro Archive at:
dnl http://www.gnu.org/software/ac-archive/htmldoc/acx_pthread.html
dnl
AC_DEFUN([ACX_PTHREAD], [
AC_REQUIRE([AC_CANONICAL_HOST])
AC_LANG_SAVE
AC_LANG_C
acx_pthread_ok=no

# We used to check for pthread.h first, but this fails if pthread.h
# requires special compiler flags (e.g. on True64 or Sequent).
# It gets checked for in the link test anyway.

# First of all, check if the user has set any of the PTHREAD_LIBS,
# etcetera environment variables, and if threads linking works using
# them:
if test x"$PTHREAD_LIBS$PTHREAD_CFLAGS" != x; then
        save_CFLAGS="$CFLAGS"
        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
        save_LIBS="$LIBS"
        LIBS="$PTHREAD_LIBS $LIBS"
        AC_MSG_CHECKING([for pthread_join in LIBS=$PTHREAD_LIBS with CFLAGS=$PTHREAD_CFLAGS])
        AC_TRY_LINK_FUNC(pthread_join, acx_pthread_ok=yes)
        AC_MSG_RESULT($acx_pthread_ok)
        if test x"$acx_pthread_ok" = xno; then
                PTHREAD_LIBS=""
                PTHREAD_CFLAGS=""
        fi
        LIBS="$save_LIBS"
        CFLAGS="$save_CFLAGS"
fi

# We must check for the threads library under a number of different
# names; the ordering is very important because some systems
# (e.g. DEC) have both -lpthread and -lpthreads, where one of the
# libraries is broken (non-POSIX).

# Create a list of thread flags to try.  Items starting with a "-" are
# C compiler flags, and other items are library names, except for "none"
# which indicates that we try without any flags at all.

acx_pthread_flags="pthreads none -Kthread -kthread lthread pthread -pthread -pthreads -mthreads --thread-safe -mt"

# The ordering *is* (sometimes) important.  Some notes on the
# individual items follow:

# pthreads: AIX (must check this before -lpthread)
# none: in case threads are in libc; should be tried before -Kthread and
#       other compiler flags to prevent continual compiler warnings
# -Kthread: Sequent (threads in libc, but -Kthread needed for pthread.h)
# -kthread: FreeBSD kernel threads (preferred to -pthread since SMP-able)
# lthread: LinuxThreads port on FreeBSD (also preferred to -pthread)
# -pthread: Linux/gcc (kernel threads), BSD/gcc (userland threads)
# -pthreads: Solaris/gcc
# -mthreads: Mingw32/gcc, Lynx/gcc
# -mt: Sun Workshop C (may only link SunOS threads [-lthread], but it
#      doesn't hurt to check since this sometimes defines pthreads too;
#      also defines -D_REENTRANT)
# pthread: Linux, etcetera
# --thread-safe: KAI C++

case "${host_cpu}-${host_os}" in
        *solaris*)

        # On Solaris (at least, for some versions), libc contains stubbed
        # (non-functional) versions of the pthreads routines, so link-based
        # tests will erroneously succeed.  (We need to link with -pthread or
        # -lpthread.)  (The stubs are missing pthread_cleanup_push, or rather
        # a function called by this macro, so we could check for that, but
        # who knows whether they'll stub that too in a future libc.)  So,
        # we'll just look for -pthreads and -lpthread first:

        acx_pthread_flags="-pthread -pthreads pthread -mt $acx_pthread_flags"
        ;;
esac

if test x"$acx_pthread_ok" = xno; then
for flag in $acx_pthread_flags; do

        case $flag in
                none)
                AC_MSG_CHECKING([whether pthreads work without any flags])
                ;;

                -*)
                AC_MSG_CHECKING([whether pthreads work with $flag])
                PTHREAD_CFLAGS="$flag"
                ;;

                *)
                AC_MSG_CHECKING([for the pthreads library -l$flag])
                PTHREAD_LIBS="-l$flag"
                ;;
        esac

        save_LIBS="$LIBS"
        save_CFLAGS="$CFLAGS"
        LIBS="$PTHREAD_LIBS $LIBS"
        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"

        # Check for various functions.  We must include pthread.h,
        # since some functions may be macros.  (On the Sequent, we
        # need a special flag -Kthread to make this header compile.)
        # We check for pthread_join because it is in -lpthread on IRIX
        # while pthread_create is in libc.  We check for pthread_attr_init
        # due to DEC craziness with -lpthreads.  We check for
        # pthread_cleanup_push because it is one of the few pthread
        # functions on Solaris that doesn't have a non-functional libc stub.
        # We try pthread_create on general principles.
        AC_TRY_LINK([#include <pthread.h>],
                    [pthread_t th; pthread_join(th, 0);
                     pthread_attr_init(0); pthread_cleanup_push(0, 0);
                     pthread_create(0,0,0,0); pthread_cleanup_pop(0); ],
                    [acx_pthread_ok=yes])

        LIBS="$save_LIBS"
        CFLAGS="$save_CFLAGS"

        AC_MSG_RESULT($acx_pthread_ok)
        if test "x$acx_pthread_ok" = xyes; then
                break;
        fi

        PTHREAD_LIBS=""
        PTHREAD_CFLAGS=""
done
fi

# Various other checks:
if test "x$acx_pthread_ok" = xyes; then
        save_LIBS="$LIBS"
        LIBS="$PTHREAD_LIBS $LIBS"
        save_CFLAGS="$CFLAGS"
        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"

        # Detect AIX lossage: threads are created detached by default
        # and the JOINABLE attribute has a nonstandard name (UNDETACHED).
        AC_MSG_CHECKING([for joinable pthread attribute])
        AC_TRY_LINK([#include <pthread.h>],
                    [int attr=PTHREAD_CREATE_JOINABLE;],
                    ok=PTHREAD_CREATE_JOINABLE, ok=unknown)
        if test x"$ok" = xunknown; then
                AC_TRY_LINK([#include <pthread.h>],
                            [int attr=PTHREAD_CREATE_UNDETACHED;],
                            ok=PTHREAD_CREATE_UNDETACHED, ok=unknown)
        fi
        if test x"$ok" != xPTHREAD_CREATE_JOINABLE; then
                AC_DEFINE(PTHREAD_CREATE_JOINABLE, $ok,
                          [Define to the necessary symbol if this constant
                           uses a non-standard name on your system.])
        fi
        AC_MSG_RESULT(${ok})
        if test x"$ok" = xunknown; then
                AC_MSG_WARN([we do not know how to create joinable pthreads])
        fi

        AC_MSG_CHECKING([if more special flags are required for pthreads])
        flag=no
        case "${host_cpu}-${host_os}" in
                *-aix* | *-freebsd*)     flag="-D_THREAD_SAFE";;
                *solaris* | *-osf* | *-hpux*) flag="-D_REENTRANT";;
        esac
        AC_MSG_RESULT(${flag})
        if test "x$flag" != xno; then
                PTHREAD_CFLAGS="$flag $PTHREAD_CFLAGS"
        fi

        LIBS="$save_LIBS"
        CFLAGS="$save_CFLAGS"

        # More AIX lossage: must compile with cc_r
        AC_CHECK_PROG(PTHREAD_CC, cc_r, cc_r, ${CC})
else
        PTHREAD_CC="$CC"
fi

AC_SUBST(PTHREAD_LIBS)
AC_SUBST(PTHREAD_CFLAGS)
AC_SUBST(PTHREAD_CC)

# Finally, execute ACTION-IF-FOUND/ACTION-IF-NOT-FOUND:
if test x"$acx_pthread_ok" = xyes; then
        ifelse([$1],,AC_DEFINE(HAVE_PTHREAD,1,[Define if you have POSIX threads libraries and header files.]),[$1])
        :
else
        acx_pthread_ok=no
        $2
fi
AC_LANG_RESTORE
])dnl ACX_PTHREAD
dnl Usage:
dnl    DC_TEST_SHOBJFLAGS(shobjflags, shobjldflags, action-if-not-found)
dnl
AC_DEFUN(DC_TEST_SHOBJFLAGS, [
  AC_SUBST(SHOBJFLAGS)
  AC_SUBST(SHOBJLDFLAGS)

  OLD_LDFLAGS="$LDFLAGS"
  SHOBJFLAGS=""

  LDFLAGS="$OLD_LDFLAGS $1 $2"

  AC_TRY_LINK([#include <stdio.h>
int unrestst(void);], [ printf("okay\n"); unrestst(); return(0); ], [ SHOBJFLAGS="$1"; SHOBJLDFLAGS="$2" ], [
    LDFLAGS="$OLD_LDFLAGS"
    $3
  ])

  LDFLAGS="$OLD_LDFLAGS"
])

AC_DEFUN(DC_GET_SHOBJFLAGS, [
  AC_SUBST(SHOBJFLAGS)
  AC_SUBST(SHOBJLDFLAGS)

  AC_MSG_CHECKING(how to create shared objects)

  if test -z "$SHOBJFLAGS" -a -z "$SHOBJLDFLAGS"; then
    DC_TEST_SHOBJFLAGS([-fPIC -DPIC], [-shared -rdynamic], [
      DC_TEST_SHOBJFLAGS([-fPIC -DPIC], [-shared], [
	DC_TEST_SHOBJFLAGS([-fPIC -DPIC], [-shared -rdynamic -mimpure-text], [
	  DC_TEST_SHOBJFLAGS([-fPIC -DPIC], [-shared -mimpure-text], [
	    DC_TEST_SHOBJFLAGS([-fPIC -DPIC], [-shared -rdynamic -Wl,-G,-z,textoff], [
	      DC_TEST_SHOBJFLAGS([-fPIC -DPIC], [-shared -Wl,-G,-z,textoff], [
		DC_TEST_SHOBJFLAGS([-fPIC -DPIC], [-shared -dynamiclib -flat_namespace -undefined suppress -bind_at_load], [
		  DC_TEST_SHOBJFLAGS([-fPIC -DPIC], [-dynamiclib -flat_namespace -undefined suppress -bind_at_load], [
		    DC_TEST_SHOBJFLAGS([-fPIC -DPIC], [-Wl,-dynamiclib -Wl,-flat_namespace -Wl,-undefined,suppress -Wl,-bind_at_load], [
		      DC_TEST_SHOBJFLAGS([-fPIC -DPIC], [-dynamiclib -flat_namespace -undefined suppress], [
		        DC_TEST_SHOBJFLAGS([-fPIC -DPIC], [-dynamiclib], [
		          AC_MSG_RESULT(cant)
		          AC_MSG_ERROR([We are unable to make shared objects.])
                        ])
		      ])
		    ])
		  ])
		])
	      ])
	    ])
	  ])
	])
      ])
    ])
  fi

  AC_MSG_RESULT($SHOBJLDFLAGS $SHOBJFLAGS)

  DC_SYNC_SHLIBOBJS
])

AC_DEFUN(DC_SYNC_SHLIBOBJS, [
  AC_SUBST(SHLIBOBJS)
  SHLIBOBJS=""
  for obj in $LIB@&t@OBJS; do
    SHLIBOBJS="$SHLIBOBJS `echo $obj | sed 's/\.o$/_shr.o/g'`"
  done
])

AC_DEFUN(DC_SYNC_RPATH, [
  OLD_LDFLAGS="$LDFLAGS"

  for tryrpath in "-Wl,-rpath" "-Wl,--rpath" "-Wl,-R"; do
    LDFLAGS="$OLD_LDFLAGS $tryrpath -Wl,/tmp"
    AC_LINK_IFELSE(AC_LANG_PROGRAM([], [ return(0); ]), [
      rpathldflags="$tryrpath"
      break
    ])
  done
  unset tryrpath

  LDFLAGS="$OLD_LDFLAGS"
  unset OLD_LDFLAGS

  ADDLDFLAGS=""
  for opt in $LDFLAGS; do
    if echo "$opt" | grep '^-L' >/dev/null; then
      rpathdir=`echo "$opt" | sed 's@^-L *@@'`
      ADDLDFLAGS="$ADDLDFLAGS $rpathldflags -Wl,$rpathdir"
    fi
  done
  unset opt rpathldflags

  LDFLAGS="$LDFLAGS $ADDLDFLAGS"

  unset ADDLDFLAGS
])

AC_DEFUN(DC_CHK_OS_INFO, [
	AC_CANONICAL_HOST
	AC_SUBST(SHOBJEXT)
	AC_SUBST(SHOBJFLAGS)
	AC_SUBST(SHOBJLDFLAGS)
	AC_SUBST(CFLAGS)
	AC_SUBST(CPPFLAGS)
	AC_SUBST(AREXT)

	AC_MSG_CHECKING(host operating system)
	AC_MSG_RESULT($host_os)

	SHOBJEXT="so"
	AREXT="a"

	case $host_os in
		darwin*)
			SHOBJEXT="dylib"
			;;
		hpux*)
			SHOBJEXT="sl"
			;;
		mingw32|mingw32msvc*)
			SHOBJEXT="dll"
			SHOBJFLAGS="-DPIC"
			CFLAGS="$CFLAGS -mms-bitfields"
			CPPFLAGS="$CPPFLAGS -mms-bitfields"
			SHOBJLDFLAGS='-shared -Wl,--dll -Wl,--enable-auto-image-base -Wl,--output-def,$[@].def,--out-implib,$[@].a'
			;;
		cygwin*)
			SHOBJEXT="dll"
			SHOBJFLAGS="-fPIC -DPIC"
			CFLAGS="$CFLAGS -mms-bitfields"
			CPPFLAGS="$CPPFLAGS -mms-bitfields"
			SHOBJLDFLAGS='-shared -Wl,--enable-auto-image-base -Wl,--output-def,$[@].def,--out-implib,$[@].a'
			;;
	esac
])
AC_DEFUN(DC_PCSC_HEADERS, [
	AC_ARG_WITH(pcsc-headers, AC_HELP_STRING([--with-pcsc-headers=<path>], [Specify a path to look for PC/SC Headers]), [
		manualheaders="${withval}"
	], [
		manualheaders="no"
	])

	if test "${manualheaders}" != "no"; then
		CFLAGS="${CFLAGS} -I${manualheaders}"
		CPPFLAGS="${CPPFLAGS} -I${manualheaders}"

		AC_CHECK_HEADER(wintypes.h, [
			AC_DEFINE(HAVE_WINTYPES_H, [1], [Define if you have the PCSC-Lite header file (you should)])
		])

		AC_CHECK_HEADER(pcsclite.h, [
			AC_DEFINE(HAVE_PCSCLITE_H, [1], [Define if you have the PCSC-Lite header file (you should)])
		])

		AC_CHECK_HEADER(winscard.h, [
			AC_DEFINE(HAVE_WINSCARD_H, [1], [Define if you have the PCSC-Lite header file (you should)])
		])
	else
		DC_PCSC_HEADERS_SEARCH
	fi
])

AC_DEFUN(DC_PCSC_HEADERS_SEARCH, [
	SAVE_CFLAGS="${CFLAGS}"
	SAVE_CPPFLAGS="${CPPFLAGS}"
	ADD_CFLAGS=""
	ADD_CPPFLAGS=""

	found_pcsclite=0
	found_winscard=0
	found_wintypes=0

	for headerpath in /usr/include /usr/local/include /usr/cac/include /Developer/SDKs/*/System/Library/Frameworks/PCSC.framework/Versions/A/Headers; do
		for subdir in smartcard PCSC pcsc pcsclite ""; do
			headerdir="${headerpath}/${subdir}"
			CFLAGS="${SAVE_CFLAGS} -I${headerdir}"
			CPPFLAGS="${SAVE_CPPFLAGS} -I${headerdir}"

			unset ac_cv_header_pcsclite_h
			unset ac_cv_header_winscard_h
			unset ac_cv_header_wintypes_h

			just_found_pcsclite=0
			just_found_winscard=0
			just_found_wintypes=0

			AC_CHECK_HEADER(wintypes.h, [
				AC_DEFINE(HAVE_WINTYPES_H, [1], [Define if you have the PCSC-Lite header file (you should)])

				found_wintypes=1
				just_found_wintypes=1
			])

			AC_CHECK_HEADER(pcsclite.h, [
				AC_DEFINE(HAVE_PCSCLITE_H, [1], [Define if you have the PCSC-Lite header file (you should)])

				found_pcsclite=1
				just_found_pcsclite=1
			])

			AC_CHECK_HEADER(winscard.h, [
				AC_DEFINE(HAVE_WINSCARD_H, [1], [Define if you have the PCSC-Lite header file (you should)])

				found_winscard=1
				just_found_winscard=1
			])


			if test "${just_found_pcsclite}" = 1 -a "${just_found_winscard}" = 1 -a "${just_found_wintypes}" = 1; then
				ADD_CFLAGS=" -I${headerdir}"
				ADD_CPPFLAGS=" -I${headerdir}"
			fi

			if test "${found_pcsclite}" = 1 -a "${found_winscard}" = 1 -a "${found_wintypes}" = 1; then
				break
			fi
		done

		if test -n "${ADD_CFLAGS}" -o -n "${ADD_CPPFLAGS}"; then
			break
		fi
	done

	CFLAGS="${SAVE_CFLAGS}${ADD_CFLAGS}"
	CPPFLAGS="${SAVE_CPPFLAGS}${ADD_CPPFLAGS}"
])

AC_DEFUN(DC_PCSC_LIBS, [
	AC_ARG_WITH(pcsc-libs, AC_HELP_STRING([--with-pcsc-libs=<libs>], [Specify PC/SC Libraries (e.g., -lpcsclite)]), [
		manuallibs="${withval}"
	], [
		manuallibs="no"
	])

	if test "${manuallibs}" != "no"; then
		LIBS="${LIBS} ${manuallibs}"
	else
		DC_PCSC_LIBS_SEARCH
	fi
])

AC_DEFUN(DC_PCSC_LIBS_SEARCH, [
	foundlib="0"

	SAVELIBS="${LIBS}"

	AC_MSG_CHECKING([for how to link to PC/SC])

	for lib in -lpcsclite -lpcsc-lite -lpcsc /Developer/SDKs/*/System/Library/Frameworks/PCSC.framework/PCSC; do
		LIBS="${SAVELIBS} ${lib}"

		AC_LINK_IFELSE(AC_LANG_PROGRAM([[
int SCardEstablishContext(void);
]], [[
	int x;

	x = SCardEstablishContext();
		]]), [
			AC_MSG_RESULT([${lib}])
			LIBS="${SAVELIBS} ${lib}"

			foundlib="1"

			break
		])
	done

	if test "${foundlib}" = "0"; then
		AC_MSG_RESULT(cant)

		AC_MSG_WARN([unable to find PCSC library, compilation will likely fail.])
	fi

	dnl Check for SCardIsValidContext, only in newer PCSC-Lite
	AC_CHECK_FUNCS(SCardIsValidContext)
])

AC_DEFUN(DC_PCSC, [
	DC_PCSC_HEADERS
	DC_PCSC_LIBS
])
AC_DEFUN(DC_SETVERSIONSCRIPT, [
	VERSIONSCRIPT="$1"
	SYMFILE="$2"

	delete_symfile='0'
	if test ! -f "${SYMFILE}"; then
		delete_symfile='1'

		echo '' > "${SYMFILE}"
	fi

	SAVE_LDFLAGS="${LDFLAGS}"

	AC_MSG_CHECKING([for how to set version script])

	for tryaddldflags in "-Wl,--version-script -Wl,${VERSIONSCRIPT}" "-Wl,-exported_symbols_list -Wl,${SYMFILE}"; do
		LDFLAGS="${SAVE_LDFLAGS} ${tryaddldflags}"
		AC_TRY_LINK([], [], [
			addldflags="${tryaddldflags}"

			break
		])
	done

	if test "${delete_symfile}" = "1"; then
		rm -f "${SYMFILE}"
	fi

	if test -n "${addldflags}"; then
		LDFLAGS="${SAVE_LDFLAGS} ${addldflags}"
		AC_MSG_RESULT($addldflags)
	else
		LDFLAGS="${SAVE_LDFLAGS}"
		AC_MSG_RESULT([don't know])
	fi
])

AC_DEFUN(DC_FIND_STRIP_AND_REMOVESYMS, [
	SYMFILE="$1"

	dnl Determine how to strip executables
	AC_CHECK_TOOLS(OBJCOPY, objcopy gobjcopy, [false])
	AC_CHECK_TOOLS(STRIP, strip gstrip, [false])

	if test "x${STRIP}" = "xfalse"; then
		STRIP="${OBJCOPY}"
	fi

	WEAKENSYMS='true'
	REMOVESYMS='true'
	SYMPREFIX=''

	case $host_os in
		darwin*)
			REMOVESYMS="${STRIP} -i -u -s ${SYMFILE}"
			SYMPREFIX="_"
			;;
		*)
			if test "x${OBJCOPY}" != "xfalse"; then
				WEAKENSYMS="${OBJCOPY} --keep-global-symbols=${SYMFILE}"
				REMOVESYMS="${OBJCOPY} --discard-all"
			elif test "x${STRIP}" != "xfalse"; then
				REMOVESYMS="${STRIP} -x"
			fi
			;;
	esac

	AC_SUBST(WEAKENSYMS)
	AC_SUBST(REMOVESYMS)
	AC_SUBST(SYMPREFIX)
])
