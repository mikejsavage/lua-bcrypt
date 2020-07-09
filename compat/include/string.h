/*
 * Public domain
 * string.h compatibility shim
 */

#include_next <string.h>

#ifndef LIBCRYPTOCOMPAT_STRING_H
#define LIBCRYPTOCOMPAT_STRING_H

void explicit_bzero(void *, size_t);
int timingsafe_bcmp(const void *b1, const void *b2, size_t n);

#endif
