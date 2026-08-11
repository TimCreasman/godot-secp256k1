/*************************************************************************
 * Copyright (c) 2020-2021 Elichai Turkel                                *
 * Distributed under the CC0 software license, see the accompanying file *
 * EXAMPLES_COPYING or https://creativecommons.org/publicdomain/zero/1.0 *
 *************************************************************************/

/*
 * This file is an attempt at collecting best practice methods for obtaining randomness with different operating systems.
 * It may be out-of-date. Consult the documentation of the operating system before considering to use the methods below.
 *
 * Platform randomness sources:
 * Linux   -> `getrandom(2)`(`sys/random.h`), if not available `/dev/urandom` should be used. http://man7.org/linux/man-pages/man2/getrandom.2.html, https://linux.die.net/man/4/urandom
 * macOS   -> `getentropy(2)`(`sys/random.h`), if not available `/dev/urandom` should be used. https://www.unix.com/man-page/mojave/2/getentropy, https://opensource.apple.com/source/xnu/xnu-517.12.7/bsd/man/man4/random.4.auto.html
 * FreeBSD -> `getrandom(2)`(`sys/random.h`), if not available `kern.arandom` should be used. https://www.freebsd.org/cgi/man.cgi?query=getrandom, https://www.freebsd.org/cgi/man.cgi?query=random&sektion=4
 * OpenBSD -> `getentropy(2)`(`unistd.h`), if not available `/dev/urandom` should be used. https://man.openbsd.org/getentropy, https://man.openbsd.org/urandom
 * Windows -> `BCryptGenRandom`(`bcrypt.h`). https://docs.microsoft.com/en-us/windows/win32/api/bcrypt/nf-bcrypt-bcryptgenrandom
 */

#include <cstring>
#if defined(_WIN32)
/*
 * The defined WIN32_NO_STATUS macro disables return code definitions in
 * windows.h, which avoids "macro redefinition" MSVC warnings in ntstatus.h.
 */
#define WIN32_NO_STATUS
#include <windows.h>
#undef WIN32_NO_STATUS
#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")
#include <ntstatus.h>
#elif defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__)
#include <sys/random.h>

#elif defined(__OpenBSD__)
#include <unistd.h>

#elif defined(__wasm__)
#include <emscripten.h>
#include <cstddef>
#include <cstdint>

#else
#error "Couldn't identify the OS"
#endif

#include <limits.h>
#include <stddef.h>
#include <stdio.h>

int fill_random(unsigned char *data, size_t size);
void print_hex(unsigned char *data, size_t size);
void secure_erase(void *ptr, size_t len);
