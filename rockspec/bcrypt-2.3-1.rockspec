package = "bcrypt"
version = "2.3-1"

source = {
	url = "git+https://github.com/mikejsavage/lua-bcrypt.git",
	tag = "v2.3-1",
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
	type = "builtin",
	modules = {
		bcrypt = {
			"src/main.c",
			"src/bcrypt.c",
			"src/blowfish.c",
			"src/ggentropy.c",
			"src/safebfuns.c",
		}
	},
}
