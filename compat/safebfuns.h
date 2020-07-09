/* Public domain */

#include <string.h>

#if __clang__
	/*
	 * http://clang.llvm.org/docs/LanguageExtensions.html#feature-checking-macros
	 * http://lists.cs.uiuc.edu/pipermail/cfe-dev/2014-December/040627.html
	 */
	#if __has_attribute( noinline ) && __has_attribute( optnone )
		#define NOOPT __attribute__ (( optnone ))
		#define NOINLINE __attribute__ (( noinline ))
	#else
		#error "require clang with noinline and optnone attributes"
	#endif
#elif __GNUC__
	/*
	 * http://gcc.gnu.org/onlinedocs/gcc/Function-Specific-Option-Pragmas.html
	 * http://gcc.gnu.org/onlinedocs/gcc/Function-Attributes.html
	 */
	#if __GNUC__ > 4 || ( __GNUC__ == 4 && __GNUC_MINOR__ >= 4 )
		#define NOOPT __attribute__ (( optimize( 0 ) ))
		#define NOINLINE __attribute__ (( noinline ))
	#else
		#error "require gcc >= 4.4"
	#endif
#else
	#error "unrecognised compiler"
	explode
#endif

NOOPT NOINLINE void explicit_bzero( void * const buf, const size_t n );
NOOPT NOINLINE int timingsafe_bcmp( const void * const b1, const void * const b2, const size_t n );
NOOPT NOINLINE int timingsafe_memcmp( const void * const b1, const void * const b2, const size_t len );

#ifndef __SAFEBFUNS_INTERNAL
#undef NOOPT
#undef NOINLINE
#endif
