#! /usr/bin/lua

local bcrypt = require( "bcrypt" )

local salt = bcrypt.salt( 5 )
assert( salt:len() == 29 )

local digest = bcrypt.digest( salt, salt )
assert( bcrypt.verify( salt, digest ) )
assert( not bcrypt.verify( salt:gsub( "(.)$", function( ch )
	return string.char( ( ch:byte() + 1 ) % 255 )
end ), digest ) )

local tests = {
	{
		"$2y$04$TnjywYklQbbZjdjBgBoA4e9G7RJt9blgMgsCvUvus4Iv4TENB5nHy",
		"test"
	},

	-- rest are taken from John the Ripper
	-- http://cvsweb.openwall.com/cgi/cvsweb.cgi/Owl/packages/john/john/src/BF_fmt.c?rev=HEAD
	{
		"$2y$05$CCCCCCCCCCCCCCCCCCCCC.E5YPO9kmyuRGyh0XouQYb4YMJKvyOeW",
		"U*U"
	},
	{
		"$2y$05$CCCCCCCCCCCCCCCCCCCCC.VGOzA784oUp/Z0DY336zx7pLYAy0lwK",
		"U*U*"
	},
	{
		"$2y$05$XXXXXXXXXXXXXXXXXXXXXOAcXxm9kjPGEMsLznoKqmqw7tc8WCx4a",
		"U*U*U"
	},
	{
		"$2y$05$CCCCCCCCCCCCCCCCCCCCC.7uG0VCzI2bS7j6ymqJi9CdcdxiRTWNy",
		""
	},
	{
		"$2y$05$/OK.fbVrR/bpIqNJ5ianF.Sa7shbm4.OzKpvFnX1pQLmQW96oUlCq",
		"\\xa3"
	},
	{
		"$2y$05$/OK.fbVrR/bpIqNJ5ianF.nRht2l/HRhr6zmCp9vYUvvsqynflf9e",
		"\\xff\\xa3345"
	},
	{
		"$2y$05$/OK.fbVrR/bpIqNJ5ianF.6IflQkJytoRVc1yuaNtHfiuq.FRlSIS",
		"\\xa3ab"
	},
	{
		"$2y$05$abcdefghijklmnopqrstuu5s2v8.iXieOjg/.AySBTTZIIVFJeBui",
		"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789chars after 72 are ignored"
	},
	{
		"$2y$05$/OK.fbVrR/bpIqNJ5ianF.swQOIzjOiJ9GHEPuhEkvqrUyvWhEMx6",
		"\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaachars after 72 are ignored as usual"
	},
	{
		"$2y$05$/OK.fbVrR/bpIqNJ5ianF.R9xrDjiycxMbQE2bp.vgqlYpW5wx2yy",
		"\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55"
	},
	{
		"$2y$05$CCCCCCCCCCCCCCCCCCCCC.7uG0VCzI2bS7j6ymqJi9CdcdxiRTWNy",
		""
	},
	{
		"$2y$05$/OK.fbVrR/bpIqNJ5ianF.9tQZzcJfm3uj2NvJ/n5xkhpqLrMpWCe",
		"\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff"
	},
}

for _, test in pairs( tests ) do
	local key = test[ 2 ]:gsub( "\\x([%l%d][%l%d])", function( hex )
		return string.char( tonumber( hex, 16 ) )
	end )

	assert( bcrypt.verify( key, test[ 1 ] ) )
end

print( "Tests passed!" )
