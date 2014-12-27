/*
 * Copyright (c) 2014, Michael Savage <mike@mikejsavage.co.uk>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <sys/types.h>

#include <pwd.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#if LUA_VERSION_NUM < 502
	#define luaL_newlib( L, l ) ( lua_newtable( L ), luaL_register( L, NULL, l ) )
#endif

static int luabcrypt_digest( lua_State * const L ) {
	const char * const password = luaL_checkstring( L, 1 );
	const int log_rounds = lua_tointeger( L, 2 );

	char hash[ _PASSWORD_LEN ];

	int rv = bcrypt_newhash( password, log_rounds, hash, sizeof( hash ) );

	if( rv != 0 ) {
		lua_pushliteral( L, "bcrypt_newhash failed" );

		return lua_error( L );
	}

	lua_pushstring( L, hash );

	return 1;
}

static int luabcrypt_verify( lua_State * const L ) {
	const char * const password = luaL_checkstring( L, 1 );
	const char * const goodhash = luaL_checkstring( L, 2 );

	const int ok = !bcrypt_checkpass( password, goodhash );

	lua_pushboolean( L, ok );

	return 1;
}

static const struct luaL_Reg luabcrypt_lib[] = {
	{ "digest", luabcrypt_digest },
	{ "verify", luabcrypt_verify },
	{ NULL, NULL },
};

LUALIB_API int luaopen_bcrypt( lua_State * const L ) {
	luaL_newlib( L, luabcrypt_lib );

	return 1;
}
