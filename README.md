A Lua wrapper for the OpenBSD implementation of bcrypt. Note that it
will run on other operating systems.


Requirements
------------

luabcrypt requires OpenSSL (for salt generation) and Lua to compile.


Usage
-----

	require( "bcrypt" )

	local salt = bcrypt.salt( 5 )
	local digest = bcrypt.digest( "password", salt )

	assert( bcrypt.verify( "password", digest ) )
