/* Public domain */

#include <string.h>

#if __clang__
	/*
	 * http://clang.llvm.org/docs/LanguageExtensions.html#feature-checking-macros
	 * https://www.mail-archive.com/cfe-commits@cs.uiuc.edu/msg97459.html
	 */
	#if __clang_major__ > 3 || ( __clang_major__ == 3 && __clang_minor__ >= 5 )
		#pragma clang optimize off
		#define NOINLINE __attribute__ (( noinline ))
	#else
		#error "require clang >= 3.5"
	#endif
#elif __GNUC__
	/*
	 * http://gcc.gnu.org/onlinedocs/gcc/Function-Specific-Option-Pragmas.html
	 * http://gcc.gnu.org/onlinedocs/gcc/Function-Attributes.html
	 */
	#if __GNUC__ > 4 || ( __GNUC__ == 4 && __GNUC_MINOR__ >= 4 )
		#pragma GCC push_options
		#pragma GCC optimize ( "-O0" )
		#define NOINLINE __attribute__ (( noinline ))
	#else
		#error "require gcc >= 4.4"
	#endif
#else
	#error "unrecognised compiler"
#endif

NOINLINE void explicit_bzero( void * const buf, const size_t n ) {
	size_t i;
	unsigned char * p = buf;

	for( i = 0; i < n; i++ ) {
		p[ i ] = 0;
	}
}

NOINLINE int timingsafe_bcmp( const void * const b1, const void * const b2, const size_t n ) {
	size_t i;
	const unsigned char * const p1 = b1;
	const unsigned char * const p2 = b2;
	int result = 0;

	for( i = 0; i < n; i++ ) {
		result |= p1[ i ] ^ p2[ i ];
	}

	return result != 0;
}

/*	$OpenBSD: timingsafe_memcmp.c,v 1.1 2014/06/13 02:12:17 matthew Exp $	*/
/*
 * Copyright (c) 2014 Google Inc.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <limits.h>
#include <string.h>

NOINLINE int timingsafe_memcmp( const void * const b1, const void * const b2, const size_t len ) {
	const unsigned char * p1 = b1;
	const unsigned char * p2 = b2;
	size_t i;
	int res = 0, done = 0;

	for( i = 0; i < len; i++ ) {
		/* lt is -1 if p1[i] < p2[i]; else 0. */
		int lt = ( p1[ i ] - p2[ i ] ) >> CHAR_BIT;

		/* gt is -1 if p1[i] > p2[i]; else 0. */
		int gt = ( p2[ i ] - p1[ i ] ) >> CHAR_BIT;

		/* cmp is 1 if p1[i] > p2[i]; -1 if p1[i] < p2[i]; else 0. */
		int cmp = lt - gt;

		/* set res = cmp if !done. */
		res |= cmp & ~done;

		/* set done if p1[i] != p2[i]. */
		done |= lt | gt;
	}

	return res;
}

/* Public domain */

#ifdef __clang__
	#pragma clang optimize on
#else
	#pragma GCC pop_options
#endif
