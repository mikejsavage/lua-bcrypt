#include <stdint.h>

#include "ggentropy.h"
#include "monocypher.h"

static crypto_chacha_ctx chacha;
static size_t bytes_since_stir;

int CSPRNG_Init() {
	uint8_t entropy[ 32 + 8 ];
	int ok = ggentropy( entropy, sizeof( entropy ) );
	if( ok == -1 )
		return -1;

	bytes_since_stir = 0;

	crypto_chacha20_init( &chacha, entropy, entropy + 32 );

	return 0;
}

static void stir() {
	// stir every 64k
	if( bytes_since_stir < 64000 )
		return;

	uint8_t entropy[ 32 ];
	int ok = ggentropy( entropy, sizeof( entropy ) );
	if( ok == -1 )
		return; // not the end of the world

	uint8_t ciphertext[ 32 ];
	crypto_chacha20_encrypt( &chacha, ciphertext, entropy, sizeof( entropy ) );

	bytes_since_stir = 0;
}

void CSPRNG_Bytes( void * buf, size_t n ) {
	stir();
	bytes_since_stir += n;

	crypto_chacha20_stream( &chacha, ( uint8_t * ) buf, n );
}
