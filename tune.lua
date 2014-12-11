#! /usr/bin/lua

local bcrypt = require( "bcrypt" )

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

print( bcrypt.tune( 250 ) )
