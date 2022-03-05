#include <stdbool.h>
#include <string.h>
#include <unity.h>
#define _VINT64_INTERNAL
#include <vint64.h>

#define ARRAY(...) (uint8_t*)(&(uint8_t[]){__VA_ARGS__})

#define TEST_VINT64_DECODE(value, ...) TEST_ASSERT_EQUAL_UINT64(value, bytes_to_vint(ARRAY(__VA_ARGS__)))
#define TEST_VINT64_DECODE_SIGNED(value, ...)                                                                          \
	TEST_ASSERT_EQUAL_UINT64(value, zigzag_decode(bytes_to_vint(ARRAY(__VA_ARGS__))))

void setUp(void) {}

void tearDown(void) {}

uint64_t bytes_to_vint(uint8_t* bytes) {
	Vint64 vint = vint64_read(bytes, 9);
	return vint64_value(&vint);
}

void test_decode_zero(void) { TEST_VINT64_DECODE(0, 0x01); }

void test_decode_bit_patterns(void) {
	TEST_VINT64_DECODE(0x0f0f, 0x3e, 0x3c);
	TEST_VINT64_DECODE(0x0f0ff0f0, 0x08, 0x0f, 0xff, 0xf0);
	TEST_VINT64_DECODE(0x0f0ff0f00f0f, 0xc0, 0x87, 0x07, 0x78, 0xf8, 0x87, 0x07);
	TEST_VINT64_DECODE(0x0f0ff0f00f0ff0f0, 0x00, 0xf0, 0xf0, 0x0f, 0x0f, 0xf0, 0xf0, 0x0f, 0x0f);
}

void test_decode_maxint(void) { TEST_VINT64_DECODE(UINT64_MAX, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff); }

void test_decode_with_trailing_data(void) { TEST_VINT64_DECODE(0x0f0f, 0x3e, 0x3c, 0xde, 0xad, 0xbe, 0xef); }

void test_decode_signed_values(void) {
	TEST_VINT64_DECODE_SIGNED(0x0f0ff0f0, 0x10, 0x3c, 0xfc, 0xc3, 0x03);
	TEST_VINT64_DECODE_SIGNED(-0x0f0ff0f0, 0xf0, 0x3b, 0xfc, 0xc3, 0x03);
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_decode_zero);
	RUN_TEST(test_decode_bit_patterns);
	RUN_TEST(test_decode_maxint);
	RUN_TEST(test_decode_with_trailing_data);
	RUN_TEST(test_decode_signed_values);
	return UNITY_END();
}
