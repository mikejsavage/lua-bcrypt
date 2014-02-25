#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "ow-crypt.h"

#define CRYPT_OUTPUT_SIZE ( 7 + 22 + 31 + 1 )
#define CRYPT_GENSALT_OUTPUT_SIZE ( 7 + 22 + 1 )
#define ENTROPY_SIZE 32

#if LUA_VERSION_NUM < 502
	#define luaL_newlib( L, l ) ( lua_newtable( L ), luaL_register( L, NULL, l ) ) 
#endif

static int urandom;

static int makesalt( char * const salt, const size_t size, const int rounds ) {
	char entropy[ ENTROPY_SIZE ];
	const ssize_t bytes = read( urandom, entropy, sizeof( entropy ) );

	if( bytes != sizeof( entropy ) ) {
		return 1;
	}

	const char * const ok = crypt_gensalt_rn( "$2y$", rounds, entropy, sizeof( entropy ), salt, size );

	if( ok == NULL ) {
		return 1;
	}

	return 0;
}

// bcrypt.digest( key, [salt/rounds] )
static int luabcrypt_digest( lua_State * const L ) {
	const char * const key = luaL_checkstring( L, 1 );
	const long rounds = lua_tointeger( L, 2 );

	char hash[ CRYPT_OUTPUT_SIZE ];
	memset( hash, 0, sizeof( hash ) );

	char newsalt[ CRYPT_GENSALT_OUTPUT_SIZE ];
	const char * salt = NULL;

	if( rounds != 0 ) {
		int rv_salt = makesalt( newsalt, sizeof( newsalt ), rounds );

		if( rv_salt != 0 ) {
			lua_pushstring( L, strerror( errno ) );

			return lua_error( L );
		}

		salt = newsalt;
	}
	else {
		salt = luaL_checkstring( L, 2 );
	}

	const char * const rv_crypt = crypt_rn( key, salt, hash, sizeof( hash ) );

	if( rv_crypt == NULL ) {
		lua_pushstring( L, strerror( errno ) );

		return lua_error( L );
	}

	lua_pushstring( L, hash );

	return 1;
}

// bcrypt.salt( rounds )
static int luabcrypt_salt( lua_State * const L ) {
	const unsigned long rounds = luaL_checkinteger( L, 1 );

	char salt[ CRYPT_GENSALT_OUTPUT_SIZE ];
	int rv = makesalt( salt, sizeof( salt ), rounds );

	if( rv != 0 ) {
		lua_pushstring( L, strerror( errno ) );

		return lua_error( L );
	}

	lua_pushstring( L, salt );

	return 1;
}

// bcrypt.verify( key, digest )
static int luabcrypt_verify( lua_State * const L ) {
	const char * const key = luaL_checkstring( L, 1 );
	const char * const digest = luaL_checkstring( L, 2 );

	char hash[ CRYPT_OUTPUT_SIZE ];
	memset( hash, 0, sizeof( hash ) );

	crypt_rn( key, digest, hash, sizeof( hash ) );

	const int verified = strncmp( hash, digest, sizeof( hash ) ) == 0;

	lua_pushboolean( L, verified );

	return 1;
}

static const struct luaL_Reg luabcrypt_lib[] = {
	{ "digest", luabcrypt_digest },
	{ "salt", luabcrypt_salt },
	{ "verify", luabcrypt_verify },
	{ NULL, NULL },
};

LUALIB_API int luaopen_bcrypt( lua_State * const L ) {
	urandom = open( "/dev/urandom", O_RDONLY );

	if( urandom == -1 ) {
		lua_pushstring( L, strerror( errno ) );

		return lua_error( L );
	}

	luaL_newlib( L, luabcrypt_lib );

	return 1;
}
