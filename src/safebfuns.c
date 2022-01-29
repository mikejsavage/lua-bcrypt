/* Public domain */

#include <string.h>

#if defined( _MSC_VER )
	#pragma optimize( "", off )
#elif defined( __clang__ )
	#pragma clang optimize off
#elif defined( __GNUC__ )
	#pragma GCC optimize ( "O0" )
#else
	#error new compiler
#endif

void explicit_bzero( void * buf, size_t n ) {
	volatile char * volbuf = ( volatile char * ) buf;

	size_t i;
	for( i = 0; i < n; i++ ) {
		volbuf[ i ] = 0;
	}
}

int timingsafe_bcmp( const void * b1, const void * b2, size_t n ) {
	const unsigned char * p1 = b1;
	const unsigned char * p2 = b2;
	int result = 0;

	size_t i;
	for( i = 0; i < n; i++ ) {
		result |= p1[ i ] ^ p2[ i ];
	}

	return result != 0;
}
