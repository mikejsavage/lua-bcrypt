#pragma once

#include <stddef.h>

#define      _PASSWORD_LEN           128

int             bcrypt_newhash(const char *, int, char *, size_t);
int             bcrypt_checkpass(const char *, const char *);
