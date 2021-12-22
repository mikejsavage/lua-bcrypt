/* Public domain */

#pragma once

#include <stddef.h>

void explicit_bzero( void * buf, size_t n );
int timingsafe_bcmp( const void * b1, const void * b2, size_t n );
