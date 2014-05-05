#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "ow-crypt.h"

#define CRYPT_OUTPUT_SIZE ( 7 + 22 + 31 + 1 )
#define CRYPT_GENSALT_OUTPUT_SIZE ( 7 + 22 + 1 )
#define ENTROPY_SIZE 32
#define DEFAULT_RANDOM "/dev/urandom"

#if LUA_VERSION_NUM < 502
	#define luaL_newlib( L, l ) ( lua_newtable( L ), luaL_register( L, NULL, l ) )
#endif

enum {
	BC_ERR_API = 1,
	BC_ERR_RNG,
	BC_ERR_NORNG,
};

static FILE * random_file = NULL;

static const char * my_strerror( const int err ) {
	switch( err ) {
		case BC_ERR_API:
			return strerror( errno );

		case BC_ERR_RNG:
			return "Bad entropy source";

		case BC_ERR_NORNG:
			return "No entropy source";

		default:
			return ".";
	}
}

// http://insanecoding.blogspot.ca/2014/05/a-good-idea-with-bad-usage-devurandom.html
static int open_random( const char * const path ) {
	FILE * f = fopen( path, "r" );

	if( f == NULL ) {
		return BC_ERR_API;
	}

	int fd = fileno( f );
	struct stat stat_buf;

	if( fd == -1 || fstat( fd, &stat_buf ) != 0 ) {
		( void ) fclose( f );

		return BC_ERR_API;
	}

	// is this a random device?
	if( S_ISCHR( stat_buf.st_mode ) == 0 || ( stat_buf.st_rdev != makedev( 1, 8 ) && stat_buf.st_rdev != makedev( 1, 9 ) ) ) {
		( void ) fclose( f );

		return BC_ERR_RNG;
	}

	random_file = f;

	return 0;
}

static int read_random( char * const buf, const size_t len ) {
	if( random_file == NULL ) {
		return BC_ERR_NORNG;
	}

	size_t bytes_read = fread( buf, 1, len, random_file );

	if( bytes_read < len ) {
		return BC_ERR_API;
	}

	return 0;
}

static int makesalt( char * const salt, const size_t size, const int rounds ) {
	char entropy[ ENTROPY_SIZE ];
	int rv_random = read_random( entropy, sizeof( entropy ) );

	if( rv_random != 0 ) {
		return rv_random;
	}

	const char * const ok = crypt_gensalt_rn( "$2y$", rounds, entropy, sizeof( entropy ), salt, size );

	if( ok == NULL ) {
		return BC_ERR_API;
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
			lua_pushstring( L, my_strerror( rv_salt ) );

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
		lua_pushstring( L, my_strerror( rv ) );

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

static int luabcrypt_random( lua_State * const L ) {
	const char * const path = luaL_checkstring( L, 1 );

	int rv = open_random( path );

	if( rv != 0 ) {
		lua_pushstring( L, my_strerror( rv ) );

		return lua_error( L );
	}

	return 0;
}

static const struct luaL_Reg luabcrypt_lib[] = {
	{ "digest", luabcrypt_digest },
	{ "salt", luabcrypt_salt },
	{ "verify", luabcrypt_verify },
	{ "random", luabcrypt_random },
	{ NULL, NULL },
};

LUALIB_API int luaopen_bcrypt( lua_State * const L ) {
	int rv = open_random( DEFAULT_RANDOM );

	if( rv != 0 ) {
		lua_pushstring( L, my_strerror( rv ) );

		return lua_error( L );
	}

	luaL_newlib( L, luabcrypt_lib );

	return 1;
}
