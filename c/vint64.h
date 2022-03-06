// SPDX-License-Identifier: MIT OR Apache-2.0
#ifndef VINT64_H
#define VINT64_H

#include <stddef.h>
#include <stdint.h>
#if defined(_WIN32) || defined(__CYGWIN__)
#if defined(__GNUC__) || defined(__GNUG__)
#define EXPORT __attribute__((dllexport))
#else
#define EXPORT __declspec(dllexport)
#endif
#else
#define EXPORT __attribute__((visibility("default")))
#endif

#ifdef _VINT64_INTERNAL
#if defined(__GNUC__) ||                                                                                               \
	defined(__GNUG__) // clang anc gcc have __builtin_ctz, which does pretty much exactly what we want
#define trailing_zeros __builtin_ctz
#define leading_zeros __builtin_clzll
#if defined(__clang__)
#define assume(x) __builtin_assume(x)
#else
// Source: https://stackoverflow.com/a/63494072
#define assume(x)                                                                                                      \
	if (!(x)) __builtin_unreachable()
#endif
#elif defined(_MSC_VER) // msvc has _BitScanForward, which is about the same thing but slightly less convienient
#include <intrin.h>
#define assume __assume
static inline int trailing_zeros(uint32_t value) {
	unsigned long trailing_zero = 0;
	if (_BitScanForward(&trailing_zero, value)) {
		assume(trailing_zero >= 0 && trailing_zero <= 32);
		return trailing_zero;
	} else {
		return 32;
	}
}
static inline int leading_zeros(uint64_t value) {
	unsigned long leading_zero = 0;
	if (_BitScanReverse64(&leading_zero, value)) {
		return 63 - leading_zero;
	} else {
		return 64;
	}
}
#else // unknown compiler
// Source: https://stackoverflow.com/a/7813486
static inline int ctz(uint32_t value) {
	unsigned int v;		 // 32-bit word input to count zero bits on right
	unsigned int c = 32; // c will be the number of zero bits on the right
	v &= -signed(v);
	if (v) c--;
	if (v & 0x0000FFFF) c -= 16;
	if (v & 0x00FF00FF) c -= 8;
	if (v & 0x0F0F0F0F) c -= 4;
	if (v & 0x33333333) c -= 2;
	if (v & 0x55555555) c -= 1;
	return c;
}

// Source: https://stackoverflow.com/a/40943402
static inline int leading_zeros(uint64_t x) {
	unsigned long y;
	long n, c;
	n = 64;
	c = 32;
	do {
		y = x >> c;
		if (y != 0) {
			n = n - c;
			x = y;
		}
		c = c >> 1;
	} while (c != 0);
	return n - x;
}
// we can't rely on compiler extensions
#define assume(x)
#endif

static inline uint64_t zigzag_encode(int64_t value) { return (uint64_t)((value << 1) ^ (value >> 63)); }

static inline int64_t zigzag_decode(uint64_t encoded) { return (int64_t)(encoded >> 1) ^ -((int64_t)(encoded & 1)); }
#endif

typedef struct Vint64 {
	uint8_t bytes[9];
	size_t length;
} Vint64;

/**
 * Gets the length that an unsigned integer would have when encoded as Vint64.
 * @param value The unsigned integer to measure the Vint64 length of.
 * @return The length of a vint64 containing the given value.
 */
EXPORT uint8_t length_as_vint64(uint64_t value);
/**
 * Gets the length that an signed integer would have when encoded as Vint64.
 * @param value The signed integer to measure the Vint64 length of.
 * @return The length of a vint64 containing the given value.
 */
EXPORT uint8_t length_as_signed_vint64(int64_t value);
/**
 * Initializes a Vint64 with the given unsigned integer.
 * @param value The unsigned integer to initialize the Vint64 with.
 * @return The initialized Vint64.
 */
EXPORT Vint64 vint64_init(uint64_t value);
/**
 * Initializes a Vint64 with the given signed integer.
 * @param value The signed integer to initialize the Vint64 with.
 * @return The initialized Vint64.
 */
EXPORT Vint64 vint64_init_signed(int64_t value);
/**
 * Gets the length of an encoded vint64, given the first byte of it.
 * @param first_byte The first byte of the Vint64.
 * @return The length of the Vint64.
 */
EXPORT uint8_t vint64_length(uint8_t first_byte);
/**
 * Decodes a Vint64 into an unsigned 64-bit integer.
 * @param vint The Vint64 to decode.
 * @return The decoded unsigned integer.
 */
EXPORT uint64_t vint64_value(Vint64* vint);
/**
 * Decodes a Vint64 into a signed 64-bit integer
 * @param vint The Vint64 to decode.
 * @return The decoded signed integer.
 */
EXPORT int64_t vint64_signed_value(Vint64* vint);
/**
 * Writes the encoded vint64 to the given buffer.
 * If the buffer is too small, 0 bytes will be written.
 * @param vint The Vint64 to write.
 * @param buffer The buffer to write to.
 * @param max_buffer_length The size of the target buffer.
 * @return The number of bytes written to the buffer.
 */
EXPORT size_t vint64_write(Vint64* vint, uint8_t* buffer, size_t max_buffer_length);
/**
 * Reads a vint64 from the given buffer.
 * If the buffer is too small, a Vint64 with a value of 0 will be returned.
 * @param buffer The buffer to read from.
 * @param buffer_length The size of the buffer.
 * @return The Vint64 value.
 */
EXPORT Vint64 vint64_read(uint8_t* buffer, size_t buffer_length);
#endif
