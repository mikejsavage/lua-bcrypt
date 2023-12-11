A Lua wrapper for OpenBSD's bcrypt.


Requirements
------------

lua >= 5.1


Installation
------------

```
$ luarocks install bcrypt
```


Usage
-----

```lua
local bcrypt = require("lua-bcrypt")

-- Bigger numbers here will make your digest exponentially harder to compute
local log_rounds = 9

local digest = bcrypt.digest("password", log_rounds)
assert(bcrypt.verify("password", digest))
```


`require("bcrypt")` vs `require("lua-bcrypt")`
----------------------------------------------

Before lua-bcrypt 2.3-2 you had to use `require("bcrypt")`. I will never drop
support for this so you don't need to modify existing software unless you also
want it to run on Windows.

Windows has a system DLL called bcrypt.dll and the name clash makes
`require("bcrypt")` not work. If you want your software to run on Windows you
must use `require("lua-bcrypt")`


Security concerns
-----------------

Lua will keep plaintext passwords around in memory as part of its string
interning mechanism. As far as I'm aware, there's nothing I can do about this.


Tuning
------

If you would like to automatically tune the number of rounds to your hardware,
you can include a function like:

```lua
function bcrypt.tune(t)
    local SAMPLES = 10
    local rounds = 5

    while true do
        local total = 0

        for i = 1, SAMPLES do
            local start = os.clock()
            bcrypt.digest("asdf", rounds)
            local delta = os.clock() - start

            total = total + delta
        end

        if (total / SAMPLES) * 1000 >= t then
            return rounds - 1
        end

        rounds = rounds + 1
    end
end
```

This function returns the largest load factor such that
`bcrypt.digest(str, work)` takes less than `t` milliseconds.
