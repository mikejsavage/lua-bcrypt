/*
 * stdlib.h compatibility shim
 * Public domain
 */

#include_next <stdlib.h>

#ifndef LIBCRYPTOCOMPAT_STDLIB_H
#define LIBCRYPTOCOMPAT_STDLIB_H

#include <stdint.h>

uint32_t arc4random(void);
void arc4random_buf(void *_buf, size_t n);

#endif
