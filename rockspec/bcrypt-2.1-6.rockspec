package = "bcrypt"
version = "2.1-6"

source = {
	url = "git://github.com/mikejsavage/lua-bcrypt.git",
	tag = "v2.1-6",
}

description = {
	summary = "A Lua wrapper for bcrypt",
	homepage = "http://github.com/mikejsavage/lua-bcrypt",
	license = "ISC",
	maintainer = "Mike Savage",
}

dependencies = {
	"lua >= 5.1",
}

build = {
	type = "make",

	install_pass = false,

	build_variables = {
		LUA_INCDIR = "$(LUA_INCDIR)",
                OPENSSL_DIR = "$(OPENSSL_DIR)",
                CRYPTO_DIR = "$(CRYPTO_DIR)", 
	},

	install = {
		lib = {
			[ "bcrypt" ] = "bcrypt.so",
		},
	},
}
