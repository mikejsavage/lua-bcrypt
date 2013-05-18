CFLAGS = -O2 -shared -fPIC -fomit-frame-pointer -funroll-loops
LDFLAGS = -lcrypto -Ilib/bcrypt
WARNINGS = -Wall -Wextra -Werror -Wbad-function-cast -Wcast-align -Wcast-qual -Wstrict-prototypes -Wshadow -Wundef -Wpointer-arith

BCRYPT_DIR = lib/bcrypt
BCRYPT_OBJS = $(BCRYPT_DIR)/crypt_blowfish.o $(BCRYPT_DIR)/x86.o $(BCRYPT_DIR)/crypt_gensalt.o $(BCRYPT_DIR)/wrapper.o

all: crypto_blowfish bcrypt

crypto_blowfish:
	make -C lib/bcrypt

bcrypt: src/main.c
	${CC} -o $@.so $^ ${BCRYPT_OBJS} ${CFLAGS} ${LDFLAGS} ${WARNINGS}

test:
	make -C lib/bcrypt check
	@lua tests.lua

clean:
	make -C lib/bcrypt clean
