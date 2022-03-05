// SPDX-License-Identifier: MIT OR Apache-2.0
#define _VINT64_INTERNAL
#include "vint64.h"
#include <stdio.h>
#include <string.h>

size_t length_as_vint64(uint64_t value) {
	static const size_t lookup_table[65] = {9, 9, 9, 9, 9, 9, 9, 9, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7, 7,
											6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 3,
											3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1};
	int leading = leading_zeros(value);
	assume(leading >= 0 && leading <= 64);
	return lookup_table[leading];
}

size_t length_as_signed_vint64(int64_t value) { return length_as_vint64(zigzag_encode(value)); }

Vint64 vint64_init(uint64_t value) {
	Vint64 vint;
	memset(&vint, 0, sizeof(Vint64));
	vint.length = length_as_vint64(value);
	if (vint.length == 9) {
		memcpy((uint8_t*)&vint.bytes + 1, &value, 8);
	} else {
		uint64_t encoded = (value << 1 | 1) << (vint.length - 1);
		memcpy(&vint.bytes, &encoded, 8);
	}
	return vint;
}

Vint64 vint64_init_signed(int64_t value) { return vint64_init(zigzag_encode(value)); }

size_t vint64_length(uint8_t first_byte) {
	if (first_byte == 0) return 9;
	return trailing_zeros(first_byte) + 1;
}

uint64_t vint64_value(Vint64* vint) {
	uint64_t result = 0;
	if (vint->length == 9) {
		memcpy(&result, (uint8_t*)&vint->bytes + 1, 8);
	} else {
		memcpy(&result, &vint->bytes, vint->length);
		result >>= vint->length;
	}
	if (vint->length == 1 || result >= (1ULL << (7ULL * (vint->length - 1ULL)))) {
		return result;
	} else {
		return 0;
	}
}

int64_t vint64_signed_value(Vint64* vint) { return zigzag_decode(vint64_value(vint)); }

size_t vint64_write(Vint64* vint, uint8_t* buffer, size_t max_buffer_length) {
	if (vint->length > max_buffer_length) return 0;
	memcpy(buffer, &vint->bytes, vint->length);
	return vint->length;
}

Vint64 vint64_read(uint8_t* buffer, size_t buffer_length) {
	if (buffer_length < 1) return vint64_init(0);
	size_t length = vint64_length(buffer[0]);
	if (length > buffer_length) return vint64_init(0);
	Vint64 vint;
	memset(&vint, 0, sizeof(Vint64));
	vint.length = length;
	memcpy(&vint.bytes, buffer, length);
	return vint;
}
