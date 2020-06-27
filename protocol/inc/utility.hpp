#pragma once

#include <algorithm>
#include <array>

namespace protocol {
	bool is_big_endian_machine();

	template<typename T>
	void change_endian(T& val)
	{
		union U {
			T val;
			std::array<std::uint8_t, sizeof(T)> raw_val;
		} src, dst;

		src.val = val;
		std::reverse_copy(src.raw_val.begin(), src.raw_val.end(), dst.raw_val.begin());
		val = dst.val;
	}
} // namespace protocol
