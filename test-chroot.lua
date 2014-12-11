#! /usr/bin/lua

local setuid = require( "setuid" )

assert( setuid.chroot( "." ) )
assert( not io.open( "/etc/passwd", "r" ) )

local bcrypt = require( "bcrypt" )

print( "It works!" )
print( bcrypt.digest( "adsf", 5 ) )
