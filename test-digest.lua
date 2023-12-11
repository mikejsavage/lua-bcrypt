#! /usr/bin/env lua

for entry in package.cpath:gmatch( "[^;]+" ) do
	if entry:sub( 1, 2 ) == "./" then
		package.cpath = entry
		break
	end
end

local bcrypt = require( "lua-bcrypt" )

-- some test inputs, mostly taken from john the ripper
local tests = {
	{
		"$2b$04$TnjywYklQbbZjdjBgBoA4e9G7RJt9blgMgsCvUvus4Iv4TENB5nHy",
		"test"
	},
	{
		"$2b$05$CCCCCCCCCCCCCCCCCCCCC.E5YPO9kmyuRGyh0XouQYb4YMJKvyOeW",
		"U*U"
	},
	{
		"$2b$05$CCCCCCCCCCCCCCCCCCCCC.VGOzA784oUp/Z0DY336zx7pLYAy0lwK",
		"U*U*"
	},
	{
		"$2b$05$XXXXXXXXXXXXXXXXXXXXXOAcXxm9kjPGEMsLznoKqmqw7tc8WCx4a",
		"U*U*U"
	},
	{
		"$2b$05$CCCCCCCCCCCCCCCCCCCCC.7uG0VCzI2bS7j6ymqJi9CdcdxiRTWNy",
		""
	},
	{
		"$2b$05$/OK.fbVrR/bpIqNJ5ianF.Sa7shbm4.OzKpvFnX1pQLmQW96oUlCq",
		"\\xa3"
	},
	{
		"$2b$05$/OK.fbVrR/bpIqNJ5ianF.nRht2l/HRhr6zmCp9vYUvvsqynflf9e",
		"\\xff\\xa3345"
	},
	{
		"$2b$05$/OK.fbVrR/bpIqNJ5ianF.6IflQkJytoRVc1yuaNtHfiuq.FRlSIS",
		"\\xa3ab"
	},
	{
		"$2b$05$abcdefghijklmnopqrstuu5s2v8.iXieOjg/.AySBTTZIIVFJeBui",
		"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789chars after 72 are ignored"
	},
	{
		"$2b$05$/OK.fbVrR/bpIqNJ5ianF.swQOIzjOiJ9GHEPuhEkvqrUyvWhEMx6",
		"\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaa\\xaachars after 72 are ignored as usual"
	},
	{
		"$2b$05$/OK.fbVrR/bpIqNJ5ianF.R9xrDjiycxMbQE2bp.vgqlYpW5wx2yy",
		"\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55\\xaa\\x55"
	},
	{
		"$2b$05$CCCCCCCCCCCCCCCCCCCCC.7uG0VCzI2bS7j6ymqJi9CdcdxiRTWNy",
		""
	},
	{
		"$2b$05$/OK.fbVrR/bpIqNJ5ianF.9tQZzcJfm3uj2NvJ/n5xkhpqLrMpWCe",
		"\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff\\x55\\xaa\\xff"
	},
}

for _, test in pairs( tests ) do
	local key = test[ 2 ]:gsub( "\\x([%l%d][%l%d])", function( hex )
		return string.char( tonumber( hex, 16 ) )
	end )

	assert( bcrypt.verify( key, test[ 1 ] ) )
	assert( bcrypt.verify( key, bcrypt.digest( key, 5 ) ) )
end

print( "Tests passed!" )
