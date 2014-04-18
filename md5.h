/* MD5.H - header file for MD5.C
 */

/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.
 */

#ifndef _MD5_H_
#define _MD5_H_

#include "config.h"

#ifdef HAVE_STDINT_H
#  include <stdint.h>
#endif
#ifdef HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#define MD5HashSize 16

/* MD5 context. */
typedef struct {
	uint32_t state[4];       /* state (ABCD) */
	uint32_t count[2];       /* number of bits, modulo 2^64 (lsb first) */
	uint8_t buffer[64];      /* input buffer */
} MD5_CTX;

static void MD5Init(MD5_CTX *);
static void MD5Update(MD5_CTX *, unsigned char *, unsigned int);
static void MD5Final(unsigned char [MD5HashSize], MD5_CTX *);

#endif
