A Lua wrapper for [crypt_blowfish](http://www.openwall.com/crypt/).


Requirements
------------

lua 5.2


Usage
-----

	require( "bcrypt" )

	local salt = bcrypt.salt( 5 )
	local digest = bcrypt.digest( "password", salt )

	assert( bcrypt.verify( "password", digest ) )
