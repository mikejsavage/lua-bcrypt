package = "bcrypt"
version = "2.3-2"

source = {
	url = "git+https://github.com/mikejsavage/lua-bcrypt.git",
	tag = "v2.3-2",
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

local srcs = {
	"src/main.c",
	"src/bcrypt.c",
	"src/blowfish.c",
	"src/ggentropy.c",
	"src/safebfuns.c",
}

build = {
	type = "builtin",
	modules = { [ "lua-bcrypt" ] = srcs },
	platforms = {
		unix = { modules = { bcrypt = srcs } },
	},
}
