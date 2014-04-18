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
