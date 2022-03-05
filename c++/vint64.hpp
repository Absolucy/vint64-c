// SPDX-License-Identifier: MIT OR Apache-2.0
#ifndef VINT64PP_H
#define VINT64PP_H

#include <cstddef>
#include <vector>
#include <vint64.h>

typedef Vint64 _Vint64;

namespace Absolucy {
class Vint64 {
  private:
	_Vint64 inner;

  public:
	Vint64();
	Vint64(uint64_t value);
	Vint64(int64_t value);
	Vint64(uint8_t* buffer, size_t buffer_length);
	Vint64(std::vector<uint8_t> bytes);
	Vint64(std::vector<std::byte> bytes);
	uint64_t value();
	int64_t signed_value();
	std::vector<std::byte> encode();
	size_t write(uint8_t* buffer, size_t max_buffer_length);
	size_t write(std::vector<std::byte>& buffer);
};
} // namespace Absolucy
#endif
