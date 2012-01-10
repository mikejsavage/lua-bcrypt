CC = gcc
SRCDIR = src/
CFLAGS = -O2 -shared -fPIC
LDFLAGS = -lcrypto
WARNINGS = -Wall -Wextra -Werror

all: bcrypt test

bcrypt: ${SRCDIR}luabcrypt.c ${SRCDIR}bcrypt.c ${SRCDIR}blowfish.c
	${CC} -o $@.so $^ ${CFLAGS} ${LDFLAGS} ${WARNINGS}

test:
	@lua tests.lua
