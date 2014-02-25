A Lua wrapper for [crypt_blowfish](http://www.openwall.com/crypt/).


Requirements
------------

lua >= 5.1


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
