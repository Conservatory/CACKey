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
