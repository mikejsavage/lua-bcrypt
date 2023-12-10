#pragma once

#include <stddef.h>
#include <Windows.h>

#define      _PASSWORD_LEN           128

int             bcrypt_newhash(const char *, int, char *, size_t);
int             bcrypt_checkpass(const char *, const char *);
NTSTATUS WINAPI BCryptGenRandom(
    BCRYPT_ALG_HANDLE hAlgorithm,
    PUCHAR pbBuffer,
    ULONG cbBuffer,
    ULONG dwFlags
);
