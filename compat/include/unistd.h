/*
 * Public domain
 * unistd.h compatibility shim
 */

#include_next <unistd.h>

#ifndef LIBCRYPTOCOMPAT_UNISTD_H
#define LIBCRYPTOCOMPAT_UNISTD_H

int getentropy(void *buf, size_t buflen);

#endif
