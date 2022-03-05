#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unity.h>
#define _VINT64_INTERNAL
#include <vint64.h>

#define ARRAY(...)                                                                                                     \
	&(uint8_t[]) { __VA_ARGS__ }

#define TEST_VINT64_ENCODE(value, ...)                                                                                 \
	TEST_ASSERT_EQUAL_HEX8_ARRAY(ARRAY(__VA_ARGS__), vint_to_bytes(value), length_as_vint64(value))

void setUp(void) {}

void tearDown(void) {}

// leaks memory but this is a unit test who cares

uint8_t* vint_to_bytes(uint64_t number) {
	Vint64 vint = vint64_init(number);
	uint8_t* bytes = malloc(9);
	memset(bytes, 0, 9);
	memcpy(bytes, vint.bytes, vint.length);
	return bytes;
}

void test_encode_0(void) { TEST_VINT64_ENCODE(0, 1); }

void test_encode_bit_patterns(void) {
	TEST_VINT64_ENCODE(0x0f0f, 0x3e, 0x3c);
	TEST_VINT64_ENCODE(0x0f0ff0f0, 0x08, 0x0f, 0xff, 0xf0);
	TEST_VINT64_ENCODE(0x0f0ff0f00f0f, 0xc0, 0x87, 0x07, 0x78, 0xf8, 0x87, 0x07);
	TEST_VINT64_ENCODE(0x0f0ff0f00f0ff0f0, 0x00, 0xf0, 0xf0, 0x0f, 0x0f, 0xf0, 0xf0, 0x0f, 0x0f);
}

void test_encode_maxint(void) { TEST_VINT64_ENCODE(UINT64_MAX, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff); }

void test_encode_signed(void) {
	TEST_VINT64_ENCODE(zigzag_encode(0x0f0ff0f0), 0x10, 0x3c, 0xfc, 0xc3, 0x03);
	TEST_VINT64_ENCODE(zigzag_encode(-0x0f0ff0f0), 0xf0, 0x3b, 0xfc, 0xc3, 0x03);
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_encode_0);
	RUN_TEST(test_encode_bit_patterns);
	RUN_TEST(test_encode_maxint);
	RUN_TEST(test_encode_signed);
	return UNITY_END();
}
