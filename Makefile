all: bcrypt.so

CFLAGS += -Wall -Wno-pointer-sign
CFLAGS += -O2 -fPIC -DNDEBUG

OBJS := src/main.o src/ggentropy.o src/csprng.o src/monocypher.o

# debug target
debug: CFLAGS += -ggdb3 -UNDEBUG -O0
debug: all

# Let Luarocks point us to the right headers
ifdef LUA_INCDIR
	CFLAGS += -I$(LUA_INCDIR)
endif

uname ?= $(shell uname -s)

# Build a shared library
ifneq ($(uname),Darwin)
	LDFLAGS += -shared
else
	LDFLAGS += -bundle -undefined dynamic_lookup
endif

# Use OS bcrypt/bzero on OpenBSD
ifneq ($(uname),OpenBSD)
	OBJS += compat/bcrypt/bcrypt.o compat/bcrypt/blowfish.o compat/safebfuns.o
endif

# Don't optimise safebfuns
compat/safebfuns.o: CFLAGS += -O0

bcrypt.so: $(OBJS)
	$(CC) -o bcrypt.so $(OBJS) $(LDFLAGS)

clean:
	rm -f bcrypt.so
	rm -f $(OBJS)
