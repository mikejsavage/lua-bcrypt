#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "blf.h"

// bcrypt.digest( key, salt )
static int luabcrypt_digest( lua_State *L )
{
	const char *key = luaL_checkstring( L, 1 );
	const char *salt = luaL_checkstring( L, 2 );

	char *hash = malloc( _PASSWORD_LEN );
	bcrypt( key, salt, hash );

	lua_pushstring( L, hash );

	free( hash );

	return 1;
}

// bcrypt.gensalt( logRounds )
static int luabcrypt_gensalt( lua_State *L )
{
	u_int8_t log_rounds = luaL_checkinteger( L, 1 );

	// because the OpenBSD implementation says so
	char *salt = malloc( 7 + ( BCRYPT_MAXSALT * 4 + 2 ) / 3 + 1 );
	bcrypt_gensalt( log_rounds, salt );

	lua_pushstring( L, salt );

	free( salt );

	return 1;
}

// bcrypt.verify( key, digest )
static int luabcrypt_verify( lua_State *L )
{
	const char *key = luaL_checkstring( L, 1 );
	const char *digest = luaL_checkstring( L, 2 );

	char *encrypted = malloc( _PASSWORD_LEN );
	bcrypt( key, digest, encrypted );

	int verified = strncmp( encrypted, digest, _PASSWORD_LEN ) == 0;

	lua_pushboolean( L, verified );

	free( encrypted );

	return 1;
}

struct luaL_reg luabcrypt_lib[] =
{
	{ "digest", luabcrypt_digest },
	{ "gensalt", luabcrypt_gensalt },
	{ "verify", luabcrypt_verify },
	{ NULL, NULL },
};

LUALIB_API int luaopen_bcrypt( lua_State *L )
{
	luaL_openlib( L, "bcrypt", luabcrypt_lib, 0 );

	return 1;
}
