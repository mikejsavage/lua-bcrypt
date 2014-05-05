A Lua wrapper for [crypt_blowfish](http://www.openwall.com/crypt/).


Requirements
------------

lua >= 5.1


Installation
------------

	$ luarocks install bcrypt


Usage
-----

	local bcrypt = require( "bcrypt" )
	local rounds = 5 -- tune this as appropriate
	
	local digest = bcrypt.digest( "password", rounds )
	assert( bcrypt.verify( "password", digest ) )

You can also explicitly pass a salt to `bcrypt.digest`:

	local bcrypt = require( "bcrypt" )
	local rounds = 5 -- tune this as appropriate
	
	local salt = bcrypt.salt( rounds )
	local digest = bcrypt.digest( "password", salt )
	
	assert( bcrypt.verify( "password", digest ) )

If you want to use a different random device, you can use
`bcrypt.random`:

	local bcrypt = require( "bcrypt" )
	bcrypt.random( "/dev/random" )
	local digest = bcrypt.digest( "password", rounds )
	
	-- bcrypt.random( "/dev/null" ) - fails


Chroot
------

If you want to use `lua-bcrypt` from inside a chroot, `/dev/urandom`
must still exist when `require( "bcrypt" )` is called, even if you are
planning to use a different random device. The reasoning behind this is
that chroots are a less common use case of this library, and we should
fail as early as possible on errors in the common use case.

Therefore, if you wish to use chroot, you should run:

	mknod -m 644 /path/to/chroot/dev/urandom c 1 9
