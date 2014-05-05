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


Security
--------

You might have noticed that `luabcrypt_verify` doesn't use a constant
time comparison and `luabcrypt_digest` doesn't take steps to zero out
the entropy buffer. I don't think either of these are a problem.

I haven't used a constant time comparison because I'm not sure how to
write one and then guarantee the compiler won't optimise it out. I don't
think this is a big deal because strong hash functions, such as bcrypt,
have good preimage resistance. That said, the code is currently relying
on this holding forever, and if you aren't happy with that you shouldn't
use it.

I don't zero out the entropy buffer after generating a salt for the same
reason. There are two ways in which this can be obtained:

* another program calls `malloc` and gets handed memory that was
  previously used as an entropy buffer
* the entropy buffer is read directly from memory

I don't think it matters because you shouldn't be able to predict the
output of a CSPRNG even given previous outputs. If you are worried about
the second case, note that Lua interns short strings which will include
passwords handled by your application so an attacker can read those
directly instead.
