#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "ow-crypt.h"

#define HASH_SIZE 184
#define SALT_SIZE 128
#define ENTROPY_SIZE 32

#if LUA_VERSION_NUM < 502
	#define luaL_newlib( L, l ) ( lua_newtable( L ), luaL_register( L, NULL, l ) ) 
#endif

static int urandom;

// bcrypt.digest( key, salt )
static int luabcrypt_digest( lua_State* L ) {
	const char* key = luaL_checkstring( L, 1 );
	const char* salt = luaL_checkstring( L, 2 );

	char hash[ HASH_SIZE ];
	memset( hash, 0, sizeof( hash ) );

	crypt_rn( key, salt, hash, sizeof( hash ) );

	lua_pushstring( L, hash );

	return 1;
}

void randomBytes( char* output, size_t bytes ) {
	read( urandom, output, bytes );
}

// bcrypt.salt( logRounds )
static int luabcrypt_salt( lua_State* L ) {
	unsigned long logRounds = luaL_checkinteger( L, 1 );

	char entropy[ ENTROPY_SIZE ];
	char salt[ SALT_SIZE ];

	randomBytes( entropy, sizeof( entropy ) );
	crypt_gensalt_rn( "$2y$", logRounds, entropy, sizeof( entropy ), salt, sizeof( salt ) );

	lua_pushstring( L, salt );

	return 1;
}

// bcrypt.verify( key, digest )
static int luabcrypt_verify( lua_State* L ) {
	const char* key = luaL_checkstring( L, 1 );
	const char* digest = luaL_checkstring( L, 2 );

	char hash[ HASH_SIZE ];
	memset( hash, 0, sizeof( hash ) );

	crypt_rn( key, digest, hash, sizeof( hash ) );

	int verified = strncmp( hash, digest, sizeof( hash ) ) == 0;

	lua_pushboolean( L, verified );

	return 1;
}

static struct luaL_Reg luabcrypt_lib[] = {
	{ "digest", luabcrypt_digest },
	{ "salt", luabcrypt_salt },
	{ "verify", luabcrypt_verify },
	{ NULL, NULL },
};

LUALIB_API int luaopen_bcrypt( lua_State* L ) {
	urandom = open( "/dev/urandom", O_RDONLY );

	if( urandom == -1 ) {
		lua_pushstring( L, strerror( errno ) );

		return lua_error( L );
	}

	luaL_newlib( L, luabcrypt_lib );

	return 1;
}
