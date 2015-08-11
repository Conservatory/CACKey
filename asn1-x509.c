/*
 * Basic implementation of ITU-T X.690 (07/2002) for parsing BER encoded
 * X.509 certificates
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif
#ifdef HAVE_STDLIB_H
#  include <stdlib.h>
#endif
#ifdef HAVE_STDARG_H
#  include <stdarg.h>
#endif
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#ifdef HAVE_STRING_H
#  include <string.h>
#endif

#include "asn1-x509.h"

struct asn1_object {
	unsigned long tag;
	unsigned long size;
	void *contents;

	unsigned long asn1rep_len;
	void *asn1rep;
};

struct x509_object {
	struct asn1_object wholething;
		struct asn1_object certificate;
			struct asn1_object version;
			struct asn1_object serial_number;
			struct asn1_object signature_algo;
			struct asn1_object issuer;
			struct asn1_object validity;
			struct asn1_object subject;
			struct asn1_object pubkeyinfo;
				struct asn1_object pubkey_algoid;
					struct asn1_object pubkey_algo;
					struct asn1_object pubkey_algoparm;
				struct asn1_object pubkey;
};

static int _asn1_x509_read_asn1_object(unsigned char *buf, size_t buflen, va_list *args) {
	unsigned char small_object_size;
	unsigned char *buf_p;
	struct asn1_object *outbuf;

	outbuf = va_arg(*args, struct asn1_object *);

	if (outbuf == NULL) {
		return(0);
	}

	if (buflen == 0) {
		return(-1);
	}

	buf_p = buf;

	outbuf->tag = *buf_p;
	buf_p++;
	buflen--;
<<<<<<< HEAD
	if (buflen == 0) {
		return(-1);
	}

	/* NULL Tag -- no size is required */
	if (outbuf->tag == 0x00) {
		return(_asn1_x509_read_asn1_object(buf_p, buflen, args));
	}

=======

	/* NULL Tag -- no size is required */
	if (outbuf->tag == 0x00) {
		outbuf->size = 0;
		outbuf->asn1rep_len = 1;
		outbuf->asn1rep = buf;

		return(_asn1_x509_read_asn1_object(buf_p, buflen, args));
	}

	if (buflen == 0) {
		return(-1);
	}

>>>>>>> trunk
	small_object_size = *buf_p;
	buf_p++;
	buflen--;
	if (buflen == 0) {
		return(-1);
	}

	if ((small_object_size & 0x80) == 0x80) {
		outbuf->size = 0;

		for (small_object_size ^= 0x80; small_object_size; small_object_size--) {
			outbuf->size <<= 8;
			outbuf->size += *buf_p;

			buf_p++;
			buflen--;
<<<<<<< HEAD
=======

>>>>>>> trunk
			if (buflen == 0) {
				break;
			}
		}
	} else {
		outbuf->size = small_object_size;
	}

	if (outbuf->size > buflen) {
		return(-1);
	}

<<<<<<< HEAD
	outbuf->contents = buf_p;
=======
	if (buflen != 0) {
		outbuf->contents = buf_p;
	}

>>>>>>> trunk
	outbuf->asn1rep_len = outbuf->size + (buf_p - buf);
	outbuf->asn1rep = buf;

	buf_p += outbuf->size;
	buflen -= outbuf->size;

	return(_asn1_x509_read_asn1_object(buf_p, buflen, args));
}

static int asn1_x509_read_asn1_object(unsigned char *buf, size_t buflen, ...) {
	va_list args;
	int retval;

	va_start(args, buflen);

	retval = _asn1_x509_read_asn1_object(buf, buflen, &args);

	va_end(args);

	return(retval);
}

static int asn1_x509_read_object(unsigned char *buf, size_t buflen, struct x509_object *outbuf) {
	int read_ret;

	read_ret = asn1_x509_read_asn1_object(buf, buflen, &outbuf->wholething, NULL);
	if (read_ret != 0) {
<<<<<<< HEAD
=======
		CACKEY_DEBUG_PRINTF("Failed at reading the contents from the wrapper")

>>>>>>> trunk
		return(-1);
	}

	read_ret = asn1_x509_read_asn1_object(outbuf->wholething.contents, outbuf->wholething.size, &outbuf->certificate, NULL);
	if (read_ret != 0) {
<<<<<<< HEAD
=======
		CACKEY_DEBUG_PRINTF("Failed at reading the certificate from the contents");

>>>>>>> trunk
		return(-1);
	}

	read_ret = asn1_x509_read_asn1_object(outbuf->certificate.contents, outbuf->certificate.size, &outbuf->version, &outbuf->serial_number, &outbuf->signature_algo, &outbuf->issuer, &outbuf->validity, &outbuf->subject, &outbuf->pubkeyinfo, NULL);
	if (read_ret != 0) {
<<<<<<< HEAD
=======
		CACKEY_DEBUG_PRINTF("Failed at reading the certificate components from the certificate");

>>>>>>> trunk
		return(-1);
	}

	read_ret = asn1_x509_read_asn1_object(outbuf->pubkeyinfo.contents, outbuf->pubkeyinfo.size, &outbuf->pubkey_algoid, &outbuf->pubkey, NULL);
	if (read_ret != 0) {
<<<<<<< HEAD
=======
		CACKEY_DEBUG_PRINTF("Failed at reading the public key from the certificate components");

>>>>>>> trunk
		return(-1);
	}

	return(0);
}

ssize_t x509_to_issuer(void *x509_der_buf, size_t x509_der_buf_len, void **outbuf) {
	struct x509_object x509;
	int read_ret;

	read_ret = asn1_x509_read_object(x509_der_buf, x509_der_buf_len, &x509);
	if (read_ret != 0) {
		return(-1);
	}

	if (outbuf) {
		*outbuf = x509.issuer.asn1rep;
	}

	return(x509.issuer.asn1rep_len);
}

ssize_t x509_to_subject(void *x509_der_buf, size_t x509_der_buf_len, void **outbuf) {
	struct x509_object x509;
	int read_ret;

	read_ret = asn1_x509_read_object(x509_der_buf, x509_der_buf_len, &x509);
	if (read_ret != 0) {
		return(-1);
	}

	if (outbuf) {
		*outbuf = x509.subject.asn1rep;
	}

	return(x509.subject.asn1rep_len);
}

static ssize_t x509_to_serial(void *x509_der_buf, size_t x509_der_buf_len, void **outbuf) {
	struct x509_object x509;
	int read_ret;

	read_ret = asn1_x509_read_object(x509_der_buf, x509_der_buf_len, &x509);
	if (read_ret != 0) {
<<<<<<< HEAD
=======
		CACKEY_DEBUG_PRINTF("Unable to read serial number from a %lu byte buffer", x509_der_buf_len);
		CACKEY_DEBUG_PRINTBUF("X.509 DER:", x509_der_buf, x509_der_buf_len);

>>>>>>> trunk
		return(-1);
	}

	if (outbuf) {
		*outbuf = x509.serial_number.asn1rep;
	}

	return(x509.serial_number.asn1rep_len);
}

static ssize_t x509_to_pubkey(void *x509_der_buf, size_t x509_der_buf_len, void **outbuf) {
	struct x509_object x509;
	int read_ret;

	read_ret = asn1_x509_read_object(x509_der_buf, x509_der_buf_len, &x509);
	if (read_ret != 0) {
		return(-1);
	}

	*outbuf = x509.pubkey.contents;
	return(x509.pubkey.size);
}

static ssize_t x509_to_modulus(void *x509_der_buf, size_t x509_der_buf_len, void **outbuf) {
	struct asn1_object null, pubkey, modulus, exponent;
	struct x509_object x509;
	int read_ret;

	read_ret = asn1_x509_read_object(x509_der_buf, x509_der_buf_len, &x509);
	if (read_ret != 0) {
		return(-1);
	}

	/* The structure of "pubkey" is specified in PKCS #1 */
	read_ret = asn1_x509_read_asn1_object(x509.pubkey.contents, x509.pubkey.size, &null, &pubkey, NULL);
	if (read_ret != 0) {
		return(-1);
	}

	read_ret = asn1_x509_read_asn1_object(pubkey.contents, pubkey.size, &modulus, &exponent, NULL);
	if (read_ret != 0) {
		return(-1);
	}

	if (outbuf) {
		*outbuf = modulus.contents;
	}

	return(modulus.size);
}

static ssize_t x509_to_exponent(void *x509_der_buf, size_t x509_der_buf_len, void **outbuf) {
	struct asn1_object null, pubkey, modulus, exponent;
	struct x509_object x509;
	int read_ret;

	read_ret = asn1_x509_read_object(x509_der_buf, x509_der_buf_len, &x509);
	if (read_ret != 0) {
		return(-1);
	}

	/* The structure of "pubkey" is specified in PKCS #1 */
	read_ret = asn1_x509_read_asn1_object(x509.pubkey.contents, x509.pubkey.size, &null, &pubkey, NULL);
	if (read_ret != 0) {
		return(-1);
	}

	read_ret = asn1_x509_read_asn1_object(pubkey.contents, pubkey.size, &modulus, &exponent, NULL);
	if (read_ret != 0) {
		return(-1);
	}

	if (outbuf) {
		*outbuf = exponent.contents;
	}

	return(exponent.size);
}

static ssize_t x509_to_keysize(void *x509_der_buf, size_t x509_der_buf_len) {
	struct asn1_object null, pubkey, modulus, exponent;
	struct x509_object x509;
	int read_ret;

	read_ret = asn1_x509_read_object(x509_der_buf, x509_der_buf_len, &x509);
	if (read_ret != 0) {
		return(-1);
	}

	/* The structure of "pubkey" is specified in PKCS #1 */
	read_ret = asn1_x509_read_asn1_object(x509.pubkey.contents, x509.pubkey.size, &null, &pubkey, NULL);
	if (read_ret != 0) {
		return(-1);
	}

	read_ret = asn1_x509_read_asn1_object(pubkey.contents, pubkey.size, &modulus, &exponent, NULL);
	if (read_ret != 0) {
		return(-1);
	}

	return(modulus.size - 1);
}

/*
 * http://www.blackberry.com/developers/docs/4.6.0api/javax/microedition/pki/Certificate.html
 */
static const char *_x509_objectid_to_label_string(void *buf, size_t buflen) {
	switch (buflen) {
		case 3:
			if (memcmp(buf, "\x55\x04\x03", 3) == 0) {
				return("CN");
			}
			if (memcmp(buf, "\x55\x04\x04", 3) == 0) {
				return("SN");
			}
			if (memcmp(buf, "\x55\x04\x06", 3) == 0) {
				return("C");
			}
			if (memcmp(buf, "\x55\x04\x07", 3) == 0) {
				return("L");
			}
			if (memcmp(buf, "\x55\x04\x08", 3) == 0) {
				return("ST");
			}
			if (memcmp(buf, "\x55\x04\x09", 3) == 0) {
				return("STREET");
			}
			if (memcmp(buf, "\x55\x04\x0A", 3) == 0) {
				return("O");
			}
			if (memcmp(buf, "\x55\x04\x0B", 3) == 0) {
				return("OU");
			}
			break;
		case 9:
			if (memcmp(buf, "\x2A\x86\x48\x86\xF7\x0D\x01\x09\x01", 9) == 0) {
				return("EmailAddress");
			}
			break;
	}

	return("???");
}

static ssize_t x509_dn_to_string(void *asn1_der_buf, size_t asn1_der_buf_len, char *outbuf, size_t outbuf_len, char *matchlabel) {
	struct asn1_object whole_thing, current_set, current_seq;
	struct asn1_object label, value;
	const char *label_str;
	ssize_t snprintf_ret, retval;
	char *outbuf_s;
	int read_ret;
	int offset;

	if (outbuf == NULL) {
		return(-1);
	}

	if (outbuf_len == 0 || asn1_der_buf_len == 0 || asn1_der_buf == NULL) {
		return(0);
	}

	read_ret = asn1_x509_read_asn1_object(asn1_der_buf, asn1_der_buf_len, &whole_thing, NULL);
	if (read_ret != 0) {
		return(-1);
	}

	/* Terminate string, in case no valid elements are found we still return a valid string */
	*outbuf = '\0';
	outbuf_s = outbuf;

	offset = 0;
	while (1) {
<<<<<<< HEAD
		read_ret = asn1_x509_read_asn1_object(whole_thing.contents + offset, whole_thing.size - offset, &current_set, NULL);
=======
		read_ret = asn1_x509_read_asn1_object(((unsigned char *) whole_thing.contents) + offset, whole_thing.size - offset, &current_set, NULL);
>>>>>>> trunk
		if (read_ret != 0) {
			break;
		}

		offset += current_set.size + 2;

		read_ret = asn1_x509_read_asn1_object(current_set.contents, current_set.size, &current_seq, NULL);
		if (read_ret != 0) {
			break;
		}

		read_ret = asn1_x509_read_asn1_object(current_seq.contents, current_seq.size, &label, &value, NULL);

		label_str = _x509_objectid_to_label_string(label.contents, label.size);

		/* If the user requested only certain labels, exclude others */
		if (matchlabel) {
			if (strcmp(matchlabel, label_str) != 0) {
				continue;
			}
		}

		/* If the user requested only certain labels, don't include them in the reply */
		if (matchlabel) {
			snprintf_ret = snprintf(outbuf, outbuf_len, "%.*s, ", (unsigned int) value.size, (char *) value.contents);
		} else {
			snprintf_ret = snprintf(outbuf, outbuf_len, "%s=%.*s, ", label_str, (unsigned int) value.size, (char *) value.contents);
		}
		if (snprintf_ret < 0) {
			break;
		}

		if (snprintf_ret > outbuf_len) {
			snprintf_ret = outbuf_len;
		}

		outbuf += snprintf_ret;
		outbuf_len -= snprintf_ret;

		if (outbuf_len < 2) {
			break;
		}
	}

	retval = outbuf - outbuf_s;

	/* Remove trailing ", " added by cumulative process, if found. */
	if (retval > 2) {
		if (outbuf_s[retval - 2] == ',') {
			outbuf_s[retval - 2] = '\0';
			retval -= 2;
		}
	}

	return(retval);
}
