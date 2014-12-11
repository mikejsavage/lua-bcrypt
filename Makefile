include Makefile.mess

SRCS += src/main.c

CFLAGS += -Wall -Wno-pointer-sign
CFLAGS += -O2 -fPIC -DNDEBUG

OBJS := $(patsubst %.c,%.o,$(SRCS))

all: bcrypt.so

debug: CFLAGS += -ggdb3 -UNDEBUG
debug: all

bcrypt.so: $(OBJS)
	$(CC) -o bcrypt.so $(LDFLAGS) $(OBJS)

clean:
	rm -f bcrypt.so
	rm -f $(OBJS)
