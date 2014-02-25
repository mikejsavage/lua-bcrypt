package = "bcrypt"
version = "1.3-1"

source = {
	url = "git://github.com/mikejsavage/lua-bcrypt.git",
	tag = "v1.3",
}

description = {
	summary = "A Lua wrapper for bcrypt",
	homepage = "http://github.com/mikejsavage/lua-bcrypt",
	license = "MIT",
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
	},

	install = {
		lib = {
			[ "bcrypt" ] = "bcrypt.so",
		},
	},
}
