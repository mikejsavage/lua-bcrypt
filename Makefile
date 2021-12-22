all:
	luarocks make CFLAGS="-O2 -fPIC -Wall -Wextra" --local

debug:
	luarocks make CFLAGS="-fPIC -g -Wall -Wextra" --local

clean:
	rm -f bcrypt.so src/*.o
