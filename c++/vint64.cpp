// SPDX-License-Identifier: MIT OR Apache-2.0
#include <cstddef>
#include <stdexcept>
#include <vector>
#include <vint64.h>

typedef Vint64 _Vint64;

namespace Absolucy {
class Vint64 {
  private:
	_Vint64 inner;

  public:
	Vint64() { this->inner = vint64_init(0); }

	Vint64(uint64_t value) { this->inner = vint64_init(value); }

	Vint64(int64_t value) { this->inner = vint64_init_signed(value); }

	Vint64(uint8_t* buffer, size_t buffer_length) {
		if (buffer_length == 0) throw std::invalid_argument("Vint64 cannot be initialized with an empty buffer");
		if (vint64_length(buffer[0]) > buffer_length)
			throw std::invalid_argument("Vint64 is larger than the input buffer, likely incomplete or cut off");
		this->inner = vint64_read(buffer, buffer_length);
	}

	Vint64(std::vector<uint8_t> bytes) : Vint64(bytes.data(), bytes.size()) {}

	Vint64(std::vector<std::byte> bytes) : Vint64((uint8_t*)bytes.data(), bytes.size()) {}

	uint64_t value() { return vint64_value(&this->inner); }

	int64_t signed_value() { return vint64_signed_value(&this->inner); }

	std::vector<std::byte> encode() {
		std::vector<std::byte> encoded(this->inner.length);
		std::copy((std::byte*)&this->inner.bytes, (std::byte*)&this->inner.bytes + this->inner.length, encoded.begin());
		return encoded;
	}

	size_t write(uint8_t* buffer, size_t max_buffer_length) {
		return vint64_write(&this->inner, buffer, max_buffer_length);
	}

	size_t write(std::vector<std::byte>& buffer) {
		buffer.insert(
			buffer.end(), (std::byte*)&this->inner.bytes, (std::byte*)&this->inner.bytes + this->inner.length);
		return this->inner.length;
	}
};
} // namespace Absolucy
