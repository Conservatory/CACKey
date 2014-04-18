#ifndef USACEIT_ASN1_X509_H
#define USACEIT_ASN1_X509_H 1

#ifdef HAVE_CONFIG_H
#  include "config.h"
#  ifdef HAVE_UNISTD_H
#    include <unistd.h>
#  endif
#else
#  include <unistd.h>
#endif

static ssize_t x509_to_subject(void *x509_der_buf, size_t x509_der_buf_len, void **outbuf);

static ssize_t x509_to_issuer(void *x509_der_buf, size_t x509_der_buf_len, void **outbuf);

static ssize_t x509_to_serial(void *x509_der_buf, size_t x509_der_buf_len, void **outbuf);

static ssize_t x509_to_pubkey(void *x509_der_buf, size_t x509_der_buf_len, void **outbuf);

static ssize_t x509_to_modulus(void *x509_der_buf, size_t x509_der_buf_len, void **outbuf);

static ssize_t x509_to_exponent(void *x509_der_buf, size_t x509_der_buf_len, void **outbuf);

static ssize_t x509_to_keysize(void *x509_der_buf, size_t x509_der_buf_len);

static ssize_t x509_dn_to_string(void *asn1_der_buf, size_t asn1_der_buf_len, char *outbuf, size_t outbuf_len, char *matchlabel);

#endif
