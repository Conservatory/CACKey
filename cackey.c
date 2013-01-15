#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

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
#else
#  ifdef HAVE_LIBZ
#    undef HAVE_LIBZ
#  endif
#endif
#ifdef CACKEY_DEBUG_SEARCH_SPEEDTEST
#  include <sys/time.h>
#endif

#define CK_PTR *
#define CK_DEFINE_FUNCTION(returnType, name) returnType name
#define CK_DECLARE_FUNCTION(returnType, name) returnType name
#define CK_DECLARE_FUNCTION_POINTER(returnType, name) returnType (* name)
#define CK_CALLBACK_FUNCTION(returnType, name) returnType (* name)
#ifndef NULL_PTR
#  define NULL_PTR 0
#endif

#include "pkcs11.h"
#include "pkcs11n.h"
#include "asn1-x509.h"
#include "sha1.h"
#include "md5.h"

/*
 * Include these source files in this translation unit so that we can bind to
 * functions and not include any symbols in the output shared object.
 */
#include "asn1-x509.c"
#include "sha1.c"
#include "md5.c"

#ifndef CACKEY_CRYPTOKI_VERSION_CODE
#  define CACKEY_CRYPTOKI_VERSION_CODE 0x021e00
#endif

/* GSC-IS v2.1 Definitions */
/** Classes **/
#define GSCIS_CLASS_ISO7816           0x00
#define GSCIS_CLASS_GLOBAL_PLATFORM   0x80

/** Instructions **/
#define GSCIS_INSTR_GET_RESPONSE      0xC0
#define GSCIS_INSTR_READ_BINARY       0xB0
#define GSCIS_INSTR_UPDATE_BINARY     0xD6
#define GSCIS_INSTR_SELECT            0xA4
#define GSCIS_INSTR_EXTERNAL_AUTH     0x82
#define GSCIS_INSTR_GET_CHALLENGE     0x84
#define GSCIS_INSTR_INTERNAL_AUTH     0x88
#define GSCIS_INSTR_VERIFY            0x20
#define GSCIS_INSTR_SIGN              0x2A
#define GSCIS_INSTR_GET_PROP          0x56
#define GSCIS_INSTR_GET_ACR           0x4C
#define GSCIS_INSTR_READ_BUFFER       0x52
#define GSCIS_INSTR_SIGNDECRYPT       0x42

#define GSCIS_PARAM_SELECT_APPLET     0x04

/** Tags **/
/*** CCC Tags ***/
#define GSCIS_TAG_CARDID              0xF0
#define GSCIS_TAG_CCC_VER             0xF1
#define GSCIS_TAG_CCG_VER             0xF2
#define GSCIS_TAG_CARDURL             0xF3
#define GSCIS_TAG_PKCS15              0xF4
#define GSCIS_TAG_REG_DATA_MODEL      0xF5
#define GSCIS_TAG_ACR_TABLE           0xF6
#define GSCIS_TAG_CARD_APDU           0xF7
#define GSCIS_TAG_REDIRECTION         0xFA
#define GSCIS_TAG_CT                  0xFB
#define GSCIS_TAG_ST                  0xFC
#define GSCIS_TAG_NEXTCCC             0xFD

/*** General - EF 2200 ***/
#define GSCIS_TAG_FNAME               0x01
#define GSCIS_TAG_MNAME               0x02
#define GSCIS_TAG_LNAME               0x03
#define GSCIS_TAG_SUFFIX              0x04
#define GSCIS_TAG_GOVT_AGENCY         0x05
#define GSCIS_TAG_BUREAU              0x06
#define GSCIS_TAG_BUREAU_CODE         0x07
#define GSCIS_TAG_DEPT_CODE           0x08
#define GSCIS_TAG_TITLE               0x09
#define GSCIS_TAG_BUILDING            0x10
#define GSCIS_TAG_OFFICE_ADDR1        0x11
#define GSCIS_TAG_OFFICE_ADDR2        0x12
#define GSCIS_TAG_OFFICE_CITY         0x13
#define GSCIS_TAG_OFFICE_STATE        0x14
#define GSCIS_TAG_OFFICE_ZIP          0x15
#define GSCIS_TAG_OFFICE_COUNTRY      0x16
#define GSCIS_TAG_OFFICE_PHONE        0x17
#define GSCIS_TAG_OFFICE_PHONE_EXT    0x18
#define GSCIS_TAG_OFFICE_FAX          0x19
#define GSCIS_TAG_OFFICE_EMAIL        0x1A
#define GSCIS_TAG_OFFICE_ROOM         0x1B
#define GSCIS_TAG_NONGOV_AGENCY       0x1C
#define GSCIS_TAG_SSN_DESIGNATOR      0x1D

/*** PII - EF 2100 ***/
#define GSCIS_TAG_SSN                 0x20
#define GSCIS_TAG_DOB                 0x21
#define GSCIS_TAG_GENDER              0x22

/*** Login Information - EF 4000 ***/
#define GSCIS_TAG_USERID              0x40
#define GSCIS_TAG_DOMAIN              0x41
#define GSCIS_TAG_PASSWORD            0x42

/*** Card Information - EF 5000 ***/
#define GSCIS_TAG_ISSUERID            0x50
#define GSCIS_TAG_SERNO               0x51
#define GSCIS_TAG_ISSUE_DATE          0x52
#define GSCIS_TAG_EXPIRE_DATE         0x53
#define GSCIS_TAG_CARD_TYPE           0x54
#define GSCIS_TAG_SECURITY_CODE       0x57
#define GSCIS_TAG_CARDID_AID          0x58

/*** PKI Information - EF 7000 ***/
#define GSCIS_TAG_CERTIFICATE         0x70
#define GSCIS_TAG_CERT_ISSUE_DATE     0x71
#define GSCIS_TAG_CERT_EXPIRE_DATE    0x72

/** Applet IDs **/
#define GSCIS_AID_CCC                 0xA0, 0x00, 0x00, 0x01, 0x16, 0xDB, 0x00

/* Maximum size of data portion of APDUs */
/** Do not set this above 250 **/
#define CACKEY_APDU_MTU               250

/* ATR If not available */
#ifndef MAX_ATR_SIZE
#define MAX_ATR_SIZE 1024
#endif

#ifdef CACKEY_DEBUG
#  ifdef HAVE_TIME_H
#    include <time.h>
static time_t cackey_debug_start_time = 0;
static unsigned long CACKEY_DEBUG_GETTIME(void) {
	if (cackey_debug_start_time == 0) {
		cackey_debug_start_time = time(NULL);
	}

	return(time(NULL) - cackey_debug_start_time);
}
#  else
static unsigned long CACKEY_DEBUG_GETTIME(void) {
	return(0);
}
#  endif

#  define CACKEY_DEBUG_PRINTF(x...) { \
	static char buf_user[4096] = {0}; \
	snprintf(buf_user, sizeof(buf_user), x); \
	buf_user[sizeof(buf_user) - 1] = '\0'; \
	fprintf(cackey_debug_fd(), "[%lu]: %s():%i: %s\n", CACKEY_DEBUG_GETTIME(), __func__, __LINE__, buf_user); \
	fflush(cackey_debug_fd()); \
}
#  define CACKEY_DEBUG_PRINTBUF(f, x, y) { \
	static char buf_user[4096] = {0}, *buf_user_p; \
	unsigned long buf_user_size; \
	unsigned char *TMPBUF; \
	unsigned long idx; \
	int snprintf_ret; \
	TMPBUF = (unsigned char *) (x); \
	buf_user_p = buf_user; \
	buf_user_size = sizeof(buf_user); \
	for (idx = 1; idx < (y); idx++) { \
		if (buf_user_size <= 0) { \
			break; \
		}; \
		snprintf_ret = snprintf(buf_user_p, buf_user_size, ", %02x", TMPBUF[idx]); \
		if (snprintf_ret <= 0) { \
			break; \
		}; \
		buf_user_p += snprintf_ret; \
		buf_user_size -= snprintf_ret; \
	}; \
	buf_user[sizeof(buf_user) - 1] = '\0'; \
	fprintf(cackey_debug_fd(), "[%lu]: %s():%i: %s  (%s/%lu = {%02x%s})\n", CACKEY_DEBUG_GETTIME(), __func__, __LINE__, f, #x, (unsigned long) (y), TMPBUF[0], buf_user); \
	fflush(cackey_debug_fd()); \
}
#  define free(x) { CACKEY_DEBUG_PRINTF("FREE(%p) (%s)", (void *) x, #x); free(x); }

static FILE *cackey_debug_fd(void) {
	static FILE *fd = NULL;
	char *logfile;

	if (fd != NULL) {
		return(fd);
	}

	/*
	 * Log to stderr initially so we can use debugging within
	 * this function without getting into an infinite loop
	 */
	fd = stderr;

	logfile = getenv("CACKEY_DEBUG_LOGFILE");
	if (logfile != NULL) {
		CACKEY_DEBUG_PRINTF("Found environment variable: %s", logfile);

		logfile = strchr(logfile, '=');
		if (logfile == NULL) {
			logfile = getenv("CACKEY_DEBUG_LOGFILE");
		} else {
			logfile++;
		}
	}

	if (logfile != NULL) {
		CACKEY_DEBUG_PRINTF("Found log file: %s", logfile);

		fd = fopen(logfile, "a");
	}

	if (fd == NULL) {
		fd = stderr;
	}

	if (fd == stderr) {
		CACKEY_DEBUG_PRINTF("Returning stderr");
	} else {
		CACKEY_DEBUG_PRINTF("Returning %p", (void *) fd);
	}

	return(fd);
}

static void *CACKEY_DEBUG_FUNC_MALLOC(size_t size, const char *func, int line) {
	void *retval;

	retval = malloc(size);

	fprintf(cackey_debug_fd(), "[%lu]: %s():%i: MALLOC() = %p\n", CACKEY_DEBUG_GETTIME(), func, line, retval);
	fflush(cackey_debug_fd());

	return(retval);
}

static void *CACKEY_DEBUG_FUNC_REALLOC(void *ptr, size_t size, const char *func, int line) {
	void *retval;

	retval = realloc(ptr, size);

	if (retval != ptr) {
		fprintf(cackey_debug_fd(), "[%lu]: %s():%i: REALLOC(%p) = %p\n", CACKEY_DEBUG_GETTIME(), func, line, ptr, retval);
		fflush(cackey_debug_fd());
	}

	if (retval == NULL) {
		CACKEY_DEBUG_PRINTF(" *** ERROR *** realloc returned NULL (size = %lu)", (unsigned long) size);
	}

	return(retval);
}

static char *CACKEY_DEBUG_FUNC_STRDUP(const char *ptr, const char *func, int line) {
	char *retval;

	retval = strdup(ptr);

	fprintf(cackey_debug_fd(), "[%lu]: %s():%i: STRDUP_MALLOC() = %p\n", CACKEY_DEBUG_GETTIME(), func, line, retval);
	fflush(cackey_debug_fd());

	return(retval);
}

static const char *CACKEY_DEBUG_FUNC_TAG_TO_STR(unsigned char tag) {
	switch (tag) {
		case GSCIS_TAG_CARDID:
			return("GSCIS_TAG_CARDID");
		case GSCIS_TAG_CCC_VER:
			return("GSCIS_TAG_CCC_VER");
		case GSCIS_TAG_CCG_VER:
			return("GSCIS_TAG_CCG_VER");
		case GSCIS_TAG_CARDURL:
			return("GSCIS_TAG_CARDURL");
		case GSCIS_TAG_PKCS15:
			return("GSCIS_TAG_PKCS15");
		case GSCIS_TAG_REG_DATA_MODEL:
			return("GSCIS_TAG_REG_DATA_MODEL");
		case GSCIS_TAG_ACR_TABLE:
			return("GSCIS_TAG_ACR_TABLE");
		case GSCIS_TAG_CARD_APDU:
			return("GSCIS_TAG_CARD_APDU");
		case GSCIS_TAG_REDIRECTION:
			return("GSCIS_TAG_REDIRECTION");
		case GSCIS_TAG_CT:
			return("GSCIS_TAG_CT");
		case GSCIS_TAG_ST:
			return("GSCIS_TAG_ST");
		case GSCIS_TAG_NEXTCCC:
			return("GSCIS_TAG_NEXTCCC");
		case GSCIS_TAG_FNAME:
			return("GSCIS_TAG_FNAME");
		case GSCIS_TAG_MNAME:
			return("GSCIS_TAG_MNAME");
		case GSCIS_TAG_LNAME:
			return("GSCIS_TAG_LNAME");
		case GSCIS_TAG_SUFFIX:
			return("GSCIS_TAG_SUFFIX");
		case GSCIS_TAG_GOVT_AGENCY:
			return("GSCIS_TAG_GOVT_AGENCY");
		case GSCIS_TAG_BUREAU:
			return("GSCIS_TAG_BUREAU");
		case GSCIS_TAG_BUREAU_CODE:
			return("GSCIS_TAG_BUREAU_CODE");
		case GSCIS_TAG_DEPT_CODE:
			return("GSCIS_TAG_DEPT_CODE");
		case GSCIS_TAG_TITLE:
			return("GSCIS_TAG_TITLE");
		case GSCIS_TAG_BUILDING:
			return("GSCIS_TAG_BUILDING");
		case GSCIS_TAG_OFFICE_ADDR1:
			return("GSCIS_TAG_OFFICE_ADDR1");
		case GSCIS_TAG_OFFICE_ADDR2:
			return("GSCIS_TAG_OFFICE_ADDR2");
		case GSCIS_TAG_OFFICE_CITY:
			return("GSCIS_TAG_OFFICE_CITY");
		case GSCIS_TAG_OFFICE_STATE:
			return("GSCIS_TAG_OFFICE_STATE");
		case GSCIS_TAG_OFFICE_ZIP:
			return("GSCIS_TAG_OFFICE_ZIP");
		case GSCIS_TAG_OFFICE_COUNTRY:
			return("GSCIS_TAG_OFFICE_COUNTRY");
		case GSCIS_TAG_OFFICE_PHONE:
			return("GSCIS_TAG_OFFICE_PHONE");
		case GSCIS_TAG_OFFICE_PHONE_EXT:
			return("GSCIS_TAG_OFFICE_PHONE_EXT");
		case GSCIS_TAG_OFFICE_FAX:
			return("GSCIS_TAG_OFFICE_FAX");
		case GSCIS_TAG_OFFICE_EMAIL:
			return("GSCIS_TAG_OFFICE_EMAIL");
		case GSCIS_TAG_OFFICE_ROOM:
			return("GSCIS_TAG_OFFICE_ROOM");
		case GSCIS_TAG_NONGOV_AGENCY:
			return("GSCIS_TAG_NONGOV_AGENCY");
		case GSCIS_TAG_SSN_DESIGNATOR:
			return("GSCIS_TAG_SSN_DESIGNATOR");
		case GSCIS_TAG_SSN:
			return("GSCIS_TAG_SSN");
		case GSCIS_TAG_DOB:
			return("GSCIS_TAG_DOB");
		case GSCIS_TAG_GENDER:
			return("GSCIS_TAG_GENDER");
		case GSCIS_TAG_USERID:
			return("GSCIS_TAG_USERID");
		case GSCIS_TAG_DOMAIN:
			return("GSCIS_TAG_DOMAIN");
		case GSCIS_TAG_PASSWORD:
			return("GSCIS_TAG_PASSWORD");
		case GSCIS_TAG_ISSUERID:
			return("GSCIS_TAG_ISSUERID");
		case GSCIS_TAG_SERNO:
			return("GSCIS_TAG_SERNO");
		case GSCIS_TAG_ISSUE_DATE:
			return("GSCIS_TAG_ISSUE_DATE");
		case GSCIS_TAG_EXPIRE_DATE:
			return("GSCIS_TAG_EXPIRE_DATE");
		case GSCIS_TAG_CARD_TYPE:
			return("GSCIS_TAG_CARD_TYPE");
		case GSCIS_TAG_SECURITY_CODE:
			return("GSCIS_TAG_SECURITY_CODE");
		case GSCIS_TAG_CARDID_AID:
			return("GSCIS_TAG_CARDID_AID");
		case GSCIS_TAG_CERTIFICATE:
			return("GSCIS_TAG_CERTIFICATE");
		case GSCIS_TAG_CERT_ISSUE_DATE:
			return("GSCIS_TAG_CERT_ISSUE_DATE");
		case GSCIS_TAG_CERT_EXPIRE_DATE:
			return("GSCIS_TAG_CERT_EXPIRE_DATE");
	}

	return("UNKNOWN");
}

static const char *CACKEY_DEBUG_FUNC_SCARDERR_TO_STR(LONG retcode) {
	switch (retcode) {
		case SCARD_S_SUCCESS:
			return("SCARD_S_SUCCESS");
		case SCARD_E_CANCELLED:
			return("SCARD_E_CANCELLED");
		case SCARD_E_CANT_DISPOSE:
			return("SCARD_E_CANT_DISPOSE");
		case SCARD_E_INSUFFICIENT_BUFFER:
			return("SCARD_E_INSUFFICIENT_BUFFER");
		case SCARD_E_INVALID_ATR:
			return("SCARD_E_INVALID_ATR");
		case SCARD_E_INVALID_HANDLE:
			return("SCARD_E_INVALID_HANDLE");
		case SCARD_E_INVALID_PARAMETER:
			return("SCARD_E_INVALID_PARAMETER");
		case SCARD_E_INVALID_TARGET:
			return("SCARD_E_INVALID_TARGET");
		case SCARD_E_INVALID_VALUE:
			return("SCARD_E_INVALID_VALUE");
		case SCARD_E_NO_MEMORY:
			return("SCARD_E_NO_MEMORY");
		case SCARD_E_UNKNOWN_READER:
			return("SCARD_E_UNKNOWN_READER");
		case SCARD_E_TIMEOUT:
			return("SCARD_E_TIMEOUT");
		case SCARD_E_SHARING_VIOLATION:
			return("SCARD_E_SHARING_VIOLATION");
		case SCARD_E_NO_SMARTCARD:
			return("SCARD_E_NO_SMARTCARD");
		case SCARD_E_UNKNOWN_CARD:
			return("SCARD_E_UNKNOWN_CARD");
		case SCARD_E_PROTO_MISMATCH:
			return("SCARD_E_PROTO_MISMATCH");
		case SCARD_E_NOT_READY:
			return("SCARD_E_NOT_READY");
		case SCARD_E_SYSTEM_CANCELLED:
			return("SCARD_E_SYSTEM_CANCELLED");
		case SCARD_E_NOT_TRANSACTED:
			return("SCARD_E_NOT_TRANSACTED");
		case SCARD_E_READER_UNAVAILABLE:
			return("SCARD_E_READER_UNAVAILABLE");
		case SCARD_W_UNSUPPORTED_CARD:
			return("SCARD_W_UNSUPPORTED_CARD");
		case SCARD_W_UNRESPONSIVE_CARD:
			return("SCARD_W_UNRESPONSIVE_CARD");
		case SCARD_W_UNPOWERED_CARD:
			return("SCARD_W_UNPOWERED_CARD");
		case SCARD_W_RESET_CARD:
			return("SCARD_W_RESET_CARD");
		case SCARD_W_REMOVED_CARD:
			return("SCARD_W_REMOVED_CARD");
		case SCARD_E_PCI_TOO_SMALL:
			return("SCARD_E_PCI_TOO_SMALL");
		case SCARD_E_READER_UNSUPPORTED:
			return("SCARD_E_READER_UNSUPPORTED");
		case SCARD_E_DUPLICATE_READER:
			return("SCARD_E_DUPLICATE_READER");
		case SCARD_E_CARD_UNSUPPORTED:
			return("SCARD_E_CARD_UNSUPPORTED");
		case SCARD_E_NO_SERVICE:
			return("SCARD_E_NO_SERVICE");
		case SCARD_E_SERVICE_STOPPED:
			return("SCARD_E_SERVICE_STOPPED");
		case SCARD_E_UNSUPPORTED_FEATURE:
			return("SCARD_E_UNSUPPORTED_FEATURE");
#ifdef SCARD_W_INSERTED_CARD
		case SCARD_W_INSERTED_CARD:
			return("SCARD_W_INSERTED_CARD");
#endif
#ifdef SCARD_E_NO_READERS_AVAILABLE
		case SCARD_E_NO_READERS_AVAILABLE:
			return("SCARD_E_NO_READERS_AVAILABLE");
#endif
	}

	return("UNKNOWN");
}

static const char *CACKEY_DEBUG_FUNC_OBJID_TO_STR(uint16_t objid) {
	switch (objid) {
		case 0x2000:
			return("CACKEY_TLV_OBJID_GENERALINFO");
		case 0x2100:
			return("CACKEY_TLV_OBJID_PROPERSONALINFO");
		case 0x3000:
			return("CACKEY_TLV_OBJID_ACCESSCONTROL");
		case 0x4000:
			return("CACKEY_TLV_OBJID_LOGIN");
		case 0x5000:
			return("CACKEY_TLV_OBJID_CARDINFO");
		case 0x6000:
			return("CACKEY_TLV_OBJID_BIOMETRICS");
		case 0x7000:
			return("CACKEY_TLV_OBJID_DIGITALSIGCERT");
		case 0x0200:
			return("CACKEY_TLV_OBJID_CAC_PERSON");
		case 0x0202:
			return("CACKEY_TLV_OBJID_CAC_BENEFITS");
		case 0x0203:
			return("CACKEY_TLV_OBJID_CAC_OTHERBENEFITS");
		case 0x0201:
			return("CACKEY_TLV_OBJID_CAC_PERSONNEL");
		case 0x02FE:
			return("CACKEY_TLV_OBJID_CAC_PKICERT");
	}
	
	return("UNKNOWN");
}

static const char *CACKEY_DEBUG_FUNC_APPTYPE_TO_STR(uint8_t apptype) {
	switch (apptype) {
		case 0x00:
			return("NONE");
		case 0x01:
			return("CACKEY_TLV_APP_GENERIC");
		case 0x02:
			return("CACKEY_TLV_APP_SKI");
		case 0x03:
			return("CACKEY_TLV_APP_GENERIC | CACKEY_TLV_APP_SKI");
		case 0x04:
			return("CACKEY_TLV_APP_PKI");
		case 0x05:
			return("CACKEY_TLV_APP_GENERIC | CACKEY_TLV_APP_PKI");
		case 0x06:
			return("CACKEY_TLV_APP_SKI | CACKEY_TLV_APP_PKI");
		case 0x07:
			return("CACKEY_TLV_APP_GENERIC | CACKEY_TLV_APP_SKI | CACKEY_TLV_APP_PKI");
	}

	return("INVALID");
}

static const char *CACKEY_DEBUG_FUNC_ATTRIBUTE_TO_STR(CK_ATTRIBUTE_TYPE attr) {
	switch (attr) {
		case CKA_CLASS:
			return("CKA_CLASS");
		case CKA_TOKEN:
			return("CKA_TOKEN");
		case CKA_PRIVATE:
			return("CKA_PRIVATE");
		case CKA_LABEL:
			return("CKA_LABEL");
		case CKA_APPLICATION:
			return("CKA_APPLICATION");
		case CKA_VALUE:
			return("CKA_VALUE");
		case CKA_OBJECT_ID:
			return("CKA_OBJECT_ID");
		case CKA_CERTIFICATE_TYPE:
			return("CKA_CERTIFICATE_TYPE");
		case CKA_ISSUER:
			return("CKA_ISSUER");
		case CKA_SERIAL_NUMBER:
			return("CKA_SERIAL_NUMBER");
		case CKA_AC_ISSUER:
			return("CKA_AC_ISSUER");
		case CKA_OWNER:
			return("CKA_OWNER");
		case CKA_ATTR_TYPES:
			return("CKA_ATTR_TYPES");
		case CKA_TRUSTED:
			return("CKA_TRUSTED");
		case CKA_KEY_TYPE:
			return("CKA_KEY_TYPE");
		case CKA_SUBJECT:
			return("CKA_SUBJECT");
		case CKA_ID:
			return("CKA_ID");
		case CKA_SENSITIVE:
			return("CKA_SENSITIVE");
		case CKA_ENCRYPT:
			return("CKA_ENCRYPT");
		case CKA_DECRYPT:
			return("CKA_DECRYPT");
		case CKA_WRAP:
			return("CKA_WRAP");
		case CKA_UNWRAP:
			return("CKA_UNWRAP");
		case CKA_SIGN:
			return("CKA_SIGN");
		case CKA_SIGN_RECOVER:
			return("CKA_SIGN_RECOVER");
		case CKA_VERIFY:
			return("CKA_VERIFY");
		case CKA_VERIFY_RECOVER:
			return("CKA_VERIFY_RECOVER");
		case CKA_DERIVE:
			return("CKA_DERIVE");
		case CKA_START_DATE:
			return("CKA_START_DATE");
		case CKA_END_DATE:
			return("CKA_END_DATE");
		case CKA_MODULUS:
			return("CKA_MODULUS");
		case CKA_MODULUS_BITS:
			return("CKA_MODULUS_BITS");
		case CKA_PUBLIC_EXPONENT:
			return("CKA_PUBLIC_EXPONENT");
		case CKA_PRIVATE_EXPONENT:
			return("CKA_PRIVATE_EXPONENT");
		case CKA_PRIME_1:
			return("CKA_PRIME_1");
		case CKA_PRIME_2:
			return("CKA_PRIME_2");
		case CKA_EXPONENT_1:
			return("CKA_EXPONENT_1");
		case CKA_EXPONENT_2:
			return("CKA_EXPONENT_2");
		case CKA_COEFFICIENT:
			return("CKA_COEFFICIENT");
		case CKA_PRIME:
			return("CKA_PRIME");
		case CKA_SUBPRIME:
			return("CKA_SUBPRIME");
		case CKA_BASE:
			return("CKA_BASE");
		case CKA_PRIME_BITS:
			return("CKA_PRIME_BITS");
		case CKA_SUB_PRIME_BITS:
			return("CKA_SUB_PRIME_BITS");
		case CKA_VALUE_BITS:
			return("CKA_VALUE_BITS");
		case CKA_VALUE_LEN:
			return("CKA_VALUE_LEN");
		case CKA_EXTRACTABLE:
			return("CKA_EXTRACTABLE");
		case CKA_LOCAL:
			return("CKA_LOCAL");
		case CKA_NEVER_EXTRACTABLE:
			return("CKA_NEVER_EXTRACTABLE");
		case CKA_ALWAYS_SENSITIVE:
			return("CKA_ALWAYS_SENSITIVE");
		case CKA_KEY_GEN_MECHANISM:
			return("CKA_KEY_GEN_MECHANISM");
		case CKA_MODIFIABLE:
			return("CKA_MODIFIABLE");
		case CKA_ECDSA_PARAMS:
			return("CKA_ECDSA_PARAMS");
		case CKA_EC_POINT:
			return("CKA_EC_POINT");
		case CKA_SECONDARY_AUTH:
			return("CKA_SECONDARY_AUTH");
		case CKA_AUTH_PIN_FLAGS:
			return("CKA_AUTH_PIN_FLAGS");
		case CKA_HW_FEATURE_TYPE:
			return("CKA_HW_FEATURE_TYPE");
		case CKA_RESET_ON_INIT:
			return("CKA_RESET_ON_INIT");
		case CKA_HAS_RESET:
			return("CKA_HAS_RESET");
		case CKA_VENDOR_DEFINED:
			return("CKA_VENDOR_DEFINED");
	}

	return("UNKNOWN");
}

#  define malloc(x) CACKEY_DEBUG_FUNC_MALLOC(x, __func__, __LINE__)
#  define realloc(x, y) CACKEY_DEBUG_FUNC_REALLOC(x, y, __func__, __LINE__)
#  ifdef strdup
#    undef strdup
#  endif
#  define strdup(x) CACKEY_DEBUG_FUNC_STRDUP(x, __func__, __LINE__)
#else
#  define CACKEY_DEBUG_PRINTF(x...) /**/
#  define CACKEY_DEBUG_PRINTBUF(f, x, y) /**/
#  define CACKEY_DEBUG_FUNC_TAG_TO_STR(x) "DEBUG_DISABLED"
#  define CACKEY_DEBUG_FUNC_SCARDERR_TO_STR(x) "DEBUG_DISABLED"
#  define CACKEY_DEBUG_FUNC_OBJID_TO_STR(x) "DEBUG_DISABLED"
#  define CACKEY_DEBUG_FUNC_APPTYPE_TO_STR(x) "DEBUG_DISABLED"
#  define CACKEY_DEBUG_FUNC_ATTRIBUTE_TO_STR(x) "DEBUG_DISABLED"
#endif

struct cackey_pcsc_identity {
	unsigned char applet[7];
	uint16_t file;

	size_t certificate_len;
	unsigned char *certificate;

	ssize_t keysize;
};

struct cackey_identity {
	struct cackey_pcsc_identity *pcsc_identity;

	CK_ATTRIBUTE *attributes;
	CK_ULONG attributes_count;
};

struct cackey_session {
	int active;

	CK_SLOT_ID slotID;

	CK_STATE state;
	CK_FLAGS flags;
	CK_ULONG ulDeviceError;
	CK_VOID_PTR pApplication;
	CK_NOTIFY Notify;

	struct cackey_identity *identities;
	unsigned long identities_count;

	int search_active;
	CK_ATTRIBUTE_PTR search_query;
	CK_ULONG search_query_count;
	unsigned long search_curr_id;

	int sign_active;
	CK_MECHANISM_TYPE sign_mechanism;
	CK_BYTE_PTR sign_buf;
	unsigned long sign_buflen;
	unsigned long sign_bufused;
	struct cackey_identity *sign_identity;

	int decrypt_active;
	CK_MECHANISM_TYPE decrypt_mechanism;
	CK_VOID_PTR decrypt_mech_parm;
	CK_ULONG decrypt_mech_parmlen;
	struct cackey_identity *decrypt_identity;
};

struct cackey_slot {
	int active;
	int internal;

	char *pcsc_reader;

	int pcsc_card_connected;
	SCARDHANDLE pcsc_card;

	int transaction_depth;
	int transaction_need_hw_lock;

	int slot_reset;

	CK_FLAGS token_flags;

	unsigned char *label;

	DWORD protocol;

	unsigned int cached_certs_count;
	struct cackey_pcsc_identity *cached_certs;
};

typedef enum {
	CACKEY_TLV_APP_GENERIC = 0x01,
	CACKEY_TLV_APP_SKI     = 0x02,
	CACKEY_TLV_APP_PKI     = 0x04
} cackey_tlv_apptype;

typedef enum {
	CACKEY_TLV_OBJID_GENERALINFO       = 0x2000,
	CACKEY_TLV_OBJID_PROPERSONALINFO   = 0x2100,
	CACKEY_TLV_OBJID_ACCESSCONTROL     = 0x3000,
	CACKEY_TLV_OBJID_LOGIN             = 0x4000,
	CACKEY_TLV_OBJID_CARDINFO          = 0x5000,
	CACKEY_TLV_OBJID_BIOMETRICS        = 0x6000,
	CACKEY_TLV_OBJID_DIGITALSIGCERT    = 0x7000,
	CACKEY_TLV_OBJID_CAC_PERSON        = 0x0200,
	CACKEY_TLV_OBJID_CAC_BENEFITS      = 0x0202,
	CACKEY_TLV_OBJID_CAC_OTHERBENEFITS = 0x0203,
	CACKEY_TLV_OBJID_CAC_PERSONNEL     = 0x0201,
	CACKEY_TLV_OBJID_CAC_PKICERT       = 0x02FE
} cackey_tlv_objectid;

typedef enum {
	CACKEY_PCSC_S_TOKENPRESENT    = 1,
	CACKEY_PCSC_S_OK              = 0,
	CACKEY_PCSC_E_GENERIC         = -1,
	CACKEY_PCSC_E_BADPIN          = -2,
	CACKEY_PCSC_E_LOCKED          = -3,
	CACKEY_PCSC_E_NEEDLOGIN       = -4,
	CACKEY_PCSC_E_TOKENABSENT     = -6,
	CACKEY_PCSC_E_RETRY           = -7
} cackey_ret;

struct cackey_tlv_cardurl {
	unsigned char        rid[5];
	cackey_tlv_apptype   apptype;
	cackey_tlv_objectid  objectid;
	cackey_tlv_objectid  appid;
	unsigned char        pinid;
};

struct cackey_tlv_entity;
struct cackey_tlv_entity {
	uint8_t tag;
	size_t length;

	union {
		void *value;
		struct cackey_tlv_cardurl *value_cardurl;
		uint8_t value_byte;
	};

	struct cackey_tlv_entity *_next;
};

/* CACKEY Global Handles */
static void *cackey_biglock = NULL;
static struct cackey_session cackey_sessions[128];
static struct cackey_slot cackey_slots[128];
static int cackey_initialized = 0;
static int cackey_biglock_init = 0;
CK_C_INITIALIZE_ARGS cackey_args;

/** Extra certificates to include in token **/
struct cackey_pcsc_identity extra_certs[] = {
#include "cackey_builtin_certs.h"
};

/* PCSC Global Handles */
static LPSCARDCONTEXT cackey_pcsc_handle = NULL;

static unsigned long cackey_getversion(void) {
	static unsigned long retval = 255;
	unsigned long major = 0;
	unsigned long minor = 0;
	char *major_str = NULL;
	char *minor_str = NULL;

	CACKEY_DEBUG_PRINTF("Called.");

	if (retval != 255) {
		CACKEY_DEBUG_PRINTF("Returning 0x%lx (cached).", retval);

		return(retval);
	}

	retval = 0;

#ifdef PACKAGE_VERSION
        major_str = PACKAGE_VERSION;
	if (major_str) {
	        major = strtoul(major_str, &minor_str, 10);

		if (minor_str) {
			minor = strtoul(minor_str + 1, NULL, 10);
		}
	}

	retval = (major << 16) | (minor << 8);
#endif

	CACKEY_DEBUG_PRINTF("Returning 0x%lx", retval);

	return(retval);
}

/* PC/SC Related Functions */
/*
 * SYNPOSIS
 *     void cackey_slots_disconnect_all(void);
 *
 * ARGUMENTS
 *     None
 *
 * RETURN VALUE
 *     None
 *
 * NOTES
 *     This function disconnects from all cards.
 *
 */
static void cackey_slots_disconnect_all(void) {
	uint32_t idx;

	CACKEY_DEBUG_PRINTF("Called.");

	for (idx = 0; idx < (sizeof(cackey_slots) / sizeof(cackey_slots[0])); idx++) {
		if (cackey_slots[idx].internal) {
			/* Skip internal slots */
			continue;
		}

		if (cackey_slots[idx].pcsc_card_connected) {
			CACKEY_DEBUG_PRINTF("SCardDisconnect(%lu) called", (unsigned long) idx);

			SCardDisconnect(cackey_slots[idx].pcsc_card, SCARD_LEAVE_CARD);
		}

		if (cackey_slots[idx].label) {
			free(cackey_slots[idx].label);

			cackey_slots[idx].label = NULL;
		}

		cackey_slots[idx].pcsc_card_connected = 0;
		cackey_slots[idx].transaction_depth = 0;
		cackey_slots[idx].transaction_need_hw_lock = 0;

		if (cackey_slots[idx].active) {
			CACKEY_DEBUG_PRINTF("Marking active slot %lu as being reset", (unsigned long) idx);
		}

		cackey_slots[idx].slot_reset = 1;
	}

	CACKEY_DEBUG_PRINTF("Returning");

	return;
}

/*
 * SYNPOSIS
 *     cackey_ret cackey_pcsc_connect(void);
 *
 * ARGUMENTS
 *     None
 *
 * RETURN VALUE
 *     CACKEY_PCSC_S_OK         On success
 *     CACKEY_PCSC_E_GENERIC    On error
 *
 * NOTES
 *     This function connects to the PC/SC Connection Manager and updates the
 *     global handle.
 *
 */
static cackey_ret cackey_pcsc_connect(void) {
	LONG scard_est_context_ret;
#ifdef HAVE_SCARDISVALIDCONTEXT
	LONG scard_isvalid_ret;
#endif

	CACKEY_DEBUG_PRINTF("Called.");

	if (cackey_pcsc_handle == NULL) {
		cackey_pcsc_handle = malloc(sizeof(*cackey_pcsc_handle));
		if (cackey_pcsc_handle == NULL) {
			CACKEY_DEBUG_PRINTF("Call to malloc() failed, returning in failure");

			cackey_slots_disconnect_all();

			return(CACKEY_PCSC_E_GENERIC);
		}

		CACKEY_DEBUG_PRINTF("SCardEstablishContext() called");
		scard_est_context_ret = SCardEstablishContext(SCARD_SCOPE_SYSTEM, NULL, NULL, cackey_pcsc_handle);
		if (scard_est_context_ret != SCARD_S_SUCCESS) {
			CACKEY_DEBUG_PRINTF("Call to SCardEstablishContext failed (returned %s/%li), returning in failure", CACKEY_DEBUG_FUNC_SCARDERR_TO_STR(scard_est_context_ret), (long) scard_est_context_ret);

			free(cackey_pcsc_handle);
			cackey_pcsc_handle = NULL;

			cackey_slots_disconnect_all();

			return(CACKEY_PCSC_E_GENERIC);
		}
	}

#ifdef HAVE_SCARDISVALIDCONTEXT
	CACKEY_DEBUG_PRINTF("SCardIsValidContext() called");
	scard_isvalid_ret = SCardIsValidContext(*cackey_pcsc_handle);
	if (scard_isvalid_ret != SCARD_S_SUCCESS) {
		CACKEY_DEBUG_PRINTF("Handle has become invalid (SCardIsValidContext = %s/%li), trying to re-establish...", CACKEY_DEBUG_FUNC_SCARDERR_TO_STR(scard_isvalid_ret), (long) scard_isvalid_ret);

		CACKEY_DEBUG_PRINTF("SCardEstablishContext() called");
		scard_est_context_ret = SCardEstablishContext(SCARD_SCOPE_SYSTEM, NULL, NULL, cackey_pcsc_handle);
		if (scard_est_context_ret != SCARD_S_SUCCESS) {
			CACKEY_DEBUG_PRINTF("Call to SCardEstablishContext failed (returned %s/%li), returning in failure", CACKEY_DEBUG_FUNC_SCARDERR_TO_STR(scard_est_context_ret), (long) scard_est_context_ret);

			free(cackey_pcsc_handle);
			cackey_pcsc_handle = NULL;

			cackey_slots_disconnect_all();

			return(CACKEY_PCSC_E_GENERIC);
		}

		CACKEY_DEBUG_PRINTF("Handle has been re-established");
	}
#endif

	CACKEY_DEBUG_PRINTF("Sucessfully connected to PC/SC, returning in success");

	return(CACKEY_PCSC_S_OK);
}

/*
 * SYNPOSIS
 *     cackey_ret cackey_pcsc_disconnect(void);
 *
 * ARGUMENTS
 *     None
 *
 * RETURN VALUE
 *     CACKEY_PCSC_S_OK         On success
 *     CACKEY_PCSC_E_GENERIC    On error
 *
 * NOTES
 *     This function disconnects from the PC/SC Connection manager and updates
 *     the global handle.
 *
 */
static cackey_ret cackey_pcsc_disconnect(void) {
	LONG scard_rel_context_ret;

	CACKEY_DEBUG_PRINTF("Called.");

	if (cackey_pcsc_handle == NULL) {
		return(CACKEY_PCSC_S_OK);
	}

	scard_rel_context_ret = SCardReleaseContext(*cackey_pcsc_handle);

	if (cackey_pcsc_handle) {
		free(cackey_pcsc_handle);
	
		cackey_pcsc_handle = NULL;
	}

	if (scard_rel_context_ret != SCARD_S_SUCCESS) {
		return(CACKEY_PCSC_E_GENERIC);
	}

	return(CACKEY_PCSC_S_OK);
}

/*
 * SYNPOSIS
 *     void cackey_mark_slot_reset(struct cackey_slot *slot);
 *
 * ARGUMENTS
 *     None
 *
 * RETURN VALUE
 *     None
 *
 * NOTES
 *     This function marks a slot has having been reset, to later be cleaned up.
 *     Cleanup only happens when a PKCS#11 client calls C_FindObjectsInit.
 *
 */
static void cackey_mark_slot_reset(struct cackey_slot *slot) {
	if (slot == NULL) {
		return;
	}

	CACKEY_DEBUG_PRINTF("Called.");

	if (slot->pcsc_card_connected) {
		SCardDisconnect(slot->pcsc_card, SCARD_LEAVE_CARD);
	}

	slot->slot_reset = 1;
	slot->pcsc_card_connected = 0;
	slot->token_flags = CKF_LOGIN_REQUIRED;

	CACKEY_DEBUG_PRINTF("Returning.");

	return;
}

/*
 * SYNPOSIS
 *     LONG cackey_reconnect_card(struct cackey_slot *slot, DWORD default_protocol, LPDWORD selected_protocol);
 *
 * ARGUMENTS
 *     cackey_slot *slot
 *         Slot to send commands to
 *
 *     DWORD default_protocol
 *         Protocol to attempt first
 *
 *     LPDWORD selected_protocol
 *         [OUT] Protocol selected
 *
 * RETURN VALUE
 *     The return value from SCardReconnect()
 *
 * NOTES
 *     This function is a wrapper around SCardReconnect()
 *
 *     The SCardReconnect() function call will be called first with the
 *     dwPreferredProtocols of "default_protocol".  If that call returns
 *     SCARD_E_PROTO_MISMATCH try again with a protocol of T=0, and failing
 *     that T=1.
 *
 */
static LONG cackey_reconnect_card(struct cackey_slot *slot, DWORD default_protocol, LPDWORD selected_protocol) {
	LONG scard_conn_ret;

	scard_conn_ret = SCardReconnect(slot->pcsc_card, SCARD_SHARE_SHARED, default_protocol, SCARD_RESET_CARD, selected_protocol);

	if (scard_conn_ret == SCARD_E_PROTO_MISMATCH) {
		CACKEY_DEBUG_PRINTF("SCardReconnect() returned SCARD_E_PROTO_MISMATCH, trying with just T=0")
		scard_conn_ret = SCardReconnect(slot->pcsc_card, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0, SCARD_RESET_CARD, selected_protocol);

		if (scard_conn_ret == SCARD_E_PROTO_MISMATCH) {
			CACKEY_DEBUG_PRINTF("SCardReconnect() returned SCARD_E_PROTO_MISMATCH, trying with just T=1")
			scard_conn_ret = SCardReconnect(slot->pcsc_card, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T1, SCARD_RESET_CARD, selected_protocol);
		}
	}

	return(scard_conn_ret);
}

/*
 * SYNPOSIS
 *     cackey_ret cackey_connect_card(struct cackey_slot *slot);
 *
 * ARGUMENTS
 *     cackey_slot *slot
 *         Slot to send commands to
 *
 * RETURN VALUE
 *     CACKEY_PCSC_S_OK         On success
 *     CACKEY_PCSC_E_GENERIC    On error
 *
 * NOTES
 *     None
 *
 */
static cackey_ret cackey_connect_card(struct cackey_slot *slot) {
	cackey_ret pcsc_connect_ret;
	DWORD protocol;
	LONG scard_conn_ret;

	CACKEY_DEBUG_PRINTF("Called.");

	if (!slot) {
		CACKEY_DEBUG_PRINTF("Invalid slot specified, returning in failure");

		return(CACKEY_PCSC_E_GENERIC);
	}

	pcsc_connect_ret = cackey_pcsc_connect();
	if (pcsc_connect_ret != CACKEY_PCSC_S_OK) {
		CACKEY_DEBUG_PRINTF("Connection to PC/SC failed, returning in failure");

		return(CACKEY_PCSC_E_GENERIC);
	}

	/* Connect to reader, if needed */
	if (!slot->pcsc_card_connected) {
		CACKEY_DEBUG_PRINTF("SCardConnect(%s) called", slot->pcsc_reader);
		scard_conn_ret = SCardConnect(*cackey_pcsc_handle, slot->pcsc_reader, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &slot->pcsc_card, &protocol);

		if (scard_conn_ret == SCARD_E_PROTO_MISMATCH) {
			CACKEY_DEBUG_PRINTF("SCardConnect() returned SCARD_E_PROTO_MISMATCH, trying with just T=0")
			scard_conn_ret = SCardConnect(*cackey_pcsc_handle, slot->pcsc_reader, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0, &slot->pcsc_card, &protocol);

			if (scard_conn_ret == SCARD_E_PROTO_MISMATCH) {
				CACKEY_DEBUG_PRINTF("SCardConnect() returned SCARD_E_PROTO_MISMATCH, trying with just T=1")
				scard_conn_ret = SCardConnect(*cackey_pcsc_handle, slot->pcsc_reader, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T1, &slot->pcsc_card, &protocol);
			}
		}

		if (scard_conn_ret == SCARD_W_UNPOWERED_CARD) {
			CACKEY_DEBUG_PRINTF("SCardConnect() returned SCARD_W_UNPOWERED_CARD, trying to re-connect...");

			scard_conn_ret = SCardConnect(*cackey_pcsc_handle, slot->pcsc_reader, SCARD_SHARE_DIRECT, SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &slot->pcsc_card, &protocol);

			if (scard_conn_ret == SCARD_E_PROTO_MISMATCH) {
				CACKEY_DEBUG_PRINTF("SCardConnect() returned SCARD_E_PROTO_MISMATCH, trying with just T=0")
				scard_conn_ret = SCardConnect(*cackey_pcsc_handle, slot->pcsc_reader, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0, &slot->pcsc_card, &protocol);

				if (scard_conn_ret == SCARD_E_PROTO_MISMATCH) {
					CACKEY_DEBUG_PRINTF("SCardConnect() returned SCARD_E_PROTO_MISMATCH, trying with just T=1")
					scard_conn_ret = SCardConnect(*cackey_pcsc_handle, slot->pcsc_reader, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T1, &slot->pcsc_card, &protocol);
				}
			}

			scard_conn_ret = cackey_reconnect_card(slot, protocol, &protocol);
		}

		if (scard_conn_ret != SCARD_S_SUCCESS) {
			CACKEY_DEBUG_PRINTF("Connection to card failed, returning in failure (SCardConnect() = %s/%li)", CACKEY_DEBUG_FUNC_SCARDERR_TO_STR(scard_conn_ret), (long) scard_conn_ret);

			return(CACKEY_PCSC_E_GENERIC);
		}

		slot->pcsc_card_connected = 1;
		slot->transaction_depth = 0;
		slot->transaction_need_hw_lock = 0;
		slot->protocol = protocol;
	}

	return(CACKEY_PCSC_S_OK);
}

/*
 * SYNPOSIS
 *     cackey_ret cackey_begin_transaction(struct cackey_slot *slot);
 *
 * ARGUMENTS
 *     cackey_slot *slot
 *         Slot to send commands to
 *
 * RETURN VALUE
 *     CACKEY_PCSC_S_OK         On success
 *     CACKEY_PCSC_E_GENERIC    On error
 *
 * NOTES
 *     The transaction should be terminated using "cackey_end_transaction"
 *
 */
static cackey_ret cackey_begin_transaction(struct cackey_slot *slot) {
	cackey_ret cackey_conn_ret;
	LONG scard_trans_ret;

	CACKEY_DEBUG_PRINTF("Called.");

	cackey_conn_ret = cackey_connect_card(slot);
	if (cackey_conn_ret != CACKEY_PCSC_S_OK) {
		CACKEY_DEBUG_PRINTF("Unable to connect to card, returning in error");

		return(CACKEY_PCSC_E_GENERIC);
	}

	slot->transaction_depth++;

	if (slot->transaction_depth > 1 && !slot->transaction_need_hw_lock) {
		CACKEY_DEBUG_PRINTF("Already in a transaction, performing no action (new depth = %i)", slot->transaction_depth);

		return(CACKEY_PCSC_S_OK);
	}

	slot->transaction_need_hw_lock = 0;

	scard_trans_ret = SCardBeginTransaction(slot->pcsc_card);
	if (scard_trans_ret != SCARD_S_SUCCESS) {
		CACKEY_DEBUG_PRINTF("Unable to begin transaction, returning in error");

		return(CACKEY_PCSC_E_GENERIC);
	}

	CACKEY_DEBUG_PRINTF("Sucessfully began transaction on slot (%s)", slot->pcsc_reader);

	return(CACKEY_PCSC_S_OK);
}

/*
 * SYNPOSIS
 *     cackey_ret cackey_end_transaction(struct cackey_slot *slot);
 *
 * ARGUMENTS
 *     cackey_slot *slot
 *         Slot to send commands to
 *
 * RETURN VALUE
 *     CACKEY_PCSC_S_OK         On success
 *     CACKEY_PCSC_E_GENERIC    On error
 *
 * NOTES
 *     This function requires "cackey_begin_transaction" to be called first
 *
 */
static cackey_ret cackey_end_transaction(struct cackey_slot *slot) {
	LONG scard_trans_ret;

	CACKEY_DEBUG_PRINTF("Called.");

	if (!slot->pcsc_card_connected) {
		CACKEY_DEBUG_PRINTF("Card is not connected, unable to end transaction on card");

		if (slot->transaction_depth > 0) {
			CACKEY_DEBUG_PRINTF("Decreasing transaction depth and asking for a hardware lock on the next begin transaction (current depth = %i)", slot->transaction_depth);

			slot->transaction_depth--;

			if (slot->transaction_depth > 0) {
				slot->transaction_need_hw_lock = 1;
			}
		}

		return(CACKEY_PCSC_E_GENERIC);
	}

	if (slot->transaction_depth == 0) {
		CACKEY_DEBUG_PRINTF("Terminating a transaction that has not begun!");

		return(CACKEY_PCSC_E_GENERIC);
	}

	slot->transaction_depth--;

	if (slot->transaction_depth > 0) {
		CACKEY_DEBUG_PRINTF("Transactions still in progress, not terminating on-card Transaction (current depth = %i)", slot->transaction_depth);

		return(CACKEY_PCSC_S_OK);
	}

	scard_trans_ret = SCardEndTransaction(slot->pcsc_card, SCARD_LEAVE_CARD);
	if (scard_trans_ret != SCARD_S_SUCCESS) {
		CACKEY_DEBUG_PRINTF("Unable to end transaction, returning in error");

		return(CACKEY_PCSC_E_GENERIC);
	}

	CACKEY_DEBUG_PRINTF("Sucessfully terminated transaction on slot (%s)", slot->pcsc_reader);

	return(CACKEY_PCSC_S_OK);
}

/* APDU Related Functions */
/*
 * SYNPOSIS
 *     cackey_ret cackey_send_apdu(struct cackey_slot *slot, unsigned char class, unsigned char instruction, unsigned char p1, unsigned char p2, unsigned char lc, unsigned char *data, unsigned char le, uint16_t *respcode, unsigned char *respdata, size_t *respdata_len);
 *
 * ARGUMENTS
 *     cackey_slot *slot
 *         Slot to send commands to
 *
 *     unsigned char class
 *         APDU Class (GSCIS_CLASS_ISO7816 or GSCIS_CLASS_GLOBAL_PLATFORM
 *         usually), (CLA)
 *
 *     unsigned char instruction
 *         APDU Instruction (INS)
 *
 *     unsigned char p1
 *         APDU Parameter 1 (P1)
 *
 *     unsigned char p2
 *         APDU Parameter 2 (P2)
 *
 *     unsigned char lc
 *         APDU Length of Content (Lc) -- this is the length of "data"
 *         parameter.  If "data" is specified as NULL, this parameter will
 *         be ignored.
 *
 *     unsigned char *data
 *         Pointer to buffer to send.  It should be "Lc" bytes long.  If
 *         specified as NULL, "Lc" will not be sent, and this buffer will be
 *         ignored.
 *
 *     unsigned char le
 *         APDU Length of Expectation (Le) -- this is the length of the
 *         expected reply.  If this is specified as 0 then it will not
 *         be sent.
 *
 *     uint16_t *respcode
 *         [OUT] Pointer to storage of APDU response code.  If this is
 *         specified as NULL, the response code will be discarded.
 *
 *     unsigned char *respdata
 *         [OUT] Pointer to storage of APDU response data.  If this is
 *         specified as NULL, the response data will be discarded.  If
 *         the "respdata_len" parameter is specified as NULL, this buffer
 *         will not be updated.
 *
 *     size_t *respdata_len
 *         [IN, OUT] Pointer initialing containing the size of the "respdata"
 *         buffer.  Before returning, the pointed to value is updated to the
 *         number of bytes written to the buffer.  If this is specified as
 *         NULL, it will not be updated, and "respdata" will be ignored causing
 *         the response data to be discarded.
 *
 * RETURN VALUE
 *     CACKEY_PCSC_S_OK           On success
 *     CACKEY_PCSC_E_GENERIC      On error
 *     CACKEY_PCSC_E_TOKENABSENT  If the sending failed because the token is
 *                                absent
 *
 * NOTES
 *     This function will connect to the PC/SC Connection Manager via
 *     cackey_pcsc_connect() if needed.
 *
 *     It will connect to the card in the reader attached to the slot
 *     specified.  It will reconnect to the card if the connection
 *     goes away.
 *
 */
static cackey_ret cackey_send_apdu(struct cackey_slot *slot, unsigned char class, unsigned char instruction, unsigned char p1, unsigned char p2, unsigned char lc, unsigned char *data, unsigned char le, uint16_t *respcode, unsigned char *respdata, size_t *respdata_len) {
	uint8_t major_rc, minor_rc;
	size_t bytes_to_copy, tmp_respdata_len;
	LPCSCARD_IO_REQUEST pioSendPci;
	DWORD protocol;
	DWORD xmit_len, recv_len;
	LONG scard_xmit_ret, scard_reconn_ret;
	BYTE xmit_buf[1024], recv_buf[1024];
	int pcsc_connect_ret, pcsc_getresp_ret;
	int idx;

	CACKEY_DEBUG_PRINTF("Called.");

	if (!slot) {
		CACKEY_DEBUG_PRINTF("Invalid slot specified.");

		return(CACKEY_PCSC_E_GENERIC);
	}

	pcsc_connect_ret = cackey_connect_card(slot);
	if (pcsc_connect_ret != CACKEY_PCSC_S_OK) {
		CACKEY_DEBUG_PRINTF("Unable to connect to card, returning in failure");

		return(CACKEY_PCSC_E_GENERIC);
	}

	/* Determine which protocol to send using */
	switch (slot->protocol) {
		case SCARD_PROTOCOL_T0:
			pioSendPci = SCARD_PCI_T0;

			break;
		case SCARD_PROTOCOL_T1:
			pioSendPci = SCARD_PCI_T1;

			break;
		default:
			CACKEY_DEBUG_PRINTF("Invalid protocol found, aborting.");

			return(CACKEY_PCSC_E_GENERIC);
	}

	/* Transmit */
	xmit_len = 0;
	xmit_buf[xmit_len++] = class;
	xmit_buf[xmit_len++] = instruction;
	xmit_buf[xmit_len++] = p1;
	xmit_buf[xmit_len++] = p2;
	if (data) {
		xmit_buf[xmit_len++] = lc;
		for (idx = 0; idx < lc; idx++) {
			xmit_buf[xmit_len++] = data[idx];
		}
	}

	if (le != 0x00) {
		xmit_buf[xmit_len++] = le;
	}

	/* Begin Smartcard Transaction */
	cackey_begin_transaction(slot);

	if (class == GSCIS_CLASS_ISO7816 && instruction == GSCIS_INSTR_VERIFY && p1 == 0x00 && p2 == 0x00) {
		CACKEY_DEBUG_PRINTF("Sending APDU: <<censored>>");
	} else {
		CACKEY_DEBUG_PRINTBUF("Sending APDU:", xmit_buf, xmit_len);
	}

	recv_len = sizeof(recv_buf);
	scard_xmit_ret = SCardTransmit(slot->pcsc_card, pioSendPci, xmit_buf, xmit_len, NULL, recv_buf, &recv_len);

	if (scard_xmit_ret == SCARD_E_NOT_TRANSACTED) {
		CACKEY_DEBUG_PRINTF("Failed to send APDU to card (SCardTransmit() = %s/%lx), will ask calling function to retry (not resetting card)...", CACKEY_DEBUG_FUNC_SCARDERR_TO_STR(scard_xmit_ret), (unsigned long) scard_xmit_ret);

		/* Begin Smartcard Transaction */
		cackey_end_transaction(slot);

		return(CACKEY_PCSC_E_RETRY);
	}

	if (scard_xmit_ret != SCARD_S_SUCCESS) {
		CACKEY_DEBUG_PRINTF("Failed to send APDU to card (SCardTransmit() = %s/%lx)", CACKEY_DEBUG_FUNC_SCARDERR_TO_STR(scard_xmit_ret), (unsigned long) scard_xmit_ret);

		CACKEY_DEBUG_PRINTF("Marking slot as having been reset");
		cackey_mark_slot_reset(slot);

		if (scard_xmit_ret == SCARD_W_RESET_CARD) {
			CACKEY_DEBUG_PRINTF("Reset required, please hold...");

			scard_reconn_ret = cackey_reconnect_card(slot, SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &protocol);

			if (scard_reconn_ret == SCARD_S_SUCCESS) {
				/* Update protocol */
				slot->protocol = protocol;
				switch (slot->protocol) {
					case SCARD_PROTOCOL_T0:
						pioSendPci = SCARD_PCI_T0;

						break;
					case SCARD_PROTOCOL_T1:
						pioSendPci = SCARD_PCI_T1;

						break;
					default:
						CACKEY_DEBUG_PRINTF("Invalid protocol found, but too late to do anything about it now -- trying anyway.");

						break;
				}

				/* Re-establish transaction, if it was present */
				if (slot->transaction_depth > 0) {
					slot->transaction_depth--;
					slot->transaction_need_hw_lock = 1;
					cackey_begin_transaction(slot);
				}

				CACKEY_DEBUG_PRINTF("Reset successful, retransmitting");

				recv_len = sizeof(recv_buf);
				scard_xmit_ret = SCardTransmit(slot->pcsc_card, pioSendPci, xmit_buf, xmit_len, NULL, recv_buf, &recv_len);

				if (scard_xmit_ret != SCARD_S_SUCCESS) {
					CACKEY_DEBUG_PRINTF("Retransmit failed, returning in failure after disconnecting the card (SCardTransmit = %s/%li)", CACKEY_DEBUG_FUNC_SCARDERR_TO_STR(scard_xmit_ret), (long) scard_xmit_ret);

					SCardDisconnect(slot->pcsc_card, SCARD_LEAVE_CARD);
					slot->pcsc_card_connected = 0;

					/* End Smartcard Transaction */
					slot->transaction_depth = 1;
					cackey_end_transaction(slot);

					return(CACKEY_PCSC_E_TOKENABSENT);
				}
			} else {
				CACKEY_DEBUG_PRINTF("Disconnecting card");

				SCardDisconnect(slot->pcsc_card, SCARD_LEAVE_CARD);
				slot->pcsc_card_connected = 0;

				/* End Smartcard Transaction */
				slot->transaction_depth = 1;
				cackey_end_transaction(slot);

				CACKEY_DEBUG_PRINTF("Returning in failure");
				return(CACKEY_PCSC_E_TOKENABSENT);
			}
		} else {
			CACKEY_DEBUG_PRINTF("Disconnecting card");

			SCardDisconnect(slot->pcsc_card, SCARD_LEAVE_CARD);
			slot->pcsc_card_connected = 0;

			/* End Smartcard Transaction */
			slot->transaction_depth = 1;
			cackey_end_transaction(slot);

			CACKEY_DEBUG_PRINTF("Returning in failure");
			return(CACKEY_PCSC_E_TOKENABSENT);
		}
	}

	CACKEY_DEBUG_PRINTBUF("Returned Value:", recv_buf, recv_len);

	if (recv_len < 2) {
		/* Minimal response length is 2 bytes, returning in failure */
		CACKEY_DEBUG_PRINTF("Response too small, returning in failure (recv_len = %lu)", (unsigned long) recv_len);

		/* End Smartcard Transaction */
		cackey_end_transaction(slot);

		return(CACKEY_PCSC_E_GENERIC);
	}

	/* Determine result code */
	major_rc = recv_buf[recv_len - 2];
	minor_rc = recv_buf[recv_len - 1];
	if (respcode) {
		*respcode = (major_rc << 8) | minor_rc;
	}

	/* Adjust message buffer */
	recv_len -= 2;

	/* Add bytes to return value */
	tmp_respdata_len = 0;
	if (respdata && respdata_len) {
		tmp_respdata_len = *respdata_len;

		bytes_to_copy = *respdata_len;

		if (recv_len < bytes_to_copy) {
			bytes_to_copy = recv_len;
		}

		CACKEY_DEBUG_PRINTF("Copying %lu bytes to the buffer (recv'd %lu bytes, but only %lu bytes left in our buffer)", (unsigned long) bytes_to_copy, (unsigned long) recv_len, (unsigned long) *respdata_len);

		memcpy(respdata, recv_buf, bytes_to_copy);
		respdata += bytes_to_copy;

		*respdata_len = bytes_to_copy;
		tmp_respdata_len -= bytes_to_copy;
	} else {
		if (recv_len != 0) {
			CACKEY_DEBUG_PRINTF("Throwing away %lu bytes, nowhere to put them!", (unsigned long) recv_len);
		}
	}

	if (major_rc == 0x61) {
		/* We need to READ */
		CACKEY_DEBUG_PRINTF("Buffer read required");

		if (minor_rc == 0x00) {
			minor_rc = CACKEY_APDU_MTU;
		}

		pcsc_getresp_ret = cackey_send_apdu(slot, GSCIS_CLASS_ISO7816, GSCIS_INSTR_GET_RESPONSE, 0x00, 0x00, 0, NULL, minor_rc, respcode, respdata, &tmp_respdata_len);

		if (pcsc_getresp_ret != CACKEY_PCSC_S_OK) {
			CACKEY_DEBUG_PRINTF("Buffer read failed!  Returning in failure");

			/* End Smartcard Transaction */
			cackey_end_transaction(slot);

			if (pcsc_getresp_ret == CACKEY_PCSC_E_RETRY) {
				return(CACKEY_PCSC_E_RETRY);
			}

			return(CACKEY_PCSC_E_GENERIC);
		}

		if (respdata_len) {
			*respdata_len += tmp_respdata_len;
		}

		/* End Smartcard Transaction */
		cackey_end_transaction(slot);

		CACKEY_DEBUG_PRINTF("Returning in success (buffer read complete)");
		return(CACKEY_PCSC_S_OK);
	}

	/* End Smartcard Transaction */
	cackey_end_transaction(slot);

	if (major_rc == 0x90) {
		/* Success */
		CACKEY_DEBUG_PRINTF("Returning in success (major_rc = 0x90)");

		return(CACKEY_PCSC_S_OK);
	}


	CACKEY_DEBUG_PRINTF("APDU Returned an error, returning in failure");

	return(CACKEY_PCSC_E_GENERIC);
}

/*
 * SYNPOSIS
 *     ssize_t cackey_read_buffer(struct cackey_slot *slot, unsigned char *buffer, size_t count, unsigned char t_or_v, size_t initial_offset);
 *
 * ARGUMENTS
 *     struct cackey_slot *slot
 *         Slot to send commands to
 *
 *     unsigned char *buffer
 *         [OUT] Buffer
 *
 *     size_t count
 *         Number of bytes to attempt to read
 *
 *     unsigned char t_or_v
 *         Select the T-buffer (01) or V-buffer (02) to read from.  
 *
 *     size_t initial_offset
 *         Specify the offset to begin the read from
 *
 *
 * RETURN VALUE
 *     This function returns the number of bytes actually read, or -1 on error.
 *
 * NOTES
 *     None
 *
 */
static ssize_t cackey_read_buffer(struct cackey_slot *slot, unsigned char *buffer, size_t count, unsigned char t_or_v, size_t initial_offset) {
	unsigned char *init_buffer;
	size_t init_count;
	size_t init_initial_offset;

	size_t offset = 0, max_offset, max_count;
	unsigned char cmd[2];
	uint16_t respcode;
	int send_ret;

	CACKEY_DEBUG_PRINTF("Called.");

	init_buffer = buffer;
	init_count = count;
	init_initial_offset = initial_offset;

	max_offset = count;
	max_count = CACKEY_APDU_MTU;

	if (t_or_v != 1 && t_or_v != 2) {
		CACKEY_DEBUG_PRINTF("Invalid T or V parameter specified, returning in failure");

		return(-1);
	}

	cmd[0] = t_or_v;

	while (1) {
		if (offset >= max_offset) {
			CACKEY_DEBUG_PRINTF("Buffer too small, returning what we got...");

			break;
		}

		count = max_offset - offset;
		if (count > max_count) {
			count = max_count;
		}

		cmd[1] = count;

		send_ret = cackey_send_apdu(slot, GSCIS_CLASS_GLOBAL_PLATFORM, GSCIS_INSTR_READ_BUFFER, ((initial_offset + offset) >> 8) & 0xff, (initial_offset + offset) & 0xff, sizeof(cmd), cmd, 0x00, &respcode, buffer + offset, &count);

		if (send_ret == CACKEY_PCSC_E_RETRY) {
			CACKEY_DEBUG_PRINTF("ADPU Sending failed, retrying read buffer");

			return(cackey_read_buffer(slot, init_buffer, init_count, t_or_v, init_initial_offset));
		}

		if (send_ret != CACKEY_PCSC_S_OK) {
			if (respcode == 0x6A86) {
				if (max_count == 1) {
					break;
				}

				max_count = max_count / 2;

				continue;
			}

			CACKEY_DEBUG_PRINTF("cackey_send_apdu() failed, returning in failure");

			return(-1);
		}

		offset += count;

		if (count < max_count) {
			CACKEY_DEBUG_PRINTF("Short read -- count = %i, cmd[1] = %i", (int) count, (int) cmd[1]);

			break;
		}
	}

#ifdef CACKEY_PARANOID
#  ifdef _POSIX_SSIZE_MAX
	if (offset > _POSIX_SSIZE_MAX) {
		CACKEY_DEBUG_PRINTF("Offset exceeds maximum value, returning in failure. (max = %li, offset = %lu)", (long) _POSIX_SSIZE_MAX, (unsigned long) offset);

		return(-1);
	}
#  endif
#endif

	CACKEY_DEBUG_PRINTF("Returning in success, read %lu bytes", (unsigned long) offset);

	return(offset);
}

/*
 * SYNPOSIS
 *     cackey_ret cackey_select_applet(struct cackey_slot *slot, unsigned char *aid, size_t aid_len);
 *
 * ARGUMENTS
 *     struct cackey_slot *slot
 *         Slot to send commands to
 *
 *     unsigned char *aid
 *         Buffer containing Applet ID to select
 *
 *     size_t aid_len
 *         Number of bytes in the "aid" (Applet ID) parameter
 *
 * RETURN VALUE
 *     CACKEY_PCSC_S_OK         On success
 *     CACKEY_PCSC_E_GENERIC    On error
 *
 * NOTES
 *     None
 *
 */
static cackey_ret cackey_select_applet(struct cackey_slot *slot, unsigned char *aid, size_t aid_len) {
	int send_ret;

	CACKEY_DEBUG_PRINTF("Called.");

	CACKEY_DEBUG_PRINTBUF("Selecting applet:", aid, aid_len);

	send_ret = cackey_send_apdu(slot, GSCIS_CLASS_ISO7816, GSCIS_INSTR_SELECT, GSCIS_PARAM_SELECT_APPLET, 0x00, aid_len, aid, 0x00, NULL, NULL, NULL);

	if (send_ret == CACKEY_PCSC_E_RETRY) {
		CACKEY_DEBUG_PRINTF("ADPU Sending failed, retrying select applet");

		return(cackey_select_applet(slot, aid, aid_len));
	}

	if (send_ret != CACKEY_PCSC_S_OK) {
		CACKEY_DEBUG_PRINTF("Failed to open applet, returning in failure");

		return(CACKEY_PCSC_E_GENERIC);
	}

	CACKEY_DEBUG_PRINTF("Successfully selected file");

	return(CACKEY_PCSC_S_OK);
}

/*
 * SYNPOSIS
 *     cackey_ret cackey_select_file(struct cackey_slot *slot, uint16_t ef);
 *
 * ARGUMENTS
 *     struct cackey_slot *slot
 *         Slot to send commands to
 *
 *     uint16_t ef
 *         Elemental File to select
 *
 * RETURN VALUE
 *     CACKEY_PCSC_S_OK         On success
 *     CACKEY_PCSC_E_GENERIC    On error
 *
 * NOTES
 *     This selects an Elementary File (EF) under the currently selected
 *     Dedicated File (DF)
 *
 *     Typically this is called after selecting the correct Applet (using
 *     cackey_select_applet) for VM cards
 *
 */
static cackey_ret cackey_select_file(struct cackey_slot *slot, uint16_t ef) {
	unsigned char fid_buf[2];
	int send_ret;

	CACKEY_DEBUG_PRINTF("Called.");

	/* Open the elementary file */
	fid_buf[0] = (ef >> 8) & 0xff;
	fid_buf[1] = ef & 0xff;

	CACKEY_DEBUG_PRINTF("Selecting file: %04lx", (unsigned long) ef);

	send_ret = cackey_send_apdu(slot, GSCIS_CLASS_ISO7816, GSCIS_INSTR_SELECT, 0x02, 0x0C, sizeof(fid_buf), fid_buf, 0x00, NULL, NULL, NULL);
	if (send_ret != CACKEY_PCSC_S_OK) {
		CACKEY_DEBUG_PRINTF("Failed to open file, returning in failure");

		return(CACKEY_PCSC_E_GENERIC);
	}

	CACKEY_DEBUG_PRINTF("Successfully selected file");

	return(CACKEY_PCSC_S_OK);
}

/*
 * SYNPOSIS
 *     void cackey_free_tlv(struct cackey_tlv_entity *root);
 *
 * ARGUMENTS
 *     struct cackey_tlv_entity *root
 *         Root of the TLV list to start freeing
 *
 * RETURN VALUE
 *     None
 *
 * NOTES
 *     This function frees the TLV linked listed returned from
 *     "cackey_read_tlv"
 *
 */
static void cackey_free_tlv(struct cackey_tlv_entity *root) {
	struct cackey_tlv_entity *curr, *next;

	if (root == NULL) {
		return;
	}

	for (curr = root; curr; curr = next) {
		next = curr->_next;

		switch (curr->tag) {
			case GSCIS_TAG_ACR_TABLE:
			case GSCIS_TAG_CERTIFICATE:
				if (curr->value) {
					free(curr->value);
				}
				break;
			case GSCIS_TAG_CARDURL:
				if (curr->value_cardurl) {
					free(curr->value_cardurl);
				}
				break;
		}

		free(curr);
	}

	return;
}

/*
 * SYNPOSIS
 *     ...
 *
 * ARGUMENTS
 *     ...
 *
 * RETURN VALUE
 *     ...
 *
 * NOTES
 *     ...
 *
 */
static struct cackey_tlv_entity *cackey_read_tlv(struct cackey_slot *slot) {
	struct cackey_tlv_entity *curr_entity, *root = NULL, *last = NULL;
	unsigned char tlen_buf[2], tval_buf[1024], *tval;
	unsigned char vlen_buf[2], vval_buf[8192], *vval;
	unsigned char *tmpbuf;
	unsigned long tmpbuflen;
	ssize_t tlen, vlen;
	ssize_t read_ret;
	size_t offset_t = 0, offset_v = 0;
	unsigned char tag;
	size_t length;
#ifdef HAVE_LIBZ
	int uncompress_ret;
#endif

	CACKEY_DEBUG_PRINTF("Called.");

	read_ret = cackey_read_buffer(slot, tlen_buf, sizeof(tlen_buf), 1, offset_t);
	if (read_ret != sizeof(tlen_buf)) {
		CACKEY_DEBUG_PRINTF("Read failed, returning in failure");

		return(NULL);
	}

	tlen = (tlen_buf[1] << 8) | tlen_buf[0];

	read_ret = cackey_read_buffer(slot, vlen_buf, sizeof(vlen_buf), 2, offset_v);
	if (read_ret != sizeof(vlen_buf)) {
		CACKEY_DEBUG_PRINTF("Read failed, returning in failure");

		return(NULL);
	}

	vlen = (vlen_buf[1] << 8) | vlen_buf[0];

	CACKEY_DEBUG_PRINTF("Tag Length = %lu, Value Length = %lu", (unsigned long) tlen, (unsigned long) vlen);

	offset_t += 2;
	offset_v += 2;

	if (tlen > sizeof(tval_buf)) {
		CACKEY_DEBUG_PRINTF("Tag length is too large, returning in failure");

		return(NULL);
	}

	if (vlen > sizeof(vval_buf)) {
		CACKEY_DEBUG_PRINTF("Value length is too large, returning in failure");

		return(NULL);
	}

	read_ret = cackey_read_buffer(slot, tval_buf, tlen, 1, offset_t);
	if (read_ret != tlen) {
		CACKEY_DEBUG_PRINTF("Unable to read entire T-buffer, returning in failure");

		return(NULL);
	}

	read_ret = cackey_read_buffer(slot, vval_buf, vlen, 2, offset_v);
	if (read_ret != vlen) {
		CACKEY_DEBUG_PRINTF("Unable to read entire V-buffer, returning in failure");

		return(NULL);
	}

	tval = tval_buf;
	vval = vval_buf;
	while (tlen > 0 && vlen > 0) {
		tag = *tval;
		tval++;
		tlen--;

		if (*tval == 0xff) {
			length = (tval[2] << 8) | tval[1];
			tval += 3;
			tlen -= 3;
		} else {
			length = *tval;
			tval++;
			tlen--;
		}

		CACKEY_DEBUG_PRINTF("Tag: %s (%02x)", CACKEY_DEBUG_FUNC_TAG_TO_STR(tag), (unsigned int) tag);
		CACKEY_DEBUG_PRINTBUF("Value:", vval, length);

		curr_entity = NULL;
		switch (tag) {
			case GSCIS_TAG_CARDURL:
				curr_entity = malloc(sizeof(*curr_entity));
				curr_entity->value_cardurl = malloc(sizeof(*curr_entity->value_cardurl));

				memcpy(curr_entity->value_cardurl->rid, vval, 5);
				curr_entity->value_cardurl->apptype = vval[5];
				curr_entity->value_cardurl->objectid = (vval[6] << 8) | vval[7];
				curr_entity->value_cardurl->appid = (vval[8] << 8) | vval[9];

				curr_entity->tag = tag;
				curr_entity->_next = NULL;

				break;
			case GSCIS_TAG_ACR_TABLE:
				curr_entity = malloc(sizeof(*curr_entity));
				tmpbuf = malloc(length);

				memcpy(tmpbuf, vval, length);

				curr_entity->tag = tag;
				curr_entity->length = length;
				curr_entity->value = tmpbuf;
				curr_entity->_next = NULL;

				break;
			case GSCIS_TAG_CERTIFICATE:
				curr_entity = malloc(sizeof(*curr_entity));

#ifdef HAVE_LIBZ
				tmpbuflen = length * 2;
				tmpbuf = malloc(tmpbuflen);

				uncompress_ret = uncompress(tmpbuf, &tmpbuflen, vval, length);
				if (uncompress_ret != Z_OK) {
					CACKEY_DEBUG_PRINTF("Failed to decompress, uncompress() returned %i -- resorting to direct copy", uncompress_ret);

					tmpbuflen = length;
					memcpy(tmpbuf, vval, length);
				}

				CACKEY_DEBUG_PRINTBUF("Decompressed to:", tmpbuf, tmpbuflen);
#else
				CACKEY_DEBUG_PRINTF("Missing ZLIB Support, this certificate is likely useless...");

				tmpbuflen = length;
				memcpy(tmpbuf, vval, length);
#endif

				curr_entity->tag = tag;
				curr_entity->length = tmpbuflen;
				curr_entity->value = tmpbuf;
				curr_entity->_next = NULL;

				break;
			case GSCIS_TAG_PKCS15:
				curr_entity = malloc(sizeof(*curr_entity));

				curr_entity->tag = tag;
				curr_entity->value_byte = vval[0];
				curr_entity->_next = NULL;

				break;
		}

		vval += length;
		vlen -= length;

		if (curr_entity != NULL) {
			if (root == NULL) {
				root = curr_entity;
			}

			if (last != NULL) {
				last->_next = curr_entity;
			}

			last = curr_entity;
		}
	}

	return(root);
}

/*
 * SYNPOSIS
 *     ...
 *
 * ARGUMENTS
 *     ...
 *
 * RETURN VALUE
 *     ...
 *
 * NOTES
 *     ...
 *
 */
static void cackey_free_certs(struct cackey_pcsc_identity *start, size_t count, int free_start) {
	size_t idx;

	if (start == NULL) {
		return;
	}

	for (idx = 0; idx < count; idx++) {
		if (start[idx].certificate) {
			free(start[idx].certificate);
		}
	}

	if (free_start) {
		free(start);
	}

	return;
}

static struct cackey_pcsc_identity *cackey_copy_certs(struct cackey_pcsc_identity *dest, struct cackey_pcsc_identity *start, size_t count) {
	size_t idx;

	if (start == NULL) {
		return(NULL);
	}

	if (dest == NULL) {
		dest = malloc(sizeof(*dest) * count);
	}

	for (idx = 0; idx < count; idx++) {
		memcpy(dest[idx].applet, start[idx].applet, sizeof(dest[idx].applet));
		dest[idx].file = start[idx].file;
		dest[idx].certificate_len = start[idx].certificate_len;
		dest[idx].keysize = start[idx].keysize;

		dest[idx].certificate = malloc(dest[idx].certificate_len);
		memcpy(dest[idx].certificate, start[idx].certificate, dest[idx].certificate_len);
	}

	return(dest);
}

/*
 * SYNPOSIS
 *     ...
 *
 * ARGUMENTS
 *     ...
 *
 * RETURN VALUE
 *     ...
 *
 * NOTES
 *     ...
 *
 */
static struct cackey_pcsc_identity *cackey_read_certs(struct cackey_slot *slot, struct cackey_pcsc_identity *certs, unsigned long *count) {
	struct cackey_pcsc_identity *curr_id;
	struct cackey_tlv_entity *ccc_tlv, *ccc_curr, *app_tlv, *app_curr;
	unsigned char ccc_aid[] = {GSCIS_AID_CCC};
	unsigned char curr_aid[7];
	unsigned long outidx = 0;
	cackey_ret transaction_ret;
	int certs_resizable;
	int send_ret, select_ret;

	CACKEY_DEBUG_PRINTF("Called.");

	if (count == NULL) {
		CACKEY_DEBUG_PRINTF("count is NULL, returning in failure");

		return(NULL);
	}

	if (certs != NULL) {
		if (*count == 0) {
			CACKEY_DEBUG_PRINTF("Requested we return 0 objects, short-circuit");

			return(certs);
		}
	}

	if (!slot->slot_reset) {
		if (slot->cached_certs) {
			if (certs == NULL) {
				certs = malloc(sizeof(*certs) * slot->cached_certs_count);
				*count = slot->cached_certs_count;

			} else {
				if (*count > slot->cached_certs_count) {
					*count = slot->cached_certs_count;
				}
			}

			cackey_copy_certs(certs, slot->cached_certs, *count);

			return(certs);
		}
	}

	if (slot->cached_certs) {
		cackey_free_certs(slot->cached_certs, slot->cached_certs_count, 1);

		slot->cached_certs = NULL;
	}

	/* Begin a SmartCard transaction */
	transaction_ret = cackey_begin_transaction(slot);
	if (transaction_ret != CACKEY_PCSC_S_OK) {
		CACKEY_DEBUG_PRINTF("Unable begin transaction, returning in failure");

		return(NULL);
	}

	if (certs == NULL) {
		certs = malloc(sizeof(*certs) * 5);
		*count = 5;
		certs_resizable = 1;
	} else {
		certs_resizable = 0;
	}

	/* Select the CCC Applet */
	send_ret = cackey_select_applet(slot, ccc_aid, sizeof(ccc_aid));
	if (send_ret != CACKEY_PCSC_S_OK) {
		CACKEY_DEBUG_PRINTF("Unable to select CCC Applet, returning in failure");

		/* Terminate SmartCard Transaction */
		cackey_end_transaction(slot);

		return(NULL);
	}

	/* Read all the applets from the CCC's TLV */
	ccc_tlv = cackey_read_tlv(slot);

	/* Look for CARDURLs that coorespond to PKI applets */
	for (ccc_curr = ccc_tlv; ccc_curr; ccc_curr = ccc_curr->_next) {
		CACKEY_DEBUG_PRINTF("Found tag: %s ... ", CACKEY_DEBUG_FUNC_TAG_TO_STR(ccc_curr->tag));

		if (ccc_curr->tag != GSCIS_TAG_CARDURL) {
			CACKEY_DEBUG_PRINTF("  ... skipping it (we only care about CARDURLs)");

			continue;
		}

		if ((ccc_curr->value_cardurl->apptype & CACKEY_TLV_APP_PKI) != CACKEY_TLV_APP_PKI) {
			CACKEY_DEBUG_PRINTF("  ... skipping it (we only care about PKI applets, this applet supports: %s/%02x)", CACKEY_DEBUG_FUNC_APPTYPE_TO_STR(ccc_curr->value_cardurl->apptype), (unsigned int) ccc_curr->value_cardurl->apptype);

			continue;
		}

		CACKEY_DEBUG_PRINTBUF("RID:", ccc_curr->value_cardurl->rid, sizeof(ccc_curr->value_cardurl->rid));
		CACKEY_DEBUG_PRINTF("AppID = %s/%04lx", CACKEY_DEBUG_FUNC_OBJID_TO_STR(ccc_curr->value_cardurl->appid), (unsigned long) ccc_curr->value_cardurl->appid);
		CACKEY_DEBUG_PRINTF("ObjectID = %s/%04lx", CACKEY_DEBUG_FUNC_OBJID_TO_STR(ccc_curr->value_cardurl->objectid), (unsigned long) ccc_curr->value_cardurl->objectid);

		memcpy(curr_aid, ccc_curr->value_cardurl->rid, sizeof(ccc_curr->value_cardurl->rid));
		curr_aid[sizeof(curr_aid) - 2] = (ccc_curr->value_cardurl->appid >> 8) & 0xff;
		curr_aid[sizeof(curr_aid) - 1] = ccc_curr->value_cardurl->appid & 0xff;

		/* Select found applet ... */
		select_ret = cackey_select_applet(slot, curr_aid, sizeof(curr_aid));
		if (select_ret != CACKEY_PCSC_S_OK) {
			CACKEY_DEBUG_PRINTF("Failed to select applet, skipping processing of this object");

			continue;
		}

		/* ... and object (file) */
		select_ret = cackey_select_file(slot, ccc_curr->value_cardurl->objectid);
		if (select_ret != CACKEY_PCSC_S_OK) {
			CACKEY_DEBUG_PRINTF("Failed to select file, skipping processing of this object");

			continue;
		}

		/* Process this file's TLV looking for certificates */
		app_tlv = cackey_read_tlv(slot);

		for (app_curr = app_tlv; app_curr; app_curr = app_curr->_next) {
			CACKEY_DEBUG_PRINTF("Found tag: %s", CACKEY_DEBUG_FUNC_TAG_TO_STR(app_curr->tag));
			if (app_curr->tag != GSCIS_TAG_CERTIFICATE) {
				CACKEY_DEBUG_PRINTF("  ... skipping it (we only care about CERTIFICATEs)");

				continue;
			}

			curr_id = &certs[outidx];
			outidx++;

			memcpy(curr_id->applet, curr_aid, sizeof(curr_id->applet));
			curr_id->file = ccc_curr->value_cardurl->objectid;
			curr_id->keysize = -1;

			CACKEY_DEBUG_PRINTF("Filling curr_id->applet (%p) with %lu bytes:", curr_id->applet, (unsigned long) sizeof(curr_id->applet));
			CACKEY_DEBUG_PRINTBUF("VAL:", curr_id->applet, sizeof(curr_id->applet));

			curr_id->certificate_len = app_curr->length;

			curr_id->certificate = malloc(curr_id->certificate_len);
			memcpy(curr_id->certificate, app_curr->value, curr_id->certificate_len);

			if (outidx >= *count) {
				if (certs_resizable) {
					*count *= 2;
					certs = realloc(certs, sizeof(*certs) * (*count));
				} else {
					break;
				}
			}
		}

		cackey_free_tlv(app_tlv);

		if (outidx >= *count) {
			break;
		}
	}

	cackey_free_tlv(ccc_tlv);

	*count = outidx;

	if (certs_resizable) {
		certs = realloc(certs, sizeof(*certs) * (*count));
	}

	slot->cached_certs = cackey_copy_certs(NULL, certs, *count);
	slot->cached_certs_count = *count;

	/* Terminate SmartCard Transaction */
	cackey_end_transaction(slot);

	return(certs);
}

/*
 * SYNPOSIS
 *     ...
 *
 * ARGUMENTS
 *     ...
 *
 * RETURN VALUE
 *     ...
 *
 * NOTES
 *     ...
 *
 */
static ssize_t cackey_signdecrypt(struct cackey_slot *slot, struct cackey_identity *identity, unsigned char *buf, size_t buflen, unsigned char *outbuf, size_t outbuflen, int padInput, int unpadOutput) {
	unsigned char *tmpbuf, *tmpbuf_s, *outbuf_s;
	unsigned char bytes_to_send, p1;
	unsigned char blocktype;
	cackey_ret send_ret;
	uint16_t respcode;
	ssize_t retval = 0, unpadoffset;
	size_t tmpbuflen, padlen, tmpoutbuflen;
	int free_tmpbuf = 0;
	int le;

	CACKEY_DEBUG_PRINTF("Called.");

	if (slot == NULL) {
		CACKEY_DEBUG_PRINTF("Error.  slot is NULL");

		return(-1);
	}

	if (buf == NULL) {
		CACKEY_DEBUG_PRINTF("Error.  buf is NULL");

		return(-1);
	}

	if (outbuf == NULL) {
		CACKEY_DEBUG_PRINTF("Error.  outbuf is NULL");

		return(-1);
	}

	if (identity == NULL) {
		CACKEY_DEBUG_PRINTF("Error.  identity is NULL");

		return(-1);
	}

	if (identity->pcsc_identity == NULL) {
		CACKEY_DEBUG_PRINTF("Error.  identity->pcsc_identity is NULL");

		return(-1);
	}

	/* Determine identity Key size */
	if (identity->pcsc_identity->keysize < 0) {
		identity->pcsc_identity->keysize = x509_to_keysize(identity->pcsc_identity->certificate, identity->pcsc_identity->certificate_len);
	}

	/* Pad message to key size */
	if (padInput) {
		if (identity->pcsc_identity->keysize > 0) {
			if (buflen != identity->pcsc_identity->keysize) {
				if (buflen > (identity->pcsc_identity->keysize + 3)) {
					CACKEY_DEBUG_PRINTF("Error.  Message is too large to sign/decrypt");

					return(-1);
				}

				tmpbuflen = identity->pcsc_identity->keysize;
				tmpbuf = malloc(tmpbuflen);
				free_tmpbuf = 1;

				padlen = tmpbuflen - buflen - 3;

				/* RSA PKCS#1 EMSA-PKCS1-v1_5 Padding */
				tmpbuf[0] = 0x00;
				tmpbuf[1] = 0x01;
				memset(&tmpbuf[2], 0xFF, padlen);
				tmpbuf[padlen + 2]= 0x00;
				memcpy(&tmpbuf[padlen + 3], buf, buflen);

				CACKEY_DEBUG_PRINTBUF("Unpadded:", buf, buflen);
				CACKEY_DEBUG_PRINTBUF("Padded:", tmpbuf, tmpbuflen);
			} else {
				tmpbuf = buf;
				tmpbuflen = buflen;
				free_tmpbuf = 0;
				padlen = 0;
			}
		} else {
			CACKEY_DEBUG_PRINTF("Unable to determine key size, hoping the message is properly padded!");

			tmpbuf = buf;
			tmpbuflen = buflen;
			free_tmpbuf = 0;
			padlen = 0;
		}
	} else {
		tmpbuf = buf;
		tmpbuflen = buflen;
		free_tmpbuf = 0;
		padlen = 0;
	}

	/* Begin transaction */
	cackey_begin_transaction(slot);

	/* Select correct applet */
	CACKEY_DEBUG_PRINTF("Selecting applet found at %p ...", identity->pcsc_identity->applet);
	cackey_select_applet(slot, identity->pcsc_identity->applet, sizeof(identity->pcsc_identity->applet));

	/* Select correct file */
	cackey_select_file(slot, identity->pcsc_identity->file);

	tmpbuf_s = tmpbuf;
	outbuf_s = outbuf;
	while (tmpbuflen) {
		if (tmpbuflen > 245) {
			bytes_to_send = 245;
			p1 = 0x80;
			le = 0x00;
		} else {
			bytes_to_send = tmpbuflen;
			p1 = 0x00;
			le = 0x00;
		}

		tmpoutbuflen = outbuflen;

		send_ret = cackey_send_apdu(slot, GSCIS_CLASS_GLOBAL_PLATFORM, GSCIS_INSTR_SIGNDECRYPT, p1, 0x00, bytes_to_send, tmpbuf, le, &respcode, outbuf, &tmpoutbuflen);
		if (send_ret != CACKEY_PCSC_S_OK) {
			CACKEY_DEBUG_PRINTF("ADPU Sending Failed -- returning in error.");

			if (free_tmpbuf) {
				if (tmpbuf_s) {
					free(tmpbuf_s);
				}
			}

			/* End transaction */
			cackey_end_transaction(slot);

			if (respcode == 0x6982) {
				CACKEY_DEBUG_PRINTF("Security status not satisified.  Returning NEEDLOGIN");

				cackey_mark_slot_reset(slot);
				slot->token_flags = CKF_LOGIN_REQUIRED;

				return(CACKEY_PCSC_E_NEEDLOGIN);
			}

			if (send_ret == CACKEY_PCSC_E_TOKENABSENT) {
				CACKEY_DEBUG_PRINTF("Token absent.  Returning TOKENABSENT");

				cackey_mark_slot_reset(slot);

				return(CACKEY_PCSC_E_TOKENABSENT);
			}

			return(-1);
		}

		tmpbuf += bytes_to_send;
		tmpbuflen -= bytes_to_send;

		outbuf += tmpoutbuflen;
		outbuflen -= tmpoutbuflen;
		retval += tmpoutbuflen;
	}

	if (free_tmpbuf) {
		if (tmpbuf_s) {
			free(tmpbuf_s);
		}
	}

	outbuf = outbuf_s;

	/* End transaction */
	cackey_end_transaction(slot);

#ifdef CACKEY_PARANOID
#  ifdef _POSIX_SSIZE_MAX
	if (outbuflen > _POSIX_SSIZE_MAX) {
		CACKEY_DEBUG_PRINTF("Outbuflen exceeds maximum value, returning in failure. (max = %li, outbuflen = %lu)", (long) _POSIX_SSIZE_MAX, (unsigned long) outbuflen);

		return(-1);
	}
#  endif
#endif

	/* Unpad reply */
	if (unpadOutput) {
		if (retval < 3) {
			CACKEY_DEBUG_PRINTF("Reply is too small, we are not able to unpad -- passing back and hoping for the best!");

			CACKEY_DEBUG_PRINTF("Returning in success, retval = %li (bytes)", (long) retval);
			return(retval);
		}

		if (outbuf[0] != 0x00) {
			CACKEY_DEBUG_PRINTF("Unrecognized padding scheme -- passing back and hoping for the best!");

			CACKEY_DEBUG_PRINTF("Returning in success, retval = %li (bytes)", (long) retval);
			return(retval);
		}

		blocktype = outbuf[1];
		unpadoffset = 0;

		switch (blocktype) {
			case 0x00:
				/* Padding Scheme 1, the first non-zero byte is the start of data */
				for (unpadoffset = 2; unpadoffset < retval; unpadoffset++) {
					if (outbuf[unpadoffset] != 0x00) {
						break;
					}
				}
				break;
			case 0x01:
				/* Padding Scheme 2, pad bytes are 0xFF followed by 0x00 */
				for (unpadoffset = 2; unpadoffset < retval; unpadoffset++) {
					if (outbuf[unpadoffset] != 0xFF) {
						if (outbuf[unpadoffset] == 0x00) {
							unpadoffset++;

							break;
						} else {
							CACKEY_DEBUG_PRINTF("Invalid padding data found, returning in failure, should have been 0x00 found 0x%02x", (unsigned int) outbuf[unpadoffset]);

							return(-1);
						}
					} else {
						CACKEY_DEBUG_PRINTF("Invalid padding data found, returning in failure, should have been 0xFF found 0x%02x", (unsigned int) outbuf[unpadoffset]);

						return(-1);
					}
				}
				break;
			case 0x02:
				/* Padding Scheme 3, pad bytes are non-zero first zero byte found is the seperator byte */
				for (unpadoffset = 2; unpadoffset < retval; unpadoffset++) {
					if (outbuf[unpadoffset] == 0x00) {
						unpadoffset++;

						break;
					}
				}
				break;
		}

		if (unpadoffset > retval) {
			CACKEY_DEBUG_PRINTF("Offset greater than reply size, aborting.  (unpadoffset = %lu, retval = %lu)", (unsigned long) unpadoffset, (unsigned long) retval);

			return(-1);
		}

		CACKEY_DEBUG_PRINTBUF("Padded:", outbuf, retval);

		retval -= unpadoffset;
		memmove(outbuf, outbuf + unpadoffset, retval);

		CACKEY_DEBUG_PRINTBUF("Unpadded:", outbuf, retval);
	}


	CACKEY_DEBUG_PRINTF("Returning in success, retval = %li (bytes)", (long) retval);

	return(retval);
}

/*
 * SYNPOSIS
 *     ...
 *
 * ARGUMENTS
 *     ...
 *
 * RETURN VALUE
 *     ...
 *
 * NOTES
 *     ...
 *
 */
static cackey_ret cackey_login(struct cackey_slot *slot, unsigned char *pin, unsigned long pin_len, int *tries_remaining_p) {
	unsigned char cac_pin[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	uint16_t response_code;
	int tries_remaining;
	int send_ret;

	/* Indicate that we do not know about how many tries are remaining */
	if (tries_remaining_p) {
		*tries_remaining_p = -1;
	}

	/* Apparently, CAC PINs are *EXACTLY* 8 bytes long -- pad with 0xFF if too short */
	if (pin_len >= 8) {
		memcpy(cac_pin, pin, 8);
	} else {
		memcpy(cac_pin, pin, pin_len);
	}

	/* Issue PIN Verify */
	send_ret = cackey_send_apdu(slot, GSCIS_CLASS_ISO7816, GSCIS_INSTR_VERIFY, 0x00, 0x00, sizeof(cac_pin), cac_pin, 0x00, &response_code, NULL, NULL);
	if (send_ret != CACKEY_PCSC_S_OK) {
		if ((response_code & 0x63C0) == 0x63C0) {
			tries_remaining = (response_code & 0xF);

			CACKEY_DEBUG_PRINTF("PIN Verification failed, %i tries remaining", tries_remaining);

			if (tries_remaining_p) {
				*tries_remaining_p = tries_remaining;
			}

			return(CACKEY_PCSC_E_BADPIN);
		}

		if (response_code == 0x6983) {
			CACKEY_DEBUG_PRINTF("PIN Verification failed, device is locked");

			return(CACKEY_PCSC_E_LOCKED);
		}

		return(CACKEY_PCSC_E_GENERIC);
	}

	CACKEY_DEBUG_PRINTF("PIN Verification succeeded");

	return(CACKEY_PCSC_S_OK);
}

/*
 * SYNPOSIS
 *     ...
 *
 * ARGUMENTS
 *     ...
 *
 * RETURN VALUE
 *     ...
 *
 * NOTES
 *     ...
 *
 */
static cackey_ret cackey_token_present(struct cackey_slot *slot) {
	cackey_ret pcsc_connect_ret;
	DWORD reader_len = 0, state = 0, protocol = 0, atr_len;
	BYTE atr[MAX_ATR_SIZE];
	LONG status_ret, scard_reconn_ret;

	CACKEY_DEBUG_PRINTF("Called.");

	if (slot->internal) {
		CACKEY_DEBUG_PRINTF("Returning token present (internal token)");

		return(CACKEY_PCSC_S_TOKENPRESENT);
	}

	pcsc_connect_ret = cackey_connect_card(slot);
	if (pcsc_connect_ret != CACKEY_PCSC_S_OK) {
		CACKEY_DEBUG_PRINTF("Unable to connect to card, returning token absent");

		return(CACKEY_PCSC_E_TOKENABSENT);
	}

	atr_len = sizeof(atr);
	status_ret = SCardStatus(slot->pcsc_card, NULL, &reader_len, &state, &protocol, atr, &atr_len);

	if (status_ret == SCARD_E_INVALID_HANDLE) {
		CACKEY_DEBUG_PRINTF("SCardStatus() returned SCARD_E_INVALID_HANDLE, marking is not already connected and trying again");
		cackey_mark_slot_reset(slot);

		pcsc_connect_ret = cackey_connect_card(slot);
		if (pcsc_connect_ret != CACKEY_PCSC_S_OK) {
			CACKEY_DEBUG_PRINTF("Unable to connect to card, returning token absent");

			return(CACKEY_PCSC_E_TOKENABSENT);
		}

		atr_len = sizeof(atr);
		status_ret = SCardStatus(slot->pcsc_card, NULL, &reader_len, &state, &protocol, atr, &atr_len);
	}

	if (status_ret != SCARD_S_SUCCESS) {
		cackey_mark_slot_reset(slot);

		if (status_ret == SCARD_W_RESET_CARD) {
			CACKEY_DEBUG_PRINTF("Reset required, please hold...");

			scard_reconn_ret = cackey_reconnect_card(slot, SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &protocol);
			if (scard_reconn_ret == SCARD_S_SUCCESS) {
				/* Update protocol */
				slot->protocol = protocol;

				/* Re-establish transaction, if it was present */
				if (slot->transaction_depth > 0) {
					slot->transaction_depth--;
					slot->transaction_need_hw_lock = 1;
					cackey_begin_transaction(slot);
				}

				CACKEY_DEBUG_PRINTF("Reset successful, requerying");
				status_ret = SCardStatus(slot->pcsc_card, NULL, &reader_len, &state, &protocol, atr, &atr_len);
				if (status_ret != SCARD_S_SUCCESS) {
					CACKEY_DEBUG_PRINTF("Still unable to query card status, returning token absent.  SCardStatus() = %s", CACKEY_DEBUG_FUNC_SCARDERR_TO_STR(status_ret));

					return(CACKEY_PCSC_E_TOKENABSENT);
				}
			} else {
				CACKEY_DEBUG_PRINTF("Unable to reconnect to card, returning token absent.  SCardReconnect() = %s", CACKEY_DEBUG_FUNC_SCARDERR_TO_STR(scard_reconn_ret));

				return(CACKEY_PCSC_E_TOKENABSENT);
			}
		} else {
			CACKEY_DEBUG_PRINTF("Unable to query card status, returning token absent.  SCardStatus() = %s", CACKEY_DEBUG_FUNC_SCARDERR_TO_STR(status_ret));

			return(CACKEY_PCSC_E_TOKENABSENT);
		}
	}

	if ((state & SCARD_ABSENT) == SCARD_ABSENT) {
		CACKEY_DEBUG_PRINTF("Card is absent, returning token absent");

		return(CACKEY_PCSC_E_TOKENABSENT);
	}

	CACKEY_DEBUG_PRINTF("Returning token present.");

	return(CACKEY_PCSC_S_TOKENPRESENT);
}

/*
 * SYNPOSIS
 *     ...
 *
 * ARGUMENTS
 *     ...
 *
 * RETURN VALUE
 *     ...
 *
 * NOTES
 *     ...
 *
 */
static ssize_t cackey_pcsc_identity_to_label(struct cackey_pcsc_identity *identity, unsigned char *label_buf, unsigned long label_buf_len) {
	unsigned long certificate_len;
	void *label_asn1;
	void *certificate;
	int x509_read_ret;

	certificate = identity->certificate;
	certificate_len = identity->certificate_len;

	if (certificate_len < 0) {
		return(-1);
	}

	x509_read_ret = x509_to_subject(certificate, certificate_len, (void **) &label_asn1);
	if (x509_read_ret < 0) {
		return(-1);
	}

	x509_read_ret = x509_dn_to_string(label_asn1, x509_read_ret, (char *) label_buf, label_buf_len, "CN");
	if (x509_read_ret <= 0) {
		x509_read_ret = x509_dn_to_string(label_asn1, x509_read_ret, (char *) label_buf, label_buf_len, NULL);

		if (x509_read_ret <= 0) {
			return(-1);
		}
	}

#ifdef CACKEY_PARANOID
#  ifdef _POSIX_SSIZE_MAX
	if (x509_read_ret > _POSIX_SSIZE_MAX) {
		CACKEY_DEBUG_PRINTF("x509_read_ret exceeds maximum value, returning in failure. (max = %li, x509_read_ret = %lu)", (long) _POSIX_SSIZE_MAX, (unsigned long) x509_read_ret);

		return(-1);
	}
#  endif
#endif

	return(x509_read_ret);
}

/* Returns 0 on success */
static int cackey_mutex_create(void **mutex) {
	pthread_mutex_t *pthread_mutex;
	int pthread_retval;
	CK_RV custom_retval;

	CACKEY_DEBUG_PRINTF("Called.");

	if ((cackey_args.flags & CKF_OS_LOCKING_OK) == CKF_OS_LOCKING_OK) {
		pthread_mutex = malloc(sizeof(*pthread_mutex));
		if (!pthread_mutex) {
			CACKEY_DEBUG_PRINTF("Failed to allocate memory.");

			return(-1);
		}

		pthread_retval = pthread_mutex_init(pthread_mutex, NULL);
		if (pthread_retval != 0) {
			CACKEY_DEBUG_PRINTF("pthread_mutex_init() returned error (%i).", pthread_retval);

			return(-1);
		}

		*mutex = pthread_mutex;
	} else {
		if (cackey_args.CreateMutex) {
			custom_retval = cackey_args.CreateMutex(mutex);

			if (custom_retval != CKR_OK) {
				CACKEY_DEBUG_PRINTF("cackey_args.CreateMutex() returned error (%li).", (long) custom_retval);

				return(-1);
			}
		}
	}

	CACKEY_DEBUG_PRINTF("Returning sucessfully (0)");

	return(0);
}

/* Returns 0 on success */
static int cackey_mutex_lock(void *mutex) {
	pthread_mutex_t *pthread_mutex;
	int pthread_retval;
	CK_RV custom_retval;

	CACKEY_DEBUG_PRINTF("Called.");

	if ((cackey_args.flags & CKF_OS_LOCKING_OK) == CKF_OS_LOCKING_OK) {
		pthread_mutex = mutex;

		pthread_retval = pthread_mutex_lock(pthread_mutex);
		if (pthread_retval != 0) {
			CACKEY_DEBUG_PRINTF("pthread_mutex_lock() returned error (%i).", pthread_retval);

			return(-1);
		}
	} else {
		if (cackey_args.LockMutex) {
			custom_retval = cackey_args.LockMutex(mutex);

			if (custom_retval != CKR_OK) {
				CACKEY_DEBUG_PRINTF("cackey_args.LockMutex() returned error (%li).", (long) custom_retval);

				return(-1);
			}
		}
	}

	CACKEY_DEBUG_PRINTF("Returning sucessfully (0)");

	return(0);
}

/* Returns 0 on success */
static int cackey_mutex_unlock(void *mutex) {
	pthread_mutex_t *pthread_mutex;
	int pthread_retval;
	CK_RV custom_retval;

	CACKEY_DEBUG_PRINTF("Called.");

	if ((cackey_args.flags & CKF_OS_LOCKING_OK) == CKF_OS_LOCKING_OK) {
		pthread_mutex = mutex;

		pthread_retval = pthread_mutex_unlock(pthread_mutex);
		if (pthread_retval != 0) {
			CACKEY_DEBUG_PRINTF("pthread_mutex_unlock() returned error (%i).", pthread_retval);

			return(-1);
		}
	} else {
		if (cackey_args.UnlockMutex) {
			custom_retval = cackey_args.UnlockMutex(mutex);

			if (custom_retval != CKR_OK) {
				CACKEY_DEBUG_PRINTF("cackey_args.UnlockMutex() returned error (%li).", (long) custom_retval);

				return(-1);
			}
		}
	}

	CACKEY_DEBUG_PRINTF("Returning sucessfully (0)");

	return(0);
}

static CK_ATTRIBUTE_PTR cackey_get_attributes(CK_OBJECT_CLASS objectclass, struct cackey_pcsc_identity *identity, unsigned long identity_num, CK_ULONG_PTR pulCount) {
	static CK_BBOOL ck_true = 1;
	static CK_BBOOL ck_false = 0;
	static CK_TRUST ck_trusted = CK_TRUSTED_DELEGATOR;
	CK_ULONG numattrs = 0, retval_count;
	CK_ATTRIBUTE_TYPE curr_attr_type;
	CK_ATTRIBUTE curr_attr, *retval;
	CK_VOID_PTR pValue;
	CK_ULONG ulValueLen;
	CK_OBJECT_CLASS ck_object_class;
	CK_CERTIFICATE_TYPE ck_certificate_type;
	CK_KEY_TYPE ck_key_type;
	CK_UTF8CHAR ucTmpBuf[1024];
	SHA1Context sha1_ctx;
	MD5_CTX md5_ctx;
	uint8_t sha1_hash[SHA1HashSize];
	uint8_t md5_hash[MD5HashSize];
	unsigned char *certificate;
	ssize_t certificate_len = -1, x509_read_ret;
	int pValue_free;

	CACKEY_DEBUG_PRINTF("Called (objectClass = %lu, identity_num = %lu).", (unsigned long) objectclass, identity_num);

	*pulCount = 0;

	if (objectclass != CKO_CERTIFICATE && objectclass != CKO_PUBLIC_KEY && objectclass != CKO_PRIVATE_KEY && objectclass != CKO_NETSCAPE_TRUST) {
		CACKEY_DEBUG_PRINTF("Returning 0 objects (NULL), invalid object class");

		return(NULL);
	}

	/* Get Cert */
	if (identity == NULL) {
		CACKEY_DEBUG_PRINTF("Returning 0 objects (NULL), invalid identiy provided");

		return(NULL);
	}

	certificate = identity->certificate;
	certificate_len = identity->certificate_len;

	if (certificate_len == -1 || certificate == NULL) {
		CACKEY_DEBUG_PRINTF("Returning 0 objects (NULL), this identity does not have an X.509 certificate associated with it and will not work");

		return(NULL);
	}

	/* Verify that certificate is ASN.1 encoded X.509 certificate */
	if (x509_to_serial(certificate, certificate_len, NULL) < 0) {
		CACKEY_DEBUG_PRINTF("Returning 0 objects (NULL), the X.509 certificate associated with this identity is not valid");

		return(NULL);
	}

	retval_count = 64;
	retval = malloc(retval_count * sizeof(*retval));

	for (curr_attr_type = 0; curr_attr_type < 0xce5363bf; curr_attr_type++) {
		if (curr_attr_type == 0x800) {
			curr_attr_type = 0xce536300;
		}

		pValue_free = 0;
		pValue = NULL;
		ulValueLen = (CK_LONG) -1;

		switch (curr_attr_type) {
			case CKA_CLASS:
				CACKEY_DEBUG_PRINTF("Requesting attribute CKA_CLASS (0x%08lx) ...", (unsigned long) curr_attr_type);

				ck_object_class = objectclass;

				pValue = &ck_object_class;
				ulValueLen = sizeof(ck_object_class);

				CACKEY_DEBUG_PRINTF(" ... returning %lu (%p/%lu)", (unsigned long) *((CK_OBJECT_CLASS *) pValue), pValue, (unsigned long) ulValueLen);

				break;
			case CKA_TOKEN:
				CACKEY_DEBUG_PRINTF("Requesting attribute CKA_TOKEN (0x%08lx) ...", (unsigned long) curr_attr_type);

				pValue = &ck_true;
				ulValueLen = sizeof(ck_true);

				CACKEY_DEBUG_PRINTF(" ... returning %lu (%p/%lu)", (unsigned long) *((CK_BBOOL *) pValue), pValue, (unsigned long) ulValueLen);

				break;
			case CKA_PRIVATE:
				CACKEY_DEBUG_PRINTF("Requesting attribute CKA_PRIVATE (0x%08lx) ...", (unsigned long) curr_attr_type);

				if (objectclass != CKO_NETSCAPE_TRUST) {
					CACKEY_DEBUG_PRINTF(" ... but not getting it because we are not a Netscape trust object");

					break;
				}

				pValue = &ck_false;
				ulValueLen = sizeof(ck_false);

				CACKEY_DEBUG_PRINTF(" ... returning %lu (%p/%lu)", (unsigned long) *((CK_BBOOL *) pValue), pValue, (unsigned long) ulValueLen);

				break;
			case CKA_TRUSTED:
				CACKEY_DEBUG_PRINTF("Requesting attribute CKA_TRUSTED (0x%08lx) ...", (unsigned long) curr_attr_type);

				if (objectclass == CKO_NETSCAPE_TRUST) {
					CACKEY_DEBUG_PRINTF(" ... but not getting it because we are a Netscape trust object");

					break;
				}

				pValue = &ck_true;
				ulValueLen = sizeof(ck_true);

				CACKEY_DEBUG_PRINTF(" ... returning %lu (%p/%lu)", (unsigned long) *((CK_BBOOL *) pValue), pValue, (unsigned long) ulValueLen);

				break;
			case CKA_MODIFIABLE:
				CACKEY_DEBUG_PRINTF("Requesting attribute CKA_MODIFIABLE (0x%08lx) ...", (unsigned long) curr_attr_type);

				pValue = &ck_false;
				ulValueLen = sizeof(ck_false);

				CACKEY_DEBUG_PRINTF(" ... returning %lu (%p/%lu)", (unsigned long) *((CK_BBOOL *) pValue), pValue, (unsigned long) ulValueLen);

				break;
			case CKA_LABEL:
				CACKEY_DEBUG_PRINTF("Requesting attribute CKA_LABEL (0x%08lx) ...", (unsigned long) curr_attr_type);

				/* XXX: Determine name */
				ulValueLen = snprintf((char *) ucTmpBuf, sizeof(ucTmpBuf), "Identity #%lu", (unsigned long) identity_num);
				pValue = ucTmpBuf;

				if (ulValueLen >= sizeof(ucTmpBuf)) {
					ulValueLen = 0;
					pValue = NULL;
				}

				CACKEY_DEBUG_PRINTF(" ... returning (%p/%lu)", pValue, (unsigned long) ulValueLen);

				break;
			case CKA_VALUE:
				CACKEY_DEBUG_PRINTF("Requesting attribute CKA_VALUE (0x%08lx) ...", (unsigned long) curr_attr_type);

				switch (objectclass) {
					case CKO_PRIVATE_KEY:
						CACKEY_DEBUG_PRINTF(" ... but not getting it because we are a private key.");

						break;
					case CKO_NETSCAPE_TRUST:
						CACKEY_DEBUG_PRINTF(" ... but not getting it because we are a Netscape trust object");

						break;
					case CKO_PUBLIC_KEY:
						if (certificate_len >= 0) {
							x509_read_ret = x509_to_pubkey(certificate, certificate_len, &pValue);
							if (x509_read_ret < 0) { 
								pValue = NULL;
							} else {
								ulValueLen = x509_read_ret;
							}
						}

						break;
					case CKO_CERTIFICATE:
						pValue = certificate;
						ulValueLen = certificate_len;

						break;
				}

				CACKEY_DEBUG_PRINTF(" ... returning %p/%lu", pValue, (unsigned long) ulValueLen);

				break;
			case CKA_ISSUER:
				CACKEY_DEBUG_PRINTF("Requesting attribute CKA_ISSUER (0x%08lx) ...", (unsigned long) curr_attr_type);

				if (objectclass != CKO_CERTIFICATE && objectclass != CKO_NETSCAPE_TRUST) {
					CACKEY_DEBUG_PRINTF(" ... but not getting it because we are not a certificate or Netscape trust object");

					break;
				}

				if (certificate_len >= 0) {
					x509_read_ret = x509_to_issuer(certificate, certificate_len, &pValue);
					if (x509_read_ret < 0) {
						pValue = NULL;
					} else {
						ulValueLen = x509_read_ret;
					}
				}

				CACKEY_DEBUG_PRINTF(" ... returning %p/%lu", pValue, (unsigned long) ulValueLen);

				break;
			case CKA_SERIAL_NUMBER:
				CACKEY_DEBUG_PRINTF("Requesting attribute CKA_SERIAL_NUMBER (0x%08lx) ...", (unsigned long) curr_attr_type);

				if (objectclass != CKO_CERTIFICATE && objectclass != CKO_NETSCAPE_TRUST) {
					CACKEY_DEBUG_PRINTF(" ... but not getting it because we are not a certificate or Netscape trust object");

					break;
				}

				if (certificate_len >= 0) {
					x509_read_ret = x509_to_serial(certificate, certificate_len, &pValue);
					if (x509_read_ret < 0) {
						pValue = NULL;
					} else {
						ulValueLen = x509_read_ret;
					}
				}

				CACKEY_DEBUG_PRINTF(" ... returning (%p/%lu)", pValue, (unsigned long) ulValueLen);

				break;
			case CKA_SUBJECT:
				CACKEY_DEBUG_PRINTF("Requesting attribute CKA_SUBJECT (0x%08lx) ...", (unsigned long) curr_attr_type);

				if (objectclass != CKO_CERTIFICATE) {
					CACKEY_DEBUG_PRINTF(" ... but not getting it because we are not a certificate");

					break;
				}

				if (certificate_len >= 0) {
					x509_read_ret = x509_to_subject(certificate, certificate_len, &pValue);
					if (x509_read_ret < 0) {
						pValue = NULL;
					} else {
						ulValueLen = x509_read_ret;
					}
				}

				CACKEY_DEBUG_PRINTF(" ... returning %p/%lu", pValue, (unsigned long) ulValueLen);

				break;
			case CKA_ID:
				CACKEY_DEBUG_PRINTF("Requesting attribute CKA_ID (0x%08lx) ...", (unsigned long) curr_attr_type);

				if (objectclass == CKO_NETSCAPE_TRUST) {
					CACKEY_DEBUG_PRINTF(" ... but not getting it because we are a Netscape trust object");

					break;
				}

				ucTmpBuf[0] = ((identity_num + 1) >> 8) & 0xff;
				ucTmpBuf[1] =  (identity_num + 1) & 0xff;

				pValue = &ucTmpBuf;
				ulValueLen = 2;

				CACKEY_DEBUG_PRINTF(" ... returning %p/%lu", pValue, (unsigned long) ulValueLen);

				break;
			case CKA_CERTIFICATE_TYPE:
				CACKEY_DEBUG_PRINTF("Requesting attribute CKA_CERTIFICATE_TYPE (0x%08lx) ...", (unsigned long) curr_attr_type);

				if (objectclass != CKO_CERTIFICATE) {
					CACKEY_DEBUG_PRINTF(" ... but not getting it because we are not a certificate.");

					break;
				}

				/* We only support one certificate type */
				ck_certificate_type = CKC_X_509;

				pValue = &ck_certificate_type;
				ulValueLen = sizeof(ck_certificate_type);

				CACKEY_DEBUG_PRINTF(" ... returning CKC_X_509 (%lu) (%p/%lu)", (unsigned long) *((CK_CERTIFICATE_TYPE *) pValue), pValue, (unsigned long) ulValueLen);

				break;
			case CKA_KEY_TYPE:
				CACKEY_DEBUG_PRINTF("Requesting attribute CKA_KEY_TYPE (0x%08lx) ...", (unsigned long) curr_attr_type);

				if (objectclass != CKO_PRIVATE_KEY && objectclass != CKO_PUBLIC_KEY) {
					CACKEY_DEBUG_PRINTF(" ... but not getting it because we are not a key.");

					break;
				}

				/* We only support one key type */
				ck_key_type = CKK_RSA;

				pValue = &ck_key_type;
				ulValueLen = sizeof(ck_key_type);

				CACKEY_DEBUG_PRINTF(" ... returning CKK_RSA (%lu) (%p/%lu)", (unsigned long) *((CK_CERTIFICATE_TYPE *) pValue), pValue, (unsigned long) ulValueLen);

				break;
			case CKA_SIGN:
				CACKEY_DEBUG_PRINTF("Requesting attribute CKA_SIGN (0x%08lx) ...", (unsigned long) curr_attr_type);

				if (objectclass == CKO_NETSCAPE_TRUST) {
					CACKEY_DEBUG_PRINTF(" ... but not getting it because we are a Netscape trust object");

					break;
				}

				if (objectclass == CKO_PRIVATE_KEY) {
					pValue = &ck_true;
					ulValueLen = sizeof(ck_true);
				} else {
					pValue = &ck_false;
					ulValueLen = sizeof(ck_false);
				}

				CACKEY_DEBUG_PRINTF(" ... returning %lu (%p/%lu)", (unsigned long) *((CK_BBOOL *) pValue), pValue, (unsigned long) ulValueLen);

				break;
			case CKA_SIGN_RECOVER:
				CACKEY_DEBUG_PRINTF("Requesting attribute CKA_SIGN_RECOVER (0x%08lx) ...", (unsigned long) curr_attr_type);

				if (objectclass == CKO_NETSCAPE_TRUST) {
					CACKEY_DEBUG_PRINTF(" ... but not getting it because we are a Netscape trust object");

					break;
				}

				/* We currently only support "Sign with Appendix" */
				pValue = &ck_false;
				ulValueLen = sizeof(ck_false);

				CACKEY_DEBUG_PRINTF(" ... returning %lu (%p/%lu)", (unsigned long) *((CK_BBOOL *) pValue), pValue, (unsigned long) ulValueLen);

				break;
			case CKA_DECRYPT:
				CACKEY_DEBUG_PRINTF("Requesting attribute CKA_DECRYPT (0x%08lx) ...", (unsigned long) curr_attr_type);

				if (objectclass == CKO_NETSCAPE_TRUST) {
					CACKEY_DEBUG_PRINTF(" ... but not getting it because we are a Netscape trust object");

					break;
				}

				if (objectclass == CKO_PRIVATE_KEY || objectclass == CKO_PUBLIC_KEY) {
					pValue = &ck_true;
					ulValueLen = sizeof(ck_true);
				} else {
					pValue = &ck_false;
					ulValueLen = sizeof(ck_false);
				}

				CACKEY_DEBUG_PRINTF(" ... returning %lu (%p/%lu)", (unsigned long) *((CK_BBOOL *) pValue), pValue, (unsigned long) ulValueLen);

				break;
			case CKA_SENSITIVE:
				CACKEY_DEBUG_PRINTF("Requesting attribute CKA_SENSITIVE (0x%08lx) ...", (unsigned long) curr_attr_type);

				if (objectclass == CKO_NETSCAPE_TRUST) {
					CACKEY_DEBUG_PRINTF(" ... but not getting it because we are a Netscape trust object");

					break;
				}

				if (objectclass == CKO_PRIVATE_KEY) {
					pValue = &ck_true;
					ulValueLen = sizeof(ck_true);
				} else {
					pValue = &ck_false;
					ulValueLen = sizeof(ck_false);
				}

				CACKEY_DEBUG_PRINTF(" ... returning %lu (%p/%lu)", (unsigned long) *((CK_BBOOL *) pValue), pValue, (unsigned long) ulValueLen);

				break;
			case CKA_EXTRACTABLE:
				CACKEY_DEBUG_PRINTF("Requesting attribute CKA_EXTRACTABLE (0x%08lx) ...", (unsigned long) curr_attr_type);

				if (objectclass == CKO_NETSCAPE_TRUST) {
					CACKEY_DEBUG_PRINTF(" ... but not getting it because we are a Netscape trust object");

					break;
				}

				if (objectclass == CKO_PRIVATE_KEY) {
					pValue = &ck_false;
					ulValueLen = sizeof(ck_true);
				} else {
					pValue = &ck_true;
					ulValueLen = sizeof(ck_false);
				}

				CACKEY_DEBUG_PRINTF(" ... returning %lu (%p/%lu)", (unsigned long) *((CK_BBOOL *) pValue), pValue, (unsigned long) ulValueLen);

				break;
			case CKA_MODULUS:
				CACKEY_DEBUG_PRINTF("Requesting attribute CKA_MODULUS (0x%08lx) ...", (unsigned long) curr_attr_type);

				if (objectclass == CKO_NETSCAPE_TRUST) {
					CACKEY_DEBUG_PRINTF(" ... but not getting it because we are a Netscape trust object");

					break;
				}

				if (certificate_len >= 0) {
					x509_read_ret = x509_to_modulus(certificate, certificate_len, &pValue);
					if (x509_read_ret < 0) {
						pValue = NULL;
					} else {
						ulValueLen = x509_read_ret;
					}
				}

				CACKEY_DEBUG_PRINTF(" ... returning (%p/%lu)", pValue, (unsigned long) ulValueLen);

				break;
			case CKA_PUBLIC_EXPONENT:
				CACKEY_DEBUG_PRINTF("Requesting attribute CKA_PUBLIC_EXPONENT (0x%08lx) ...", (unsigned long) curr_attr_type);

				if (objectclass == CKO_NETSCAPE_TRUST) {
					CACKEY_DEBUG_PRINTF(" ... but not getting it because we are a Netscape trust object");

					break;
				}

				if (certificate_len >= 0) {
					x509_read_ret = x509_to_exponent(certificate, certificate_len, &pValue);
					if (x509_read_ret < 0) {
						pValue = NULL;
					} else {
						ulValueLen = x509_read_ret;
					}
				}

				CACKEY_DEBUG_PRINTF(" ... returning (%p/%lu)", pValue, (unsigned long) ulValueLen);

				break;
			case CKA_TRUST_DIGITAL_SIGNATURE:
			case CKA_TRUST_NON_REPUDIATION:
			case CKA_TRUST_KEY_ENCIPHERMENT:
			case CKA_TRUST_DATA_ENCIPHERMENT:
			case CKA_TRUST_KEY_AGREEMENT:
			case CKA_TRUST_KEY_CERT_SIGN:
			case CKA_TRUST_CRL_SIGN:
			case CKA_TRUST_SERVER_AUTH:
			case CKA_TRUST_CLIENT_AUTH:
			case CKA_TRUST_CODE_SIGNING:
			case CKA_TRUST_EMAIL_PROTECTION:
				CACKEY_DEBUG_PRINTF("Requesting attribute CKA_TRUST_... (0x%08lx) ...", (unsigned long) curr_attr_type);

				pValue = &ck_trusted;
				ulValueLen = sizeof(ck_trusted);

				CACKEY_DEBUG_PRINTF(" ... returning %lu (%p/%lu)", (unsigned long) *((CK_TRUST *) pValue), pValue, (unsigned long) ulValueLen);

				break;
			case CKA_CERT_SHA1_HASH:
				CACKEY_DEBUG_PRINTF("Requesting attribute CKA_CERT_SHA1_HASH (0x%08lx) ...", (unsigned long) curr_attr_type);

				if (objectclass != CKO_NETSCAPE_TRUST) {
					CACKEY_DEBUG_PRINTF(" ... but not getting it because we are not a Netscape trust object");

					break;
				}

				SHA1Reset(&sha1_ctx);
				SHA1Input(&sha1_ctx, certificate, certificate_len);
				SHA1Result(&sha1_ctx, sha1_hash);

				pValue = sha1_hash;
				ulValueLen = sizeof(sha1_hash);

				CACKEY_DEBUG_PRINTF(" ... returning %p/%lu", pValue, (unsigned long) ulValueLen);

				break;
			case CKA_CERT_MD5_HASH:
				CACKEY_DEBUG_PRINTF("Requesting attribute CKA_CERT_MD5_HASH (0x%08lx) ...", (unsigned long) curr_attr_type);

				if (objectclass != CKO_NETSCAPE_TRUST) {
					CACKEY_DEBUG_PRINTF(" ... but not getting it because we are not a Netscape trust object");

					break;
				}

				MD5Init(&md5_ctx);
				MD5Update(&md5_ctx, certificate, certificate_len);
				MD5Final(md5_hash, &md5_ctx);

				pValue = md5_hash;
				ulValueLen = sizeof(md5_hash);

				CACKEY_DEBUG_PRINTF(" ... returning %p/%lu", pValue, (unsigned long) ulValueLen);

				break;
			default:
				pValue = NULL;
				ulValueLen = (CK_LONG) -1;
				break;
		}

		if (((CK_LONG) ulValueLen) != ((CK_LONG) -1)) {
			/* Push curr_attr onto the stack */
			curr_attr.type = curr_attr_type;
			curr_attr.ulValueLen = ulValueLen;

			curr_attr.pValue = malloc(curr_attr.ulValueLen);
			memcpy(curr_attr.pValue, pValue, curr_attr.ulValueLen);

			if (pValue_free && pValue) {
				free(pValue);
			}

			if (numattrs >= retval_count) {
				retval = realloc(retval, retval_count * sizeof(*retval));
			}

			memcpy(&retval[numattrs], &curr_attr, sizeof(curr_attr));
			numattrs++;
		}
	}

	if (numattrs != 0) {
		retval_count = numattrs;
		retval = realloc(retval, retval_count * sizeof(*retval));
	} else {
		free(retval);

		retval = NULL;
	}

	*pulCount = numattrs;

	CACKEY_DEBUG_PRINTF("Returning %lu objects (%p).", numattrs, (void *) retval);

	return(retval);
}

static void cackey_free_identities(struct cackey_identity *identities, unsigned long identities_count) {
	CK_ATTRIBUTE *curr_attr;
	unsigned long id_idx, attr_idx;

	if (identities == NULL || identities_count == 0) {
		return;
	}

	for (id_idx = 0; id_idx < identities_count; id_idx++) {
		if (identities[id_idx].attributes) {
			for (attr_idx = 0; attr_idx < identities[id_idx].attributes_count; attr_idx++) {
				curr_attr = &identities[id_idx].attributes[attr_idx];

				if (curr_attr->pValue) {
					free(curr_attr->pValue);
				}
			}

			if (identities[id_idx].attributes) {
				free(identities[id_idx].attributes);
			}

			cackey_free_certs(identities[id_idx].pcsc_identity, 1, 1);
		}
	}

	free(identities);
}

static unsigned long cackey_read_dod_identities(struct cackey_identity *identities, unsigned long num_dod_certs) {
	unsigned long cert_idx, id_idx = 0;

	if (identities == NULL) {
		return(num_dod_certs * 3);
	}

	for (cert_idx = 0; cert_idx < num_dod_certs; cert_idx++) {
		identities[id_idx].pcsc_identity = NULL;
		identities[id_idx].attributes = cackey_get_attributes(CKO_CERTIFICATE, &extra_certs[cert_idx], 0xf000 | cert_idx, &identities[id_idx].attributes_count);
		id_idx++;

		identities[id_idx].pcsc_identity = NULL;
		identities[id_idx].attributes = cackey_get_attributes(CKO_PUBLIC_KEY, &extra_certs[cert_idx], 0xf000 | cert_idx, &identities[id_idx].attributes_count);
		id_idx++;

		identities[id_idx].pcsc_identity = NULL;
		identities[id_idx].attributes = cackey_get_attributes(CKO_NETSCAPE_TRUST, &extra_certs[cert_idx], 0xf000 | cert_idx, &identities[id_idx].attributes_count);
		id_idx++;
	}

	return(id_idx);
}

static struct cackey_identity *cackey_read_identities(struct cackey_slot *slot, unsigned long *ids_found) {
	struct cackey_pcsc_identity *pcsc_identities;
	struct cackey_identity *identities;
	unsigned long num_ids, id_idx, curr_id_type;
	unsigned long num_certs, num_dod_certs, cert_idx;
	int include_extra_certs = 0;

	CACKEY_DEBUG_PRINTF("Called.");

	if (ids_found == NULL) {
		CACKEY_DEBUG_PRINTF("Error.  ids_found is NULL");

		return(NULL);
	}

#ifdef CACKEY_CARD_SLOT_INCLUDE_EXTRA_CERTS
	include_extra_certs = 1;
#endif

	if (getenv("CACKEY_DOD_CERTS_ON_HW_SLOTS") != NULL) {
		include_extra_certs = 1;
	}

	if (getenv("CACKEY_NO_DOD_CERTS_ON_HW_SLOTS") != NULL) {
		include_extra_certs = 0;
	}

	if (getenv("CACKEY_NO_EXTRA_CERTS") != NULL) {
		num_dod_certs = 0;
	} else {
		num_dod_certs = sizeof(extra_certs) / sizeof(extra_certs[0]);
	}

	if (slot->internal) {
		num_ids = cackey_read_dod_identities(NULL, num_dod_certs);

		if (num_ids != 0) {
			identities = malloc(num_ids * sizeof(*identities));

			cackey_read_dod_identities(identities, num_dod_certs);
		} else {
			identities = NULL;
		}

		*ids_found = num_ids;

		return(identities);
	}

	pcsc_identities = cackey_read_certs(slot, NULL, &num_certs);
	if (pcsc_identities != NULL) {
		/* Convert number of Certs to number of objects */
		num_ids = (CKO_PRIVATE_KEY - CKO_CERTIFICATE + 1) * num_certs;

		if (include_extra_certs) {
			num_ids += cackey_read_dod_identities(NULL, num_dod_certs);
		}

		identities = malloc(num_ids * sizeof(*identities));

		/* Add certificates, public keys, and private keys from the smartcard */
		id_idx = 0;
		for (cert_idx = 0; cert_idx < num_certs; cert_idx++) {
			for (curr_id_type = CKO_CERTIFICATE; curr_id_type <= CKO_PRIVATE_KEY; curr_id_type++) {
				identities[id_idx].attributes = cackey_get_attributes(curr_id_type, &pcsc_identities[cert_idx], cert_idx, &identities[id_idx].attributes_count);

				identities[id_idx].pcsc_identity = malloc(sizeof(*identities[id_idx].pcsc_identity));
				memcpy(identities[id_idx].pcsc_identity, &pcsc_identities[cert_idx], sizeof(*identities[id_idx].pcsc_identity));

				identities[id_idx].pcsc_identity->certificate = malloc(pcsc_identities[cert_idx].certificate_len);
				memcpy(identities[id_idx].pcsc_identity->certificate, pcsc_identities[cert_idx].certificate, pcsc_identities[cert_idx].certificate_len);

				id_idx++;
			}
		}

		if (include_extra_certs) {
			CACKEY_DEBUG_PRINTF("Including DoD Certificates on hardware slot");

			cackey_read_dod_identities(identities + id_idx, num_dod_certs);
		}

		cackey_free_certs(pcsc_identities, num_certs, 1);

		*ids_found = num_ids;

		return(identities);
	}


	*ids_found = 0;
	return(NULL);
}

CK_DEFINE_FUNCTION(CK_RV, C_Initialize)(CK_VOID_PTR pInitArgs) {
	CK_C_INITIALIZE_ARGS CK_PTR args;
	uint32_t idx, highest_slot;
	int mutex_init_ret;

	CACKEY_DEBUG_PRINTF("Called.");

	if (cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Already initialized.");

		return(CKR_CRYPTOKI_ALREADY_INITIALIZED);
	}

	if (pInitArgs != NULL) {
		args = pInitArgs;
		memcpy(&cackey_args, args, sizeof(cackey_args));

		if (args->CreateMutex == NULL || args->DestroyMutex == NULL || args->LockMutex == NULL || args->UnlockMutex == NULL) {
			if (args->CreateMutex != NULL || args->DestroyMutex != NULL || args->LockMutex != NULL || args->UnlockMutex != NULL) {
				CACKEY_DEBUG_PRINTF("Error. Some, but not All threading primitives provided.");

				return(CKR_ARGUMENTS_BAD);
			}
		}
	} else {
		cackey_args.CreateMutex = NULL;
		cackey_args.DestroyMutex = NULL;
		cackey_args.LockMutex = NULL;
		cackey_args.UnlockMutex = NULL;
		cackey_args.flags = 0;
	}

	for (idx = 0; idx < (sizeof(cackey_sessions) / sizeof(cackey_sessions[0])); idx++) {
		cackey_sessions[idx].active = 0;
	}

	for (idx = 0; idx < (sizeof(cackey_slots) / sizeof(cackey_slots[0])); idx++) {
		cackey_slots[idx].active = 0;
		cackey_slots[idx].pcsc_reader = NULL;
		cackey_slots[idx].transaction_depth = 0;
		cackey_slots[idx].transaction_need_hw_lock = 0;
		cackey_slots[idx].slot_reset = 0;
		cackey_slots[idx].token_flags = 0;
		cackey_slots[idx].label = NULL;
		cackey_slots[idx].internal = 0;
	}

	if (getenv("CACKEY_NO_EXTRA_CERTS") != NULL) {
		CACKEY_DEBUG_PRINTF("Asked not to include DoD certificates");
	} else {
		highest_slot = (sizeof(cackey_slots) / sizeof(cackey_slots[0])) - 1;

		CACKEY_DEBUG_PRINTF("Including DoD certs in slot %lu", (unsigned long) highest_slot);

		cackey_slots[highest_slot].active = 1;
		cackey_slots[highest_slot].internal = 1;
		cackey_slots[highest_slot].label = (unsigned char *) "DoD Certificates";
		cackey_slots[highest_slot].pcsc_reader = "CACKey";
		cackey_slots[highest_slot].token_flags = 0;
	}

	cackey_initialized = 1;

	if (!cackey_biglock_init) {
		mutex_init_ret = cackey_mutex_create(&cackey_biglock);

		if (mutex_init_ret != 0) {
			CACKEY_DEBUG_PRINTF("Error.  Mutex initialization failed.");

			return(CKR_CANT_LOCK);
		}

		cackey_biglock_init = 1;
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i)", CKR_OK);

	return(CKR_OK);
}

CK_DEFINE_FUNCTION(CK_RV, C_Finalize)(CK_VOID_PTR pReserved) {
	uint32_t idx;

	CACKEY_DEBUG_PRINTF("Called.");

	if (pReserved != NULL) {
		CACKEY_DEBUG_PRINTF("Error. pReserved is not NULL.");

		return(CKR_ARGUMENTS_BAD);
	}

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	for (idx = 0; idx < (sizeof(cackey_sessions) / sizeof(cackey_sessions[0])); idx++) {
		if (cackey_sessions[idx].active) {
			C_CloseSession(idx);
		}
	}

	cackey_slots_disconnect_all();

	for (idx = 0; idx < (sizeof(cackey_slots) / sizeof(cackey_slots[0])); idx++) {
		if (cackey_slots[idx].internal) {
			continue;
		}

		if (cackey_slots[idx].pcsc_reader) {
			free(cackey_slots[idx].pcsc_reader);
		}

		if (cackey_slots[idx].cached_certs) {
			cackey_free_certs(cackey_slots[idx].cached_certs, cackey_slots[idx].cached_certs_count, 1);

			cackey_slots[idx].cached_certs = NULL;
		}
	}

	cackey_pcsc_disconnect();

	cackey_initialized = 0;

	CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i)", CKR_OK);

	return(CKR_OK);
}

CK_DEFINE_FUNCTION(CK_RV, C_GetInfo)(CK_INFO_PTR pInfo) {
	static CK_UTF8CHAR manufacturerID[] = "U.S. Government";
	static CK_UTF8CHAR libraryDescription[] = "CACKey";

	CACKEY_DEBUG_PRINTF("Called.");

	if (pInfo == NULL) {
		CACKEY_DEBUG_PRINTF("Error. pInfo is NULL.");

		return(CKR_ARGUMENTS_BAD);
	}

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	pInfo->cryptokiVersion.major = ((CACKEY_CRYPTOKI_VERSION_CODE) >> 16) & 0xff;
	pInfo->cryptokiVersion.minor = ((CACKEY_CRYPTOKI_VERSION_CODE) >> 8) & 0xff;

	memset(pInfo->manufacturerID, ' ', sizeof(pInfo->manufacturerID));
	memcpy(pInfo->manufacturerID, manufacturerID, sizeof(manufacturerID) - 1);

	pInfo->flags = 0x00;

	memset(pInfo->libraryDescription, ' ', sizeof(pInfo->libraryDescription));
	memcpy(pInfo->libraryDescription, libraryDescription, sizeof(libraryDescription) - 1);

	pInfo->libraryVersion.major = (cackey_getversion() >> 16) & 0xff;
	pInfo->libraryVersion.minor = (cackey_getversion() >> 8) & 0xff;

	CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i)", CKR_OK);

	return(CKR_OK);
}

/*
 * Process list of readers, and create mapping between reader name and slot ID
 */
CK_DEFINE_FUNCTION(CK_RV, C_GetSlotList)(CK_BBOOL tokenPresent, CK_SLOT_ID_PTR pSlotList, CK_ULONG_PTR pulCount) {
	static int first_call = 1;
	int mutex_retval;
	int pcsc_connect_ret;
	CK_ULONG count, slot_count = 0, currslot, slot_idx;
	char *pcsc_readers, *pcsc_readers_s, *pcsc_readers_e;
	DWORD pcsc_readers_len;
	LONG scard_listreaders_ret;
	size_t curr_reader_len;
	int slot_reset;

	CACKEY_DEBUG_PRINTF("Called.");

	if (pulCount == NULL) {
		CACKEY_DEBUG_PRINTF("Error. pulCount is NULL.");

		return(CKR_ARGUMENTS_BAD);
	}

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	mutex_retval = cackey_mutex_lock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Locking failed.");

		return(CKR_GENERAL_ERROR);
	}

	/* Clear list of slots */
	slot_reset = 0;
	if (pSlotList) {
		if (first_call) {
			first_call = 0;

			slot_reset = 1;
		}

		/* If any of the slots have been reset then purge all information and check again */
		for (currslot = 0; currslot < (sizeof(cackey_slots) / sizeof(cackey_slots[0])); currslot++) {
			if (cackey_slots[currslot].internal) {
				continue;
			}

			if (!cackey_slots[currslot].active) {
				continue;
			}

			if (cackey_slots[currslot].slot_reset) {
				slot_reset = 1;

				break;
			}
		}

		if (slot_reset) {
			CACKEY_DEBUG_PRINTF("Purging all slot information.");

			/* Only update the list of slots if we are actually being supply the slot information */
			cackey_slots_disconnect_all();

			for (currslot = 0; currslot < (sizeof(cackey_slots) / sizeof(cackey_slots[0])); currslot++) {
				if (cackey_slots[currslot].internal) {
					continue;
				}

				if (cackey_slots[currslot].pcsc_reader) {
					free(cackey_slots[currslot].pcsc_reader);

					cackey_slots[currslot].pcsc_reader = NULL;
				}

				if (cackey_slots[currslot].label) {
					free(cackey_slots[currslot].label);

					cackey_slots[currslot].label = NULL;
				}

				cackey_slots[currslot].active = 0;
			}
		} else {
			
		}
	}

	/* Determine list of readers */
	pcsc_connect_ret = cackey_pcsc_connect();
	if (pcsc_connect_ret != CACKEY_PCSC_S_OK) {
		CACKEY_DEBUG_PRINTF("Connection to PC/SC failed, assuming no slots");

		slot_count = 0;
	} else {
		pcsc_readers_len = 0;

		scard_listreaders_ret = SCardListReaders(*cackey_pcsc_handle, NULL, NULL, &pcsc_readers_len);

		if (scard_listreaders_ret == SCARD_F_COMM_ERROR) {
			CACKEY_DEBUG_PRINTF("Error. SCardListReaders() returned SCARD_F_COMM_ERROR, assuming Connection to PC/SC went away. Reconnecting.");

			cackey_pcsc_disconnect();
			cackey_pcsc_connect();

			CACKEY_DEBUG_PRINTF("Trying SCardListReaders() again");
			scard_listreaders_ret = SCardListReaders(*cackey_pcsc_handle, NULL, NULL, &pcsc_readers_len);
		}

		if (scard_listreaders_ret == SCARD_S_SUCCESS && pcsc_readers_len != 0) {
			pcsc_readers = malloc(pcsc_readers_len);
			pcsc_readers_s = pcsc_readers;

			scard_listreaders_ret = SCardListReaders(*cackey_pcsc_handle, NULL, pcsc_readers, &pcsc_readers_len);
			if (scard_listreaders_ret == SCARD_S_SUCCESS) {
				pcsc_readers_e = pcsc_readers + pcsc_readers_len;

				/* Start with Slot ID 1, to avoid a bug in GDM on RHEL */
				/* Bug 594911: https://bugzilla.redhat.com/show_bug.cgi?id=594911 */
				currslot = 1;
				slot_count = 0;
				while (pcsc_readers < pcsc_readers_e) {
					/* Find next available slot */
					for (; currslot < (sizeof(cackey_slots) / sizeof(cackey_slots[0])); currslot++) {
						if (!cackey_slots[currslot].active) {
							break;
						}
					}

					curr_reader_len = strlen(pcsc_readers);

					if ((pcsc_readers + curr_reader_len) > pcsc_readers_e) {
						break;
					}

					if (curr_reader_len == 0) {
						break;
					}

					if (currslot >= (sizeof(cackey_slots) / sizeof(cackey_slots[0]))) {
						CACKEY_DEBUG_PRINTF("Found more readers than slots are available!");

						break;
					}

					CACKEY_DEBUG_PRINTF("Found reader: %s", pcsc_readers);

					/* Only update the list of slots if we are actually being asked supply the slot information */
					if (pSlotList) {
						if (slot_reset) {
							cackey_slots[currslot].active = 1;
							cackey_slots[currslot].internal = 0;
							cackey_slots[currslot].pcsc_reader = strdup(pcsc_readers);
							cackey_slots[currslot].pcsc_card_connected = 0;
							cackey_slots[currslot].transaction_depth = 0;
							cackey_slots[currslot].transaction_need_hw_lock = 0;
							cackey_slots[currslot].token_flags = CKF_LOGIN_REQUIRED;
							cackey_slots[currslot].label = NULL;

							cackey_mark_slot_reset(&cackey_slots[currslot]);
						}
					} else {
						/* Artificially increase the number of active slots by what will become active */
						slot_count++;
					}
					currslot++;

					pcsc_readers += curr_reader_len + 1;
				}

				for (currslot = 0; currslot < (sizeof(cackey_slots) / sizeof(cackey_slots[0])); currslot++) {
					if (cackey_slots[currslot].active) {
						CACKEY_DEBUG_PRINTF("Found active slot %lu", (unsigned long) currslot);

						slot_count++;
					}
				}
			} else {
				CACKEY_DEBUG_PRINTF("Second call to SCardListReaders failed, return %s/%li", CACKEY_DEBUG_FUNC_SCARDERR_TO_STR(scard_listreaders_ret), (long) scard_listreaders_ret);
			}

			free(pcsc_readers_s);
		} else {
			CACKEY_DEBUG_PRINTF("First call to SCardListReaders failed, return %s/%li", CACKEY_DEBUG_FUNC_SCARDERR_TO_STR(scard_listreaders_ret), (long) scard_listreaders_ret);
		}
	}

	mutex_retval = cackey_mutex_unlock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Unlocking failed.");

		return(CKR_GENERAL_ERROR);
	}

	if (pSlotList == NULL) {
		*pulCount = slot_count;

		CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i).  Found %lu readers, but not storing IDs (pSlotList == NULL)", CKR_OK, (unsigned long) slot_count);

		return(CKR_OK);
	}

	count = *pulCount;
	if (count < slot_count) {
		CACKEY_DEBUG_PRINTF("Error. User allocated %lu entries, but we have %lu entries.", count, slot_count);

		CACKEY_DEBUG_PRINTF("Returning CKR_BUFFER_TOO_SMALL");

		return(CKR_BUFFER_TOO_SMALL);	
	}

	mutex_retval = cackey_mutex_lock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Locking failed.");

		return(CKR_GENERAL_ERROR);
	}

	slot_idx = 0;
	for (currslot = 0; (currslot < (sizeof(cackey_slots) / sizeof(cackey_slots[0]))); currslot++) {
		if (!cackey_slots[currslot].active) {
			continue;
		}

		if (slot_idx >= count) {
			CACKEY_DEBUG_PRINTF("Error. User allocated %lu entries, but we just tried to write to the %lu index -- ignoring", count, slot_idx);

			continue;
		}

		pSlotList[slot_idx] = currslot;
		slot_idx++;
	}

	mutex_retval = cackey_mutex_unlock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Unlocking failed.");

		return(CKR_GENERAL_ERROR);
	}

	*pulCount = slot_count;

	CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i).  Found %lu readers.", CKR_OK, (unsigned long) slot_count);

	return(CKR_OK);

	tokenPresent = tokenPresent; /* Supress unused variable warning */
}

CK_DEFINE_FUNCTION(CK_RV, C_GetSlotInfo)(CK_SLOT_ID slotID, CK_SLOT_INFO_PTR pInfo) {
	static CK_UTF8CHAR slotDescription[] = "CACKey Slot";
	int mutex_retval;
	int bytes_to_copy;

	CACKEY_DEBUG_PRINTF("Called.");

	if (pInfo == NULL) {
		CACKEY_DEBUG_PRINTF("Error. pInfo is NULL.");

		return(CKR_ARGUMENTS_BAD);
	}

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	if (slotID < 0 || slotID >= (sizeof(cackey_slots) / sizeof(cackey_slots[0]))) {
		CACKEY_DEBUG_PRINTF("Error. Invalid slot requested (%lu), outside of valid range", slotID);

		return(CKR_SLOT_ID_INVALID);
	}

	mutex_retval = cackey_mutex_lock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Locking failed.");

		return(CKR_GENERAL_ERROR);
	}

	if (cackey_slots[slotID].active == 0) {
		CACKEY_DEBUG_PRINTF("Error. Invalid slot requested (%lu), slot not currently active", slotID);

		cackey_mutex_unlock(cackey_biglock);

		return(CKR_SLOT_ID_INVALID);
	}

	pInfo->flags = CKF_HW_SLOT;

	if (!cackey_slots[slotID].internal) {
		pInfo->flags |= CKF_REMOVABLE_DEVICE;
	}

	if (cackey_token_present(&cackey_slots[slotID]) == CACKEY_PCSC_S_TOKENPRESENT) {
		pInfo->flags |= CKF_TOKEN_PRESENT;
	}

	bytes_to_copy = strlen(cackey_slots[slotID].pcsc_reader);
	if (sizeof(pInfo->manufacturerID) < bytes_to_copy) {
		bytes_to_copy = sizeof(pInfo->manufacturerID);
	}
	memcpy(pInfo->manufacturerID, cackey_slots[slotID].pcsc_reader, bytes_to_copy);

	mutex_retval = cackey_mutex_unlock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Unlocking failed.");

		return(CKR_GENERAL_ERROR);
	}

	memset(pInfo->slotDescription, ' ', sizeof(pInfo->slotDescription));
	memcpy(pInfo->slotDescription, slotDescription, sizeof(slotDescription) - 1);

	memset(pInfo->manufacturerID, ' ', sizeof(pInfo->manufacturerID));

	pInfo->hardwareVersion.major = (cackey_getversion() >> 16) & 0xff;
	pInfo->hardwareVersion.minor = (cackey_getversion() >> 8) & 0xff;

	pInfo->firmwareVersion.major = 0x00;
	pInfo->firmwareVersion.minor = 0x00;

	CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i)", CKR_OK);

	return(CKR_OK);
}

CK_DEFINE_FUNCTION(CK_RV, C_GetTokenInfo)(CK_SLOT_ID slotID, CK_TOKEN_INFO_PTR pInfo) {
	static CK_UTF8CHAR manufacturerID[] = "U.S. Government";
	static CK_UTF8CHAR defaultLabel[] = "Unknown Token";
	static CK_UTF8CHAR model[] = "CAC Token";
	struct cackey_pcsc_identity *pcsc_identities;
	unsigned long num_certs;
	ssize_t label_ret;
	int mutex_retval;
	int use_default_label;

	CACKEY_DEBUG_PRINTF("Called.");

	if (pInfo == NULL) {
		CACKEY_DEBUG_PRINTF("Error. pInfo is NULL.");

		return(CKR_ARGUMENTS_BAD);
	}

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	if (slotID < 0 || slotID >= (sizeof(cackey_slots) / sizeof(cackey_slots[0]))) {
		CACKEY_DEBUG_PRINTF("Error. Invalid slot requested (%lu), outside of valid range", slotID);

		return(CKR_SLOT_ID_INVALID);
	}

	mutex_retval = cackey_mutex_lock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Locking failed.");

		return(CKR_GENERAL_ERROR);
	}

	if (cackey_slots[slotID].active == 0) {
		CACKEY_DEBUG_PRINTF("Error. Invalid slot requested (%lu), slot not currently active", slotID);

		cackey_mutex_unlock(cackey_biglock);

		return(CKR_SLOT_ID_INVALID);
	}

	if (cackey_token_present(&cackey_slots[slotID]) != CACKEY_PCSC_S_TOKENPRESENT) {
		CACKEY_DEBUG_PRINTF("No token is present in slotID = %lu", slotID);

		cackey_mutex_unlock(cackey_biglock);

		return(CKR_TOKEN_NOT_PRESENT);
	}

	mutex_retval = cackey_mutex_unlock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Unlocking failed.");

		return(CKR_GENERAL_ERROR);
	}

	/* Determine token label from certificates */
	memset(pInfo->label, ' ', sizeof(pInfo->label));
	use_default_label = 1;

	if (cackey_slots[slotID].label == NULL) {
		pcsc_identities = cackey_read_certs(&cackey_slots[slotID], NULL, &num_certs);
		if (pcsc_identities != NULL) {
			if (num_certs > 0) {
				label_ret = cackey_pcsc_identity_to_label(pcsc_identities, pInfo->label, sizeof(pInfo->label));
				if (label_ret > 0) {
					use_default_label = 0;

					cackey_slots[slotID].label = malloc(sizeof(pInfo->label));

					memcpy(cackey_slots[slotID].label, pInfo->label, sizeof(pInfo->label));
				}
			}

			cackey_free_certs(pcsc_identities, num_certs, 1);
		}
	} else {
		memcpy(pInfo->label, cackey_slots[slotID].label, sizeof(pInfo->label));

		use_default_label = 0;
	}

	if (use_default_label) {
		memcpy(pInfo->label, defaultLabel, sizeof(defaultLabel) - 1);
	}

	memset(pInfo->manufacturerID, ' ', sizeof(pInfo->manufacturerID));
	memcpy(pInfo->manufacturerID, manufacturerID, sizeof(manufacturerID) - 1);

	memset(pInfo->model, ' ', sizeof(pInfo->model));
	memcpy(pInfo->model, model, sizeof(model) - 1);

	memset(pInfo->serialNumber, ' ', sizeof(pInfo->serialNumber));

	memset(pInfo->utcTime, ' ', sizeof(pInfo->utcTime));

	pInfo->hardwareVersion.major = (cackey_getversion() >> 16) & 0xff;
	pInfo->hardwareVersion.minor = (cackey_getversion() >> 8) & 0xff;

	pInfo->firmwareVersion.major = 0x00;
	pInfo->firmwareVersion.minor = 0x00;

	pInfo->flags = CKF_WRITE_PROTECTED | CKF_USER_PIN_INITIALIZED | CKF_TOKEN_INITIALIZED | cackey_slots[slotID].token_flags;

	pInfo->ulMaxSessionCount = (sizeof(cackey_sessions) / sizeof(cackey_sessions[0])) - 1;
	pInfo->ulSessionCount = CK_UNAVAILABLE_INFORMATION;
	pInfo->ulMaxRwSessionCount = 0;
	pInfo->ulRwSessionCount = CK_UNAVAILABLE_INFORMATION;
	pInfo->ulMaxPinLen = 128;
	pInfo->ulMinPinLen = 0;
	pInfo->ulTotalPublicMemory = CK_UNAVAILABLE_INFORMATION;
	pInfo->ulFreePublicMemory = CK_UNAVAILABLE_INFORMATION;
	pInfo->ulTotalPrivateMemory = CK_UNAVAILABLE_INFORMATION;
	pInfo->ulFreePrivateMemory = CK_UNAVAILABLE_INFORMATION;

	CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i)", CKR_OK);

	return(CKR_OK);
}

CK_DEFINE_FUNCTION(CK_RV, C_WaitForSlotEvent)(CK_FLAGS flags, CK_SLOT_ID_PTR pSlotID, CK_VOID_PTR pReserved) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (pReserved != NULL) {
		CACKEY_DEBUG_PRINTF("Error. pReserved is not NULL.");

		return(CKR_ARGUMENTS_BAD);
	}

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	/* XXX: TODO: Implement this... */
	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_GetMechanismList)(CK_SLOT_ID slotID, CK_MECHANISM_TYPE_PTR pMechanismList, CK_ULONG_PTR pulCount) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	if (pulCount == NULL) {
		CACKEY_DEBUG_PRINTF("Error.  pulCount is NULL.");

		return(CKR_ARGUMENTS_BAD);
	}

	if (pMechanismList == NULL) {
		*pulCount = 1;

		CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i)", CKR_OK);

		return(CKR_OK);
	}

	if (*pulCount < 1) {
		CACKEY_DEBUG_PRINTF("Error.  Buffer too small.");

		return(CKR_BUFFER_TOO_SMALL);
	}

	pMechanismList[0] = CKM_RSA_PKCS;
	*pulCount = 1;

	CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i)", CKR_OK);

	return(CKR_OK);
}

CK_DEFINE_FUNCTION(CK_RV, C_GetMechanismInfo)(CK_SLOT_ID slotID, CK_MECHANISM_TYPE type, CK_MECHANISM_INFO_PTR pInfo) {
	int mutex_retval;

	CACKEY_DEBUG_PRINTF("Called.");

	if (pInfo == NULL) {
		CACKEY_DEBUG_PRINTF("Error. pInfo is NULL.");

		return(CKR_ARGUMENTS_BAD);
	}

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	if (slotID < 0 || slotID >= (sizeof(cackey_slots) / sizeof(cackey_slots[0]))) {
		CACKEY_DEBUG_PRINTF("Error. Invalid slot requested (%lu), outside of valid range", slotID);

		return(CKR_SLOT_ID_INVALID);
	}

	mutex_retval = cackey_mutex_lock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Locking failed.");

		return(CKR_GENERAL_ERROR);
	}

	if (cackey_slots[slotID].active == 0) {
		CACKEY_DEBUG_PRINTF("Error. Invalid slot requested (%lu), slot not currently active", slotID);

		cackey_mutex_unlock(cackey_biglock);

		return(CKR_SLOT_ID_INVALID);
	}

	mutex_retval = cackey_mutex_unlock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Unlocking failed.");

		return(CKR_GENERAL_ERROR);
	}

	switch (type) {
		case CKM_RSA_PKCS:
			pInfo->ulMinKeySize = 512;
			pInfo->ulMaxKeySize = 8192;
			pInfo->flags = CKF_HW | CKF_ENCRYPT | CKF_DECRYPT | CKF_SIGN | CKF_VERIFY;
			break;
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i)", CKR_OK);

	return(CKR_OK);
}

/* We don't support this method. */
CK_DEFINE_FUNCTION(CK_RV, C_InitToken)(CK_SLOT_ID slotID, CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen, CK_UTF8CHAR_PTR pLabel) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_TOKEN_WRITE_PROTECTED (%i)", CKR_TOKEN_WRITE_PROTECTED);

	return(CKR_TOKEN_WRITE_PROTECTED);
}

/* We don't support this method. */
CK_DEFINE_FUNCTION(CK_RV, C_InitPIN)(CK_SESSION_HANDLE hSession, CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_TOKEN_WRITE_PROTECTED (%i)", CKR_TOKEN_WRITE_PROTECTED);

	return(CKR_TOKEN_WRITE_PROTECTED);
}

/* We don't support this method. */
CK_DEFINE_FUNCTION(CK_RV, C_SetPIN)(CK_SESSION_HANDLE hSession, CK_UTF8CHAR_PTR pOldPin, CK_ULONG ulOldPinLen, CK_UTF8CHAR_PTR pNewPin, CK_ULONG ulNewPinLen) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_OpenSession)(CK_SLOT_ID slotID, CK_FLAGS flags, CK_VOID_PTR pApplication, CK_NOTIFY notify, CK_SESSION_HANDLE_PTR phSession) {
	unsigned long idx;
	int mutex_retval;
	int found_session = 0;

	CACKEY_DEBUG_PRINTF("Called.");

	if ((flags & CKF_SERIAL_SESSION) != CKF_SERIAL_SESSION) {
		return(CKR_SESSION_PARALLEL_NOT_SUPPORTED);
	}

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	if (slotID < 0 || slotID >= (sizeof(cackey_slots) / sizeof(cackey_slots[0]))) {
		CACKEY_DEBUG_PRINTF("Error. Invalid slot requested (%lu), outside of valid range", slotID);

		return(CKR_SLOT_ID_INVALID);
	}

	mutex_retval = cackey_mutex_lock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Locking failed.");

		return(CKR_GENERAL_ERROR);
	}

	if (cackey_slots[slotID].active == 0) {
		CACKEY_DEBUG_PRINTF("Error. Invalid slot requested (%lu), slot not currently active", slotID);

		cackey_mutex_unlock(cackey_biglock);

		return(CKR_SLOT_ID_INVALID);
	}

	/* Verify that the card is actually in the slot. */
	/* XXX: Check to make sure this is in the PKCS#11 specification */
	if (cackey_token_present(&cackey_slots[slotID]) != CACKEY_PCSC_S_TOKENPRESENT) {
		CACKEY_DEBUG_PRINTF("Error.  Card not present.  Returning CKR_DEVICE_REMOVED");

		cackey_mutex_unlock(cackey_biglock);

		return(CKR_DEVICE_REMOVED);
	}

	for (idx = 1; idx < (sizeof(cackey_sessions) / sizeof(cackey_sessions[0])); idx++) {
		if (!cackey_sessions[idx].active) {
			found_session = 1;

			*phSession = idx;

			cackey_sessions[idx].active = 1;
			cackey_sessions[idx].slotID = slotID;
			cackey_sessions[idx].state = CKS_RO_PUBLIC_SESSION;
			cackey_sessions[idx].flags = flags;
			cackey_sessions[idx].ulDeviceError = 0;
			cackey_sessions[idx].pApplication = pApplication;
			cackey_sessions[idx].Notify = notify;

			cackey_sessions[idx].identities = NULL;
			cackey_sessions[idx].identities_count = 0;

			cackey_sessions[idx].search_active = 0;

			cackey_sessions[idx].sign_active = 0;

			cackey_sessions[idx].decrypt_active = 0;

			cackey_sessions[idx].identities = cackey_read_identities(&cackey_slots[slotID], &cackey_sessions[idx].identities_count);


			break;
		}
	}

	mutex_retval = cackey_mutex_unlock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Unlocking failed.");

		return(CKR_GENERAL_ERROR);
	}

	if (!found_session) {
		CACKEY_DEBUG_PRINTF("Returning CKR_SESSION_COUNT (%i)", CKR_SESSION_COUNT);

		return(CKR_SESSION_COUNT);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i)", CKR_OK);

	return(CKR_OK);
}

CK_DEFINE_FUNCTION(CK_RV, C_CloseSession)(CK_SESSION_HANDLE hSession) {
	int mutex_retval;

	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	if (hSession == 0 || hSession >= (sizeof(cackey_sessions) / sizeof(cackey_sessions[0]))) {
		CACKEY_DEBUG_PRINTF("Error.  Session out of range.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	mutex_retval = cackey_mutex_lock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Locking failed.");

		return(CKR_GENERAL_ERROR);
	}

	if (!cackey_sessions[hSession].active) {
		cackey_mutex_unlock(cackey_biglock);

		CACKEY_DEBUG_PRINTF("Error.  Session not active.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	cackey_sessions[hSession].active = 0;
	cackey_free_identities(cackey_sessions[hSession].identities, cackey_sessions[hSession].identities_count);

	mutex_retval = cackey_mutex_unlock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Unlocking failed.");

		return(CKR_GENERAL_ERROR);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i)", CKR_OK);

	return(CKR_OK);
}

CK_DEFINE_FUNCTION(CK_RV, C_CloseAllSessions)(CK_SLOT_ID slotID) {
	uint32_t idx;
	int mutex_retval;

	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	if (slotID < 0 || slotID >= (sizeof(cackey_slots) / sizeof(cackey_slots[0]))) {
		CACKEY_DEBUG_PRINTF("Error. Invalid slot requested (%lu), outside of valid range", slotID);

		return(CKR_SLOT_ID_INVALID);
	}

	mutex_retval = cackey_mutex_lock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Locking failed.");

		return(CKR_GENERAL_ERROR);
	}

	if (cackey_slots[slotID].active == 0) {
		CACKEY_DEBUG_PRINTF("Error. Invalid slot requested (%lu), slot not currently active", slotID);

		cackey_mutex_unlock(cackey_biglock);

		return(CKR_SLOT_ID_INVALID);
	}

	for (idx = 0; idx < (sizeof(cackey_sessions) / sizeof(cackey_sessions[0])); idx++) {
		if (cackey_sessions[idx].active) {
			if (cackey_sessions[idx].slotID != slotID) {
				continue;
			}

			cackey_mutex_unlock(cackey_biglock);
			C_CloseSession(idx);
			cackey_mutex_lock(cackey_biglock);
		}
	}

	mutex_retval = cackey_mutex_unlock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Unlocking failed.");

		return(CKR_GENERAL_ERROR);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i)", CKR_OK);

	return(CKR_OK);
}

CK_DEFINE_FUNCTION(CK_RV, C_GetSessionInfo)(CK_SESSION_HANDLE hSession, CK_SESSION_INFO_PTR pInfo) {
	int mutex_retval;

	CACKEY_DEBUG_PRINTF("Called.");

	if (pInfo == NULL) {
		CACKEY_DEBUG_PRINTF("Error. pInfo is NULL.");

		return(CKR_ARGUMENTS_BAD);
	}

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	if (hSession == 0 || hSession >= (sizeof(cackey_sessions) / sizeof(cackey_sessions[0]))) {
		CACKEY_DEBUG_PRINTF("Error.  Session out of range.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	mutex_retval = cackey_mutex_lock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Locking failed.");

		return(CKR_GENERAL_ERROR);
	}

	if (!cackey_sessions[hSession].active) {
		cackey_mutex_unlock(cackey_biglock);

		CACKEY_DEBUG_PRINTF("Error.  Session not active.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	pInfo->slotID = cackey_sessions[hSession].slotID;
	pInfo->state = cackey_sessions[hSession].state;
	pInfo->flags = cackey_sessions[hSession].flags;
	pInfo->ulDeviceError = cackey_sessions[hSession].ulDeviceError;

	mutex_retval = cackey_mutex_unlock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Unlocking failed.");

		return(CKR_GENERAL_ERROR);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i)", CKR_OK);

	return(CKR_OK);
}

CK_DEFINE_FUNCTION(CK_RV, C_GetOperationState)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pOperationState, CK_ULONG_PTR pulOperationStateLen) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_SetOperationState)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pOperationState, CK_ULONG ulOperationStateLen, CK_OBJECT_HANDLE hEncryptionKey, CK_OBJECT_HANDLE hAuthenticationKey) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_Login)(CK_SESSION_HANDLE hSession, CK_USER_TYPE userType, CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen) {
	CK_SLOT_ID slotID;
	int mutex_retval;
	int tries_remaining;
	int login_ret;

	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	if (hSession == 0 || hSession >= (sizeof(cackey_sessions) / sizeof(cackey_sessions[0]))) {
		CACKEY_DEBUG_PRINTF("Error.  Session out of range.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	if (userType != CKU_USER) {
		CACKEY_DEBUG_PRINTF("Error.  We only support USER mode, asked for %lu mode.", (unsigned long) userType)

		return(CKR_USER_TYPE_INVALID);
	}

	mutex_retval = cackey_mutex_lock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Locking failed.");

		return(CKR_GENERAL_ERROR);
	}

	if (!cackey_sessions[hSession].active) {
		cackey_mutex_unlock(cackey_biglock);

		CACKEY_DEBUG_PRINTF("Error.  Session not active.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	slotID = cackey_sessions[hSession].slotID;

	if (slotID < 0 || slotID >= (sizeof(cackey_slots) / sizeof(cackey_slots[0]))) {
		CACKEY_DEBUG_PRINTF("Error. Invalid slot requested (%lu), outside of valid range", slotID);

		return(CKR_GENERAL_ERROR);
	}

	if (cackey_slots[slotID].active == 0) {
		CACKEY_DEBUG_PRINTF("Error. Invalid slot requested (%lu), slot not currently active", slotID);

		cackey_mutex_unlock(cackey_biglock);

		return(CKR_GENERAL_ERROR);
	}

	login_ret = cackey_login(&cackey_slots[slotID], pPin, ulPinLen, &tries_remaining);
	if (login_ret != CACKEY_PCSC_S_OK) {
		cackey_mutex_unlock(cackey_biglock);

		if (login_ret == CACKEY_PCSC_E_LOCKED) {
			CACKEY_DEBUG_PRINTF("Error.  Token is locked.");

			cackey_slots[slotID].token_flags |= CKF_USER_PIN_LOCKED;

			return(CKR_PIN_LOCKED);
		} else if (login_ret == CACKEY_PCSC_E_BADPIN) {
			CACKEY_DEBUG_PRINTF("Error.  Invalid PIN.");

			cackey_slots[slotID].token_flags |= CKF_USER_PIN_COUNT_LOW;

			if (tries_remaining == 1) {
				cackey_slots[slotID].token_flags |= CKF_USER_PIN_FINAL_TRY;
			}

			return(CKR_PIN_INCORRECT);
		}

		CACKEY_DEBUG_PRINTF("Error.  Unknown error returned from cackey_login() (%i)", login_ret);

		return(CKR_GENERAL_ERROR);
	}

	cackey_slots[slotID].token_flags &= ~(CKF_USER_PIN_LOCKED | CKF_USER_PIN_COUNT_LOW | CKF_LOGIN_REQUIRED | CKF_USER_PIN_FINAL_TRY);

	cackey_sessions[hSession].state = CKS_RO_USER_FUNCTIONS;

	mutex_retval = cackey_mutex_unlock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Unlocking failed.");

		return(CKR_GENERAL_ERROR);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i)", CKR_OK);

	return(CKR_OK);
}

CK_DEFINE_FUNCTION(CK_RV, C_Logout)(CK_SESSION_HANDLE hSession) {
	CK_SLOT_ID slotID;
	int mutex_retval;

	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	if (hSession == 0 || hSession >= (sizeof(cackey_sessions) / sizeof(cackey_sessions[0]))) {
		CACKEY_DEBUG_PRINTF("Error.  Session out of range.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	mutex_retval = cackey_mutex_lock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Locking failed.");

		return(CKR_GENERAL_ERROR);
	}

	if (!cackey_sessions[hSession].active) {
		cackey_mutex_unlock(cackey_biglock);

		CACKEY_DEBUG_PRINTF("Error.  Session not active.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	slotID = cackey_sessions[hSession].slotID;

	if (slotID < 0 || slotID >= (sizeof(cackey_slots) / sizeof(cackey_slots[0]))) {
		CACKEY_DEBUG_PRINTF("Error. Invalid slot requested (%lu), outside of valid range", slotID);

		return(CKR_GENERAL_ERROR);
	}

	if (cackey_slots[slotID].active == 0) {
		CACKEY_DEBUG_PRINTF("Error. Invalid slot requested (%lu), slot not currently active", slotID);

		cackey_mutex_unlock(cackey_biglock);

		return(CKR_GENERAL_ERROR);
	}

	cackey_sessions[hSession].state = CKS_RO_PUBLIC_SESSION;
	cackey_slots[slotID].token_flags = CKF_LOGIN_REQUIRED;

	mutex_retval = cackey_mutex_unlock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Unlocking failed.");

		return(CKR_GENERAL_ERROR);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i)", CKR_OK);

	return(CKR_OK);
}

CK_DEFINE_FUNCTION(CK_RV, C_CreateObject)(CK_SESSION_HANDLE hSession, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_OBJECT_HANDLE_PTR phObject) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_CopyObject)(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_OBJECT_HANDLE_PTR phNewObject) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_DestroyObject)(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_GetObjectSize)(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject, CK_ULONG_PTR pulSize) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_GetAttributeValue)(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount) {
	CK_ATTRIBUTE *curr_attr;
	struct cackey_identity *identity;
	unsigned long identity_idx, attr_idx, sess_attr_idx, num_ids;
	int mutex_retval;
	CK_RV retval = CKR_OK;
	CK_VOID_PTR pValue;
	CK_ULONG ulValueLen;

	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	if (hSession == 0 || hSession >= (sizeof(cackey_sessions) / sizeof(cackey_sessions[0]))) {
		CACKEY_DEBUG_PRINTF("Error.  Session out of range.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	if (hObject == 0) {
		CACKEY_DEBUG_PRINTF("Error.  Object handle out of range.");
		
		return(CKR_OBJECT_HANDLE_INVALID);
	}

	if (ulCount == 0) {
		/* Short circuit, if zero objects were specified return zero items immediately */
		CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i) (short circuit)", CKR_OK);

		return(CKR_OK);
	}

	if (pTemplate == NULL) {
		CACKEY_DEBUG_PRINTF("Error.  pTemplate is NULL.");

		return(CKR_ARGUMENTS_BAD);
	}

	identity_idx = hObject - 1;

	mutex_retval = cackey_mutex_lock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Locking failed.");

		return(CKR_GENERAL_ERROR);
	}

	if (!cackey_sessions[hSession].active) {
		cackey_mutex_unlock(cackey_biglock);

		CACKEY_DEBUG_PRINTF("Error.  Session not active.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	num_ids = cackey_sessions[hSession].identities_count;

	if (identity_idx >= num_ids) {
		cackey_mutex_unlock(cackey_biglock);

		CACKEY_DEBUG_PRINTF("Error.  Object handle out of range.  identity_idx = %lu, num_ids = %lu.", (unsigned long) identity_idx, (unsigned long) num_ids);

		return(CKR_OBJECT_HANDLE_INVALID);
	}

	identity = &cackey_sessions[hSession].identities[identity_idx];

	for (attr_idx = 0; attr_idx < ulCount; attr_idx++) {
		curr_attr = &pTemplate[attr_idx];

		pValue = NULL;
		ulValueLen = (CK_LONG) -1;

		CACKEY_DEBUG_PRINTF("Looking for attribute 0x%08lx (identity:%lu) ...", (unsigned long) curr_attr->type, (unsigned long) identity_idx);

		for (sess_attr_idx = 0; sess_attr_idx < identity->attributes_count; sess_attr_idx++) {
			if (identity->attributes[sess_attr_idx].type == curr_attr->type) {
				CACKEY_DEBUG_PRINTF(" ... found it, pValue = %p, ulValueLen = %lu", identity->attributes[sess_attr_idx].pValue, identity->attributes[sess_attr_idx].ulValueLen);
				
				pValue = identity->attributes[sess_attr_idx].pValue;
				ulValueLen = identity->attributes[sess_attr_idx].ulValueLen;
			}
		}

		if (curr_attr->pValue && pValue) {
			if (curr_attr->ulValueLen >= ulValueLen) {
				memcpy(curr_attr->pValue, pValue, ulValueLen);
			} else {
				ulValueLen = (CK_LONG) -1;

				retval = CKR_BUFFER_TOO_SMALL;
			}
		}

		curr_attr->ulValueLen = ulValueLen;
	}

	mutex_retval = cackey_mutex_unlock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Unlocking failed.");

		return(CKR_GENERAL_ERROR);
	}

	if (retval == CKR_ATTRIBUTE_TYPE_INVALID) {
		CACKEY_DEBUG_PRINTF("Returning CKR_ATTRIBUTE_TYPE_INVALID (%i)", (int) retval);
	} else if (retval == CKR_BUFFER_TOO_SMALL) {
		CACKEY_DEBUG_PRINTF("Returning CKR_BUFFER_TOO_SMALL (%i)", (int) retval);
	} else if (retval == CKR_OK) {
		CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i)", (int) retval);
	} else {
		CACKEY_DEBUG_PRINTF("Returning %i", (int) retval);
	}

	return(retval);
}

CK_DEFINE_FUNCTION(CK_RV, C_SetAttributeValue)(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_FindObjectsInit)(CK_SESSION_HANDLE hSession, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount) {
	CK_SLOT_ID slotID;
	CK_ULONG idx;
	int mutex_retval;

	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	if (hSession == 0 || hSession >= (sizeof(cackey_sessions) / sizeof(cackey_sessions[0]))) {
		CACKEY_DEBUG_PRINTF("Error.  Session out of range.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	mutex_retval = cackey_mutex_lock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Locking failed.");

		return(CKR_GENERAL_ERROR);
	}

	if (!cackey_sessions[hSession].active) {
		cackey_mutex_unlock(cackey_biglock);

		CACKEY_DEBUG_PRINTF("Error.  Session not active.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	if (cackey_sessions[hSession].search_active) {
		cackey_mutex_unlock(cackey_biglock);

		CACKEY_DEBUG_PRINTF("Error.  Search already active.");
		
		return(CKR_OPERATION_ACTIVE);
	}

	slotID = cackey_sessions[hSession].slotID;

	if (slotID < 0 || slotID >= (sizeof(cackey_slots) / sizeof(cackey_slots[0]))) {
		CACKEY_DEBUG_PRINTF("Error. Invalid slot requested (%lu), outside of valid range", slotID);

		return(CKR_GENERAL_ERROR);
	}

	if (cackey_slots[slotID].active == 0) {
		CACKEY_DEBUG_PRINTF("Error. Invalid slot requested (%lu), slot not currently active", slotID);

		cackey_mutex_unlock(cackey_biglock);

		return(CKR_GENERAL_ERROR);
	}

	if (cackey_slots[slotID].slot_reset) {
		CACKEY_DEBUG_PRINTF("The slot has been reset since we last looked for identities -- rescanning");

		if (cackey_sessions[hSession].identities != NULL) {
			cackey_free_identities(cackey_sessions[hSession].identities, cackey_sessions[hSession].identities_count);

			cackey_sessions[hSession].identities = NULL;
			cackey_sessions[hSession].identities_count = 0;
		}

		if (cackey_slots[slotID].label != NULL) {
			free(cackey_slots[slotID].label);
			cackey_slots[slotID].label = NULL;
		}

		cackey_mark_slot_reset(&cackey_slots[slotID]);
		cackey_slots[slotID].slot_reset = 0;
	}

	if (cackey_sessions[hSession].identities == NULL) {
		cackey_sessions[hSession].identities = cackey_read_identities(&cackey_slots[slotID], &cackey_sessions[hSession].identities_count);
	}

	if (pTemplate != NULL) {
		if (ulCount != 0) {
			cackey_sessions[hSession].search_query_count = ulCount;
			cackey_sessions[hSession].search_query = malloc(ulCount * sizeof(*pTemplate));

			memcpy(cackey_sessions[hSession].search_query, pTemplate, ulCount * sizeof(*pTemplate));
			for (idx = 0; idx < ulCount; idx++) {
				if (pTemplate[idx].ulValueLen == 0) {
					cackey_sessions[hSession].search_query[idx].pValue = NULL;

					continue;
				}

				cackey_sessions[hSession].search_query[idx].pValue = malloc(pTemplate[idx].ulValueLen);

				if (cackey_sessions[hSession].search_query[idx].pValue) {
					memcpy(cackey_sessions[hSession].search_query[idx].pValue, pTemplate[idx].pValue, pTemplate[idx].ulValueLen);
				}
			}
		} else {
			cackey_sessions[hSession].search_query_count = 0;
			cackey_sessions[hSession].search_query = NULL;
		}
	} else {
		if (ulCount != 0) {
			cackey_mutex_unlock(cackey_biglock);

			CACKEY_DEBUG_PRINTF("Error.  Search query specified as NULL, but number of query terms not specified as 0.");

			return(CKR_ARGUMENTS_BAD);
		}

		cackey_sessions[hSession].search_query_count = 0;
		cackey_sessions[hSession].search_query = NULL;
	}

	cackey_sessions[hSession].search_active = 1;
	cackey_sessions[hSession].search_curr_id = 0;

	mutex_retval = cackey_mutex_unlock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Unlocking failed.");

		return(CKR_GENERAL_ERROR);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i)", CKR_OK);

	return(CKR_OK);
}

static int cackey_pkcs11_compare_attributes(CK_ATTRIBUTE *a, CK_ATTRIBUTE *b) {
	unsigned char *smallbuf, *largebuf;
	size_t smallbuf_len, largebuf_len;

	if (a->type != b->type) {
		return(0);
	}

	CACKEY_DEBUG_PRINTF("    ... found matching type ...");

	CACKEY_DEBUG_PRINTBUF("    ... our value:", a->pValue, a->ulValueLen);

	if (b->pValue == NULL) {
		CACKEY_DEBUG_PRINTF("       ... found wildcard match");

		return(1);
	}

	if (a->pValue == NULL) {
		return(0);
	}

 	if (b->ulValueLen == a->ulValueLen && memcmp(a->pValue, b->pValue, b->ulValueLen) == 0) {
		CACKEY_DEBUG_PRINTF("       ... found exact match");

		return(1);
	}

	switch (a->type) {
		case CKA_MODULUS:
			if (a->ulValueLen == b->ulValueLen) {
				break;
			}

			if (a->ulValueLen > b->ulValueLen) {
				smallbuf = b->pValue;
				smallbuf_len = b->ulValueLen;

				largebuf = a->pValue;
				largebuf_len = a->ulValueLen;
			} else {
				smallbuf = a->pValue;
				smallbuf_len = a->ulValueLen;

				largebuf = b->pValue;
				largebuf_len = b->ulValueLen;
			}

			for (; largebuf_len != smallbuf_len; largebuf++,largebuf_len--) {
				if (largebuf[0] != 0) {
					break;
				}
			}

			if (largebuf_len != smallbuf_len) {
				break;
			}

			if (memcmp(largebuf, smallbuf, smallbuf_len) == 0) {
				CACKEY_DEBUG_PRINTF("       ... found approximate match");

				return(1);
			}

			break;
	}

	return(0);
}

CK_DEFINE_FUNCTION(CK_RV, C_FindObjects)(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE_PTR phObject, CK_ULONG ulMaxObjectCount, CK_ULONG_PTR pulObjectCount) {
	struct cackey_identity *curr_id;
	CK_ATTRIBUTE *curr_attr;
	CK_ULONG curr_id_idx, curr_out_id_idx, curr_attr_idx, sess_attr_idx;
	CK_ULONG matched_count, prev_matched_count;
	int mutex_retval;
#ifdef CACKEY_DEBUG_SEARCH_SPEEDTEST
	struct timeval start, end;
	uint64_t start_int, end_int;
#endif

	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	if (pulObjectCount == NULL) {
		CACKEY_DEBUG_PRINTF("Error.  pulObjectCount is NULL.");

		return(CKR_ARGUMENTS_BAD);
	}

	if (phObject == NULL && ulMaxObjectCount == 0) {
		/* Short circuit, if zero objects were specified return zero items immediately */
		*pulObjectCount = 0;

		CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i) (short circuit)", CKR_OK);

		return(CKR_OK);
	}

	if (phObject == NULL) {
		CACKEY_DEBUG_PRINTF("Error.  phObject is NULL.");

		return(CKR_ARGUMENTS_BAD);
	}

	if (ulMaxObjectCount == 0) {
		CACKEY_DEBUG_PRINTF("Error.  Maximum number of objects specified as zero.");

		return(CKR_ARGUMENTS_BAD);
	}

	if (hSession == 0 || hSession >= (sizeof(cackey_sessions) / sizeof(cackey_sessions[0]))) {
		CACKEY_DEBUG_PRINTF("Error.  Session out of range.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	mutex_retval = cackey_mutex_lock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Locking failed.");

		return(CKR_GENERAL_ERROR);
	}

	if (!cackey_sessions[hSession].active) {
		cackey_mutex_unlock(cackey_biglock);

		CACKEY_DEBUG_PRINTF("Error.  Session not active.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	if (!cackey_sessions[hSession].search_active) {
		cackey_mutex_unlock(cackey_biglock);

		CACKEY_DEBUG_PRINTF("Error.  Search not active.");
		
		return(CKR_OPERATION_NOT_INITIALIZED);
	}

#ifdef CACKEY_DEBUG_SEARCH_SPEEDTEST
	gettimeofday(&start, NULL);
#endif

	curr_out_id_idx = 0;
	for (curr_id_idx = cackey_sessions[hSession].search_curr_id; curr_id_idx < cackey_sessions[hSession].identities_count && ulMaxObjectCount; curr_id_idx++) {
		curr_id = &cackey_sessions[hSession].identities[curr_id_idx];

		CACKEY_DEBUG_PRINTF("Processing identity:%lu", (unsigned long) curr_id_idx);

		matched_count = 0;

		for (curr_attr_idx = 0; curr_attr_idx < cackey_sessions[hSession].search_query_count; curr_attr_idx++) {
			prev_matched_count = matched_count;

			curr_attr = &cackey_sessions[hSession].search_query[curr_attr_idx];

			CACKEY_DEBUG_PRINTF("  Checking for attribute %s (0x%08lx) in identity:%i...", CACKEY_DEBUG_FUNC_ATTRIBUTE_TO_STR(curr_attr->type), (unsigned long) curr_attr->type, (int) curr_id_idx);
			CACKEY_DEBUG_PRINTBUF("    Value looking for:", curr_attr->pValue, curr_attr->ulValueLen);

			for (sess_attr_idx = 0; sess_attr_idx < curr_id->attributes_count; sess_attr_idx++) {
				if (cackey_pkcs11_compare_attributes(&curr_id->attributes[sess_attr_idx], curr_attr)) {
					matched_count++;

					break;
				}
			}

			/* If the attribute could not be matched, do not try to match additional attributes */
			if (prev_matched_count == matched_count) {
				break;
			}
		}

		if (matched_count == cackey_sessions[hSession].search_query_count) {
			CACKEY_DEBUG_PRINTF("  ... All %i attributes checked for found, adding identity:%i to returned list", (int) cackey_sessions[hSession].search_query_count, (int) curr_id_idx);

			phObject[curr_out_id_idx] = curr_id_idx + 1;

			ulMaxObjectCount--;

			curr_out_id_idx++;
		} else {
			CACKEY_DEBUG_PRINTF("  ... Not all %i (only found %i) attributes checked for found, not adding identity:%i", (int) cackey_sessions[hSession].search_query_count, (int) matched_count, (int) curr_id_idx);
		}
	}
	cackey_sessions[hSession].search_curr_id = curr_id_idx;
	*pulObjectCount = curr_out_id_idx;

#ifdef CACKEY_DEBUG_SEARCH_SPEEDTEST
	gettimeofday(&end, NULL);
	start_int = (start.tv_sec * 1000000) + start.tv_usec;
	end_int = (end.tv_sec * 1000000) + end.tv_usec;
	fprintf(stderr, "Search took %lu microseconds\n", (unsigned long) (end_int - start_int));
#endif

	mutex_retval = cackey_mutex_unlock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Unlocking failed.");

		return(CKR_GENERAL_ERROR);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i), num objects = %lu", CKR_OK, *pulObjectCount);

	return(CKR_OK);
}

CK_DEFINE_FUNCTION(CK_RV, C_FindObjectsFinal)(CK_SESSION_HANDLE hSession) {
	CK_ULONG idx;
	int mutex_retval;

	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	if (hSession == 0 || hSession >= (sizeof(cackey_sessions) / sizeof(cackey_sessions[0]))) {
		CACKEY_DEBUG_PRINTF("Error.  Session out of range.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	mutex_retval = cackey_mutex_lock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Locking failed.");

		return(CKR_GENERAL_ERROR);
	}

	if (!cackey_sessions[hSession].active) {
		cackey_mutex_unlock(cackey_biglock);

		CACKEY_DEBUG_PRINTF("Error.  Session not active.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	if (!cackey_sessions[hSession].search_active) {
		cackey_mutex_unlock(cackey_biglock);

		CACKEY_DEBUG_PRINTF("Error.  Search not active.");
		
		return(CKR_OPERATION_NOT_INITIALIZED);
	}

	cackey_sessions[hSession].search_active = 0;

	for (idx = 0; idx < cackey_sessions[hSession].search_query_count; idx++) {
		if (cackey_sessions[hSession].search_query[idx].pValue) {
			free(cackey_sessions[hSession].search_query[idx].pValue);
		}
	}

	if (cackey_sessions[hSession].search_query) {
		free(cackey_sessions[hSession].search_query);
	}

	mutex_retval = cackey_mutex_unlock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Unlocking failed.");

		return(CKR_GENERAL_ERROR);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i)", CKR_OK);

	return(CKR_OK);
}

CK_DEFINE_FUNCTION(CK_RV, C_EncryptInit)(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_Encrypt)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pEncryptedData, CK_ULONG_PTR pulEncryptedDataLen) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_EncryptUpdate)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pPart, CK_ULONG ulPartLen, CK_BYTE_PTR pEncryptedPart, CK_ULONG_PTR pulEncryptedPartLen) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_EncryptFinal)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pLastEncryptedPart, CK_ULONG_PTR pulLastEncryptedPartLen) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_DecryptInit)(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey) {
	int mutex_retval;

	hKey--;

	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	if (pMechanism == NULL) {
		CACKEY_DEBUG_PRINTF("Error. pMechanism is NULL.");

		return(CKR_ARGUMENTS_BAD);
	}

	if (pMechanism->mechanism != CKM_RSA_PKCS) {
		CACKEY_DEBUG_PRINTF("Error. pMechanism->mechanism not specified as CKM_RSA_PKCS");

		return(CKR_MECHANISM_PARAM_INVALID);
	}

	if (hSession == 0 || hSession >= (sizeof(cackey_sessions) / sizeof(cackey_sessions[0]))) {
		CACKEY_DEBUG_PRINTF("Error.  Session out of range.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	mutex_retval = cackey_mutex_lock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Locking failed.");

		return(CKR_GENERAL_ERROR);
	}

	if (!cackey_sessions[hSession].active) {
		cackey_mutex_unlock(cackey_biglock);

		CACKEY_DEBUG_PRINTF("Error.  Session not active.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	if (cackey_sessions[hSession].decrypt_active) {
		cackey_mutex_unlock(cackey_biglock);

		CACKEY_DEBUG_PRINTF("Error.  Decrypt already in progress.");
		
		return(CKR_OPERATION_ACTIVE);
	}

	if (hKey >= cackey_sessions[hSession].identities_count) {
		cackey_mutex_unlock(cackey_biglock);

		CACKEY_DEBUG_PRINTF("Error.  Key handle out of range (requested key %lu, only %lu identities available).", (unsigned long) hKey, (unsigned long) cackey_sessions[hSession].identities_count);

		return(CKR_KEY_HANDLE_INVALID);
	}

	cackey_sessions[hSession].decrypt_active = 1;

	cackey_sessions[hSession].decrypt_mechanism = pMechanism->mechanism;
	cackey_sessions[hSession].decrypt_mech_parm = pMechanism->pParameter;
	cackey_sessions[hSession].decrypt_mech_parmlen = pMechanism->ulParameterLen;
	cackey_sessions[hSession].decrypt_identity = &cackey_sessions[hSession].identities[hKey];

	mutex_retval = cackey_mutex_unlock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Unlocking failed.");

		return(CKR_GENERAL_ERROR);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i)", CKR_OK);

	return(CKR_OK);
}

CK_DEFINE_FUNCTION(CK_RV, C_Decrypt)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pEncryptedData, CK_ULONG ulEncryptedDataLen, CK_BYTE_PTR pData, CK_ULONG_PTR pulDataLen) {
	CK_ULONG datalen_update, datalen_final;
	CK_RV decrypt_ret;
	int mutex_retval;

	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	if (pulDataLen == NULL) {
		CACKEY_DEBUG_PRINTF("Error. pulDataLen is NULL.");

		return(CKR_ARGUMENTS_BAD);
	}

	datalen_update = *pulDataLen;

	decrypt_ret = C_DecryptUpdate(hSession, pEncryptedData, ulEncryptedDataLen, pData, &datalen_update);
	if (decrypt_ret != CKR_OK) {
		CACKEY_DEBUG_PRINTF("Error.  DecryptUpdate() returned failure (rv = %lu).", (unsigned long) decrypt_ret);

		if (decrypt_ret != CKR_BUFFER_TOO_SMALL) {
			/* Terminate decryption operation */

			mutex_retval = cackey_mutex_lock(cackey_biglock);
			if (mutex_retval != 0) {
				CACKEY_DEBUG_PRINTF("Error.  Locking failed.");

				return(CKR_GENERAL_ERROR);
			}

			if (!cackey_sessions[hSession].active) {
				cackey_mutex_unlock(cackey_biglock);

				CACKEY_DEBUG_PRINTF("Error.  Session not active.");
		
				return(CKR_SESSION_HANDLE_INVALID);
			}

			if (!cackey_sessions[hSession].decrypt_active) {
				cackey_mutex_unlock(cackey_biglock);

				CACKEY_DEBUG_PRINTF("Error.  Decrypt not active.");
		
				return(CKR_OPERATION_NOT_INITIALIZED);
			}

			cackey_sessions[hSession].decrypt_active = 0;

			mutex_retval = cackey_mutex_unlock(cackey_biglock);
			if (mutex_retval != 0) {
				CACKEY_DEBUG_PRINTF("Error.  Unlocking failed.");

				return(CKR_GENERAL_ERROR);
			}
		}

		return(decrypt_ret);
	}

	if (pData) {
		pData += datalen_update;
	}
	datalen_final = *pulDataLen - datalen_update;

	decrypt_ret = C_DecryptFinal(hSession, pData, &datalen_final);
	if (decrypt_ret != CKR_OK) {
		CACKEY_DEBUG_PRINTF("Error.  DecryptFinal() returned failure (rv = %lu).", (unsigned long) decrypt_ret);

		return(decrypt_ret);
	}

	*pulDataLen = datalen_update + datalen_final;

	CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i)", CKR_OK);

	return(CKR_OK);
}

CK_DEFINE_FUNCTION(CK_RV, C_DecryptUpdate)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pEncryptedPart, CK_ULONG ulEncryptedPartLen, CK_BYTE_PTR pPart, CK_ULONG_PTR pulPartLen) {
	static CK_BYTE buf[16384];
	ssize_t buflen;
	CK_SLOT_ID slotID;
	CK_RV retval = CKR_GENERAL_ERROR;
	int mutex_retval;

	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	if (hSession == 0 || hSession >= (sizeof(cackey_sessions) / sizeof(cackey_sessions[0]))) {
		CACKEY_DEBUG_PRINTF("Error.  Session out of range.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	if (pEncryptedPart == NULL && ulEncryptedPartLen == 0) {
		/* Short circuit if we are asked to decrypt nothing... */
		CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i) (short circuit)", CKR_OK);

		return(CKR_OK);
	}

	if (pEncryptedPart == NULL) {
		CACKEY_DEBUG_PRINTF("Error. pEncryptedPart is NULL, but ulEncryptedPartLen is not 0.");

		return(CKR_ARGUMENTS_BAD);
	}

	if (ulEncryptedPartLen == 0) {
		CACKEY_DEBUG_PRINTF("Error. ulEncryptedPartLen is 0, but pPart is not NULL.");

		return(CKR_ARGUMENTS_BAD);
	}

	if (pulPartLen == NULL) {
		CACKEY_DEBUG_PRINTF("Error. pulPartLen is NULL.");

		return(CKR_ARGUMENTS_BAD);
	}

	mutex_retval = cackey_mutex_lock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Locking failed.");

		return(CKR_GENERAL_ERROR);
	}

	if (!cackey_sessions[hSession].active) {
		cackey_mutex_unlock(cackey_biglock);

		CACKEY_DEBUG_PRINTF("Error.  Session not active.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	if (!cackey_sessions[hSession].decrypt_active) {
		cackey_mutex_unlock(cackey_biglock);

		CACKEY_DEBUG_PRINTF("Error.  Decrypt not active.");
		
		return(CKR_OPERATION_NOT_INITIALIZED);
	}

	slotID = cackey_sessions[hSession].slotID;

	if (slotID < 0 || slotID >= (sizeof(cackey_slots) / sizeof(cackey_slots[0]))) {
		CACKEY_DEBUG_PRINTF("Error. Invalid slot requested (%lu), outside of valid range", slotID);

		return(CKR_GENERAL_ERROR);
	}

	if (cackey_slots[slotID].active == 0) {
		CACKEY_DEBUG_PRINTF("Error. Invalid slot requested (%lu), slot not currently active", slotID);

		cackey_mutex_unlock(cackey_biglock);

		return(CKR_GENERAL_ERROR);
	}

	switch (cackey_sessions[hSession].decrypt_mechanism) {
		case CKM_RSA_PKCS:
			/* Ask card to decrypt */
			buflen = cackey_signdecrypt(&cackey_slots[slotID], cackey_sessions[hSession].decrypt_identity, pEncryptedPart, ulEncryptedPartLen, buf, sizeof(buf), 0, 1);

			if (buflen < 0) {
				/* Decryption failed. */
				if (buflen == CACKEY_PCSC_E_NEEDLOGIN) {
					retval = CKR_USER_NOT_LOGGED_IN;
				} else if (buflen == CACKEY_PCSC_E_TOKENABSENT) {
					retval = CKR_DEVICE_REMOVED;
				} else {
					retval = CKR_GENERAL_ERROR;
				}
			} else if (((unsigned long) buflen) > *pulPartLen && pPart) {
				/* Decrypted data too large */
				retval = CKR_BUFFER_TOO_SMALL;
			} else {
				if (pPart) {
					memcpy(pPart, buf, buflen);
				}

				*pulPartLen = buflen;

				retval = CKR_OK;
			}

			break;
	}

	mutex_retval = cackey_mutex_unlock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Unlocking failed.");

		return(CKR_GENERAL_ERROR);
	}

	CACKEY_DEBUG_PRINTF("Returning %i", (int) retval);

	return(retval);
}

CK_DEFINE_FUNCTION(CK_RV, C_DecryptFinal)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pLastPart, CK_ULONG_PTR pulLastPartLen) {
	int mutex_retval;
	int terminate_decrypt = 1;

	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	if (hSession == 0 || hSession >= (sizeof(cackey_sessions) / sizeof(cackey_sessions[0]))) {
		CACKEY_DEBUG_PRINTF("Error.  Session out of range.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	if (pulLastPartLen == NULL) {
		CACKEY_DEBUG_PRINTF("Error. pulLastPartLen is NULL.");

		return(CKR_ARGUMENTS_BAD);
	}

	mutex_retval = cackey_mutex_lock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Locking failed.");

		return(CKR_GENERAL_ERROR);
	}

	if (!cackey_sessions[hSession].active) {
		cackey_mutex_unlock(cackey_biglock);

		CACKEY_DEBUG_PRINTF("Error.  Session not active.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	if (!cackey_sessions[hSession].decrypt_active) {
		cackey_mutex_unlock(cackey_biglock);

		CACKEY_DEBUG_PRINTF("Error.  Decrypt not active.");
		
		return(CKR_OPERATION_NOT_INITIALIZED);
	}

	*pulLastPartLen = 0;

	if (pLastPart == NULL) {
		terminate_decrypt = 0;
	}

	if (terminate_decrypt) {
		cackey_sessions[hSession].decrypt_active = 0;
	}

	mutex_retval = cackey_mutex_unlock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Unlocking failed.");

		return(CKR_GENERAL_ERROR);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i)", CKR_OK);

	return(CKR_OK);
}

CK_DEFINE_FUNCTION(CK_RV, C_DigestInit)(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_Digest)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pDigest, CK_ULONG_PTR pulDigestLen) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_DigestUpdate)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pPart, CK_ULONG ulPartLen) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_DigestKey)(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hKey) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_DigestFinal)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pDigest, CK_ULONG_PTR pulDigestLen) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_SignInit)(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey) {
	int mutex_retval;

	hKey--;

	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	if (pMechanism == NULL) {
		CACKEY_DEBUG_PRINTF("Error. pMechanism is NULL.");

		return(CKR_ARGUMENTS_BAD);
	}

	if (pMechanism->mechanism != CKM_RSA_PKCS) {
		CACKEY_DEBUG_PRINTF("Error. pMechanism->mechanism not specified as CKM_RSA_PKCS");

		return(CKR_MECHANISM_PARAM_INVALID);
	}

	if (hSession == 0 || hSession >= (sizeof(cackey_sessions) / sizeof(cackey_sessions[0]))) {
		CACKEY_DEBUG_PRINTF("Error.  Session out of range.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	mutex_retval = cackey_mutex_lock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Locking failed.");

		return(CKR_GENERAL_ERROR);
	}

	if (!cackey_sessions[hSession].active) {
		cackey_mutex_unlock(cackey_biglock);

		CACKEY_DEBUG_PRINTF("Error.  Session not active.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	if (cackey_sessions[hSession].sign_active) {
		cackey_mutex_unlock(cackey_biglock);

		CACKEY_DEBUG_PRINTF("Error.  Sign already in progress.");
		
		return(CKR_OPERATION_ACTIVE);
	}

	if (hKey >= cackey_sessions[hSession].identities_count) {
		cackey_mutex_unlock(cackey_biglock);

		CACKEY_DEBUG_PRINTF("Error.  Key handle out of range (requested key %lu, only %lu identities available).", (unsigned long) hKey, (unsigned long) cackey_sessions[hSession].identities_count);

		return(CKR_KEY_HANDLE_INVALID);
	}

	cackey_sessions[hSession].sign_active = 1;

	cackey_sessions[hSession].sign_mechanism = pMechanism->mechanism;

	cackey_sessions[hSession].sign_buflen = 128;
	cackey_sessions[hSession].sign_bufused = 0;
	cackey_sessions[hSession].sign_buf = malloc(sizeof(*cackey_sessions[hSession].sign_buf) * cackey_sessions[hSession].sign_buflen);

	CACKEY_DEBUG_PRINTF("Session %lu sign_identity is %p (identity #%lu)", (unsigned long) hSession, (void *) &cackey_sessions[hSession].identities[hKey], (unsigned long) hKey);
	cackey_sessions[hSession].sign_identity = &cackey_sessions[hSession].identities[hKey];

	mutex_retval = cackey_mutex_unlock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Unlocking failed.");

		return(CKR_GENERAL_ERROR);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i)", CKR_OK);

	return(CKR_OK);
}

CK_DEFINE_FUNCTION(CK_RV, C_Sign)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pSignature, CK_ULONG_PTR pulSignatureLen) {
	unsigned long start_sign_bufused;
	CK_RV sign_ret;
	int mutex_retval;

	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	if (hSession == 0 || hSession >= (sizeof(cackey_sessions) / sizeof(cackey_sessions[0]))) {
		CACKEY_DEBUG_PRINTF("Error.  Session out of range.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	start_sign_bufused = cackey_sessions[hSession].sign_bufused;

	sign_ret = C_SignUpdate(hSession, pData, ulDataLen);
	if (sign_ret != CKR_OK) {
		CACKEY_DEBUG_PRINTF("Error.  SignUpdate() returned failure (rv = %lu).", (unsigned long) sign_ret);

		if (sign_ret != CKR_BUFFER_TOO_SMALL) {
			mutex_retval = cackey_mutex_lock(cackey_biglock);
			if (mutex_retval != 0) {
				CACKEY_DEBUG_PRINTF("Error.  Locking failed.");

				return(CKR_GENERAL_ERROR);
			}

			if (!cackey_sessions[hSession].active) {
				cackey_mutex_unlock(cackey_biglock);

				CACKEY_DEBUG_PRINTF("Error.  Session not active.");
		
				return(CKR_SESSION_HANDLE_INVALID);
			}

			if (!cackey_sessions[hSession].sign_active) {
				cackey_mutex_unlock(cackey_biglock);

				CACKEY_DEBUG_PRINTF("Error.  Sign not active.");
		
				return(CKR_OPERATION_NOT_INITIALIZED);
			}

			cackey_sessions[hSession].sign_active = 0;

			mutex_retval = cackey_mutex_unlock(cackey_biglock);
			if (mutex_retval != 0) {
				CACKEY_DEBUG_PRINTF("Error.  Unlocking failed.");

				return(CKR_GENERAL_ERROR);
			}
		}

		return(sign_ret);
	}

	sign_ret = C_SignFinal(hSession, pSignature, pulSignatureLen);
	if (sign_ret != CKR_OK) {
		if (sign_ret == CKR_BUFFER_TOO_SMALL) {
			CACKEY_DEBUG_PRINTF("SignFinal() returned CKR_BUFFER_TOO_SMALL (rv = %lu), undoing C_SignUpdate()", (unsigned long) sign_ret);

			cackey_sessions[hSession].sign_bufused = start_sign_bufused;

			return(sign_ret);
		}

		CACKEY_DEBUG_PRINTF("Error.  SignFinal() returned failure (rv = %lu).", (unsigned long) sign_ret);

		return(sign_ret);
	}

	if (pSignature == NULL) {
		CACKEY_DEBUG_PRINTF("pSignature specified as NULL, undoing C_SignUpdate()");

		cackey_sessions[hSession].sign_bufused = start_sign_bufused;

		return(sign_ret);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i)", CKR_OK);

	return(CKR_OK);
}

CK_DEFINE_FUNCTION(CK_RV, C_SignUpdate)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pPart, CK_ULONG ulPartLen) {
	int mutex_retval;

	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	if (hSession == 0 || hSession >= (sizeof(cackey_sessions) / sizeof(cackey_sessions[0]))) {
		CACKEY_DEBUG_PRINTF("Error.  Session out of range.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	if (pPart == NULL && ulPartLen == 0) {
		/* Short circuit if we are asked to sign nothing... */
		CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i) (short circuit)", CKR_OK);

		return(CKR_OK);
	}

	if (pPart == NULL) {
		CACKEY_DEBUG_PRINTF("Error. pPart is NULL, but ulPartLen is not 0.");

		return(CKR_ARGUMENTS_BAD);
	}

	if (ulPartLen == 0) {
		CACKEY_DEBUG_PRINTF("Error. ulPartLen is 0, but pPart is not NULL.");

		return(CKR_ARGUMENTS_BAD);
	}

	mutex_retval = cackey_mutex_lock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Locking failed.");

		return(CKR_GENERAL_ERROR);
	}

	if (!cackey_sessions[hSession].active) {
		cackey_mutex_unlock(cackey_biglock);

		CACKEY_DEBUG_PRINTF("Error.  Session not active.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	if (!cackey_sessions[hSession].sign_active) {
		cackey_mutex_unlock(cackey_biglock);

		CACKEY_DEBUG_PRINTF("Error.  Sign not active.");
		
		return(CKR_OPERATION_NOT_INITIALIZED);
	}

	switch (cackey_sessions[hSession].sign_mechanism) {
		case CKM_RSA_PKCS:
			/* Accumulate directly */
			if ((cackey_sessions[hSession].sign_bufused + ulPartLen) > cackey_sessions[hSession].sign_buflen) {
				cackey_sessions[hSession].sign_buflen *= 2;

				cackey_sessions[hSession].sign_buf = realloc(cackey_sessions[hSession].sign_buf, sizeof(*cackey_sessions[hSession].sign_buf) * cackey_sessions[hSession].sign_buflen);
			}

			memcpy(cackey_sessions[hSession].sign_buf + cackey_sessions[hSession].sign_bufused, pPart, ulPartLen);

			cackey_sessions[hSession].sign_bufused += ulPartLen;

			break;
	}

	mutex_retval = cackey_mutex_unlock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Unlocking failed.");

		return(CKR_GENERAL_ERROR);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i)", CKR_OK);

	return(CKR_OK);
}

CK_DEFINE_FUNCTION(CK_RV, C_SignFinal)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pSignature, CK_ULONG_PTR pulSignatureLen) {
	static CK_BYTE sigbuf[1024];
	ssize_t sigbuflen;
	CK_SLOT_ID slotID;
	CK_RV retval = CKR_GENERAL_ERROR;
	int terminate_sign = 1;
	int mutex_retval;

	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	if (pulSignatureLen == NULL) {
		CACKEY_DEBUG_PRINTF("Error. pulSignatureLen is NULL.");

		return(CKR_ARGUMENTS_BAD);
	}

	if (hSession == 0 || hSession >= (sizeof(cackey_sessions) / sizeof(cackey_sessions[0]))) {
		CACKEY_DEBUG_PRINTF("Error.  Session out of range.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	mutex_retval = cackey_mutex_lock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Locking failed.");

		return(CKR_GENERAL_ERROR);
	}

	if (!cackey_sessions[hSession].active) {
		cackey_mutex_unlock(cackey_biglock);

		CACKEY_DEBUG_PRINTF("Error.  Session not active.");
		
		return(CKR_SESSION_HANDLE_INVALID);
	}

	if (!cackey_sessions[hSession].sign_active) {
		cackey_mutex_unlock(cackey_biglock);

		CACKEY_DEBUG_PRINTF("Error.  Sign not active.");
		
		return(CKR_OPERATION_NOT_INITIALIZED);
	}

	slotID = cackey_sessions[hSession].slotID;

	if (slotID < 0 || slotID >= (sizeof(cackey_slots) / sizeof(cackey_slots[0]))) {
		CACKEY_DEBUG_PRINTF("Error. Invalid slot requested (%lu), outside of valid range", slotID);

		return(CKR_GENERAL_ERROR);
	}

	if (cackey_slots[slotID].active == 0) {
		CACKEY_DEBUG_PRINTF("Error. Invalid slot requested (%lu), slot not currently active", slotID);

		cackey_mutex_unlock(cackey_biglock);

		return(CKR_GENERAL_ERROR);
	}

	switch (cackey_sessions[hSession].sign_mechanism) {
		case CKM_RSA_PKCS:
			/* Ask card to sign */
			CACKEY_DEBUG_PRINTF("Asking to sign from identity %p in session %lu", (void *) cackey_sessions[hSession].sign_identity, (unsigned long) hSession);
			sigbuflen = cackey_signdecrypt(&cackey_slots[slotID], cackey_sessions[hSession].sign_identity, cackey_sessions[hSession].sign_buf, cackey_sessions[hSession].sign_bufused, sigbuf, sizeof(sigbuf), 1, 0);

			if (sigbuflen < 0) {
				/* Signing failed. */
				if (sigbuflen == CACKEY_PCSC_E_NEEDLOGIN) {
					retval = CKR_USER_NOT_LOGGED_IN;
				} else if (sigbuflen == CACKEY_PCSC_E_TOKENABSENT) {
					retval = CKR_DEVICE_REMOVED;
				} else {
					retval = CKR_GENERAL_ERROR;
				}
			} else if (((unsigned long) sigbuflen) > *pulSignatureLen && pSignature) {
				/* Signed data too large */
				CACKEY_DEBUG_PRINTF("retval = CKR_BUFFER_TOO_SMALL;  sigbuflen = %lu, pulSignatureLen = %lu", (unsigned long) sigbuflen, (unsigned long) *pulSignatureLen);

				retval = CKR_BUFFER_TOO_SMALL;

				terminate_sign = 0;
			} else {
				terminate_sign = 0;

				if (pSignature) {
					memcpy(pSignature, sigbuf, sigbuflen);

					terminate_sign = 1;
				}

				*pulSignatureLen = sigbuflen;

				retval = CKR_OK;
			}

			break;
	}

	if (terminate_sign) {
		if (cackey_sessions[hSession].sign_buf) {
			free(cackey_sessions[hSession].sign_buf);
		}

		cackey_sessions[hSession].sign_active = 0;
	}

	mutex_retval = cackey_mutex_unlock(cackey_biglock);
	if (mutex_retval != 0) {
		CACKEY_DEBUG_PRINTF("Error.  Unlocking failed.");

		return(CKR_GENERAL_ERROR);
	}

	CACKEY_DEBUG_PRINTF("Returning %i", (int) retval);

	return(retval);
}

CK_DEFINE_FUNCTION(CK_RV, C_SignRecoverInit)(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_SignRecover)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pSignature, CK_ULONG_PTR pulSignatureLen) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_VerifyInit)(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_Verify)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pSignature, CK_ULONG ulSignatureLen) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_VerifyUpdate)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pPart, CK_ULONG ulPartLen) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_VerifyFinal)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pSignature, CK_ULONG ulSignatureLen) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_VerifyRecoverInit)(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_VerifyRecover)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pSignature, CK_ULONG ulSignatureLen, CK_BYTE_PTR pData, CK_ULONG_PTR pulDataLen) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_DigestEncryptUpdate)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pPart, CK_ULONG ulPartLen, CK_BYTE_PTR pEncryptedPart, CK_ULONG_PTR pulEncryptedPartLen) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_DecryptDigestUpdate)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pEncryptedPart, CK_ULONG ulEncryptedPartLen, CK_BYTE_PTR pPart, CK_ULONG_PTR pulPartLen) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_SignEncryptUpdate)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pPart, CK_ULONG ulPartLen, CK_BYTE_PTR pEncryptedPart, CK_ULONG_PTR pulEncryptedPartLen) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_DecryptVerifyUpdate)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pEncryptedPart, CK_ULONG ulEncryptedPartLen, CK_BYTE_PTR pPart, CK_ULONG_PTR pulPartLen) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_GenerateKey)(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_OBJECT_HANDLE_PTR phKey) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_GenerateKeyPair)(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_ATTRIBUTE_PTR pPublicKeyTemplate, CK_ULONG ulPublicKeyAttributeCount, CK_ATTRIBUTE_PTR pPrivateKeyTemplate, CK_ULONG ulPrivateKeyAttributeCount, CK_OBJECT_HANDLE_PTR phPublicKey, CK_OBJECT_HANDLE_PTR phPrivateKey) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_WrapKey)(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hWrappingKey, CK_OBJECT_HANDLE hKey, CK_BYTE_PTR pWrappedKey, CK_ULONG_PTR pulWrappedKeyLen) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_UnwrapKey)(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hUnwrappingKey, CK_BYTE_PTR pWrappedKey, CK_ULONG ulWrappedKeyLen, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulAttributeCount, CK_OBJECT_HANDLE_PTR phKey) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_DeriveKey)(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hBaseKey, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulAttributeCount, CK_OBJECT_HANDLE_PTR phKey) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_SeedRandom)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pSeed, CK_ULONG ulSeedLen) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

CK_DEFINE_FUNCTION(CK_RV, C_GenerateRandom)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pRandomData, CK_ULONG ulRandomLen) {
	CACKEY_DEBUG_PRINTF("Called.");

	if (!cackey_initialized) {
		CACKEY_DEBUG_PRINTF("Error.  Not initialized.");

		return(CKR_CRYPTOKI_NOT_INITIALIZED);
	}

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_SUPPORTED (%i)", CKR_FUNCTION_NOT_SUPPORTED);

	return(CKR_FUNCTION_NOT_SUPPORTED);
}

/* Deprecated Function */
CK_DEFINE_FUNCTION(CK_RV, C_GetFunctionStatus)(CK_SESSION_HANDLE hSession) {
	CACKEY_DEBUG_PRINTF("Called.");

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_PARALLEL (%i)", CKR_FUNCTION_NOT_PARALLEL);

	return(CKR_FUNCTION_NOT_PARALLEL);

	hSession = hSession; /* Supress unused variable warning */
}

/* Deprecated Function */
CK_DEFINE_FUNCTION(CK_RV, C_CancelFunction)(CK_SESSION_HANDLE hSession) {
	CACKEY_DEBUG_PRINTF("Called.");

	CACKEY_DEBUG_PRINTF("Returning CKR_FUNCTION_NOT_PARALLEL (%i)", CKR_FUNCTION_NOT_PARALLEL);

	return(CKR_FUNCTION_NOT_PARALLEL);

	hSession = hSession; /* Supress unused variable warning */
}

CK_DEFINE_FUNCTION(CK_RV, C_GetFunctionList)(CK_FUNCTION_LIST_PTR_PTR ppFunctionList) {
	CK_FUNCTION_LIST_PTR pFunctionList;

	CACKEY_DEBUG_PRINTF("Called.");

	if (ppFunctionList == NULL) {
		CACKEY_DEBUG_PRINTF("Error. ppFunctionList is NULL.");

		return(CKR_ARGUMENTS_BAD);
	}

	pFunctionList = malloc(sizeof(*pFunctionList));

	pFunctionList->version.major = ((CACKEY_CRYPTOKI_VERSION_CODE) >> 16) & 0xff;
	pFunctionList->version.minor = ((CACKEY_CRYPTOKI_VERSION_CODE) >> 8) & 0xff;

	pFunctionList->C_Initialize = C_Initialize;
	pFunctionList->C_Finalize = C_Finalize;
	pFunctionList->C_GetInfo = C_GetInfo;
	pFunctionList->C_GetSlotList = C_GetSlotList;
	pFunctionList->C_GetSlotInfo = C_GetSlotInfo;
	pFunctionList->C_GetTokenInfo = C_GetTokenInfo;
	pFunctionList->C_WaitForSlotEvent = C_WaitForSlotEvent;
	pFunctionList->C_GetMechanismList = C_GetMechanismList;
	pFunctionList->C_GetMechanismInfo = C_GetMechanismInfo;
	pFunctionList->C_InitToken = C_InitToken;
	pFunctionList->C_InitPIN = C_InitPIN;
	pFunctionList->C_SetPIN = C_SetPIN;
	pFunctionList->C_OpenSession = C_OpenSession;
	pFunctionList->C_CloseSession = C_CloseSession;
	pFunctionList->C_CloseAllSessions = C_CloseAllSessions;
	pFunctionList->C_GetSessionInfo = C_GetSessionInfo;
	pFunctionList->C_GetOperationState = C_GetOperationState;
	pFunctionList->C_SetOperationState = C_SetOperationState;
	pFunctionList->C_Login = C_Login;
	pFunctionList->C_Logout = C_Logout;
	pFunctionList->C_CreateObject = C_CreateObject;
	pFunctionList->C_CopyObject = C_CopyObject;
	pFunctionList->C_DestroyObject = C_DestroyObject;
	pFunctionList->C_GetObjectSize = C_GetObjectSize;
	pFunctionList->C_GetAttributeValue = C_GetAttributeValue;
	pFunctionList->C_SetAttributeValue = C_SetAttributeValue;
	pFunctionList->C_FindObjectsInit = C_FindObjectsInit;
	pFunctionList->C_FindObjects = C_FindObjects;
	pFunctionList->C_FindObjectsFinal = C_FindObjectsFinal;
	pFunctionList->C_EncryptInit = C_EncryptInit;
	pFunctionList->C_Encrypt = C_Encrypt;
	pFunctionList->C_EncryptUpdate = C_EncryptUpdate;
	pFunctionList->C_EncryptFinal = C_EncryptFinal;
	pFunctionList->C_DecryptInit = C_DecryptInit;
	pFunctionList->C_Decrypt = C_Decrypt;
	pFunctionList->C_DecryptUpdate = C_DecryptUpdate;
	pFunctionList->C_DecryptFinal = C_DecryptFinal;
	pFunctionList->C_DigestInit = C_DigestInit;
	pFunctionList->C_Digest = C_Digest;
	pFunctionList->C_DigestUpdate = C_DigestUpdate;
	pFunctionList->C_DigestKey = C_DigestKey;
	pFunctionList->C_DigestFinal = C_DigestFinal;
	pFunctionList->C_SignInit = C_SignInit;
	pFunctionList->C_Sign = C_Sign;
	pFunctionList->C_SignUpdate = C_SignUpdate;
	pFunctionList->C_SignFinal = C_SignFinal;
	pFunctionList->C_SignRecoverInit = C_SignRecoverInit;
	pFunctionList->C_SignRecover = C_SignRecover;
	pFunctionList->C_VerifyInit = C_VerifyInit;
	pFunctionList->C_Verify = C_Verify;
	pFunctionList->C_VerifyUpdate = C_VerifyUpdate;
	pFunctionList->C_VerifyFinal = C_VerifyFinal;
	pFunctionList->C_VerifyRecoverInit = C_VerifyRecoverInit;
	pFunctionList->C_VerifyRecover = C_VerifyRecover;
	pFunctionList->C_DigestEncryptUpdate = C_DigestEncryptUpdate;
	pFunctionList->C_DecryptDigestUpdate = C_DecryptDigestUpdate;
	pFunctionList->C_SignEncryptUpdate = C_SignEncryptUpdate;
	pFunctionList->C_DecryptVerifyUpdate = C_DecryptVerifyUpdate;
	pFunctionList->C_GenerateKey = C_GenerateKey;
	pFunctionList->C_GenerateKeyPair = C_GenerateKeyPair;
	pFunctionList->C_WrapKey = C_WrapKey;
	pFunctionList->C_UnwrapKey = C_UnwrapKey;
	pFunctionList->C_DeriveKey = C_DeriveKey;
	pFunctionList->C_SeedRandom = C_SeedRandom;
	pFunctionList->C_GenerateRandom = C_GenerateRandom;
	pFunctionList->C_GetFunctionStatus = C_GetFunctionStatus;
	pFunctionList->C_CancelFunction = C_CancelFunction;
	pFunctionList->C_GetFunctionList = C_GetFunctionList;

	*ppFunctionList = pFunctionList;

	CACKEY_DEBUG_PRINTF("Returning CKR_OK (%i)", CKR_OK);

	return(CKR_OK);
}

