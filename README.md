A Lua wrapper for OpenBSD's bcrypt.


Requirements
------------

lua >= 5.1


Copying
-------

Many of the files in this repository have been taken from OpenBSD's
tree. You should consult individual file headers for specific licensing
information. More broadly, everything here is compatible with the [ISC
license][ISC].

[ISC]: http://en.wikipedia.org/wiki/ISC_license


Installation
------------

	$ luarocks install bcrypt


Usage
-----

	local bcrypt = require( "bcrypt" )
	
	-- Bigger numbers here will make your digest exponentially harder to compute
	local log_rounds = 9
	
	local digest = bcrypt.digest( "password", log_rounds )
	assert( bcrypt.verify( "password", digest ) )


Security concerns
-----------------

Lua will keep plaintext passwords around in memory as part of its string
interning mechanism. As far as I'm aware, there's nothing I can do about
this.


Tuning
------

If you would like to automatically tune the number of rounds to your
hardware, you can include a function like:

	function bcrypt.tune( t )
		local SAMPLES = 10
		local rounds = 5
	
		while true do
			local total = 0
	
			for i = 1, SAMPLES do
				local start = os.clock()
				bcrypt.digest( "asdf", rounds )
				local delta = os.clock() - start
	
				total = total + delta
			end
	
			if ( total / SAMPLES ) * 1000 >= t then
				return rounds - 1
			end
	
			rounds = rounds + 1
		end
	end

This function returns the largest load factor such that `bcrypt.digest(
str, work )` takes less than `t` milliseconds (assuming your CPU isn't
dodgy).

Note that this will take at least `2 * SAMPLES * t` ms to evaluate.
