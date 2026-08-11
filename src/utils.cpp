#include "utils.h"

#if defined(__wasm__)
// Define a secure JS replacement for getrandom
EM_JS(int, em_secure_getrandom, (void *data, size_t size), {
	try {
		// Map the Wasm memory buffer directly to a JS typed array
		// Module.HEAPU8 provides access to the shared Wasm linear memory
		var bytes = new Uint8Array(Module.HEAPU8.buffer, data, size);

		// Fills the memory buffer with cryptographically secure values
		crypto.getRandomValues(bytes);
		return 1; // Success
	} catch (e) {
		return 0; // Failure (e.g. environment missing Crypto API)
	}
});
#endif

/* Returns 1 on success, and 0 on failure. */
int fill_random(unsigned char *data, size_t size) {
#if defined(_WIN32)
	NTSTATUS res = BCryptGenRandom(NULL, data, size, BCRYPT_USE_SYSTEM_PREFERRED_RNG);
	if (res != STATUS_SUCCESS || size > ULONG_MAX) {
		return 0;
	} else {
		return 1;
	}
#elif defined(__linux__) || defined(__FreeBSD__)
	/* If `getrandom(2)` is not available you should fallback to /dev/urandom */
	ssize_t res = getrandom(data, size, 0);
	if (res < 0 || (size_t)res != size) {
		return 0;
	} else {
		return 1;
	}
#elif defined(__APPLE__) || defined(__OpenBSD__)
	/* If `getentropy(2)` is not available you should fallback to either
	 * `SecRandomCopyBytes` or /dev/urandom */
	int res = getentropy(data, size);
	if (res == 0) {
		return 1;
	} else {
		return 0;
	}
#elif defined(__wasm__)
	int res = em_secure_getrandom(data, size);
	if (res == 0) {
		return 0;
	} else {
		return 1;
	}
#endif
	return 0;
}

void print_hex(unsigned char *data, size_t size) {
	size_t i;
	printf("0x");
	for (i = 0; i < size; i++) {
		printf("%02x", data[i]);
	}
	printf("\n");
}

#if defined(_MSC_VER)
// For SecureZeroMemory
#include <Windows.h>
#endif

/* Cleanses memory to prevent leaking sensitive info. Won't be optimized out. */
void secure_erase(void *ptr, size_t len) {
#if defined(_MSC_VER)
	/* SecureZeroMemory is guaranteed not to be optimized out by MSVC. */
	SecureZeroMemory(ptr, len);
#elif defined(__GNUC__)
	/* We use a memory barrier that scares the compiler away from optimizing out the memset.
	 *
	 * Quoting Adam Langley <agl@google.com> in commit ad1907fe73334d6c696c8539646c21b11178f20f
	 * in BoringSSL (ISC License):
	 *    As best as we can tell, this is sufficient to break any optimisations that
	 *    might try to eliminate "superfluous" memsets.
	 * This method used in memzero_explicit() the Linux kernel, too. Its advantage is that it is
	 * pretty efficient, because the compiler can still implement the memset() efficiently,
	 * just not remove it entirely. See "Dead Store Elimination (Still) Considered Harmful" by
	 * Yang et al. (USENIX Security 2017) for more background.
	 */
	memset(ptr, 0, len);
	__asm__ __volatile__("" : : "r"(ptr) : "memory");
#else
	void *(*volatile const volatile_memset)(void *, int, size_t) = memset;
	volatile_memset(ptr, 0, len);
#endif
}
