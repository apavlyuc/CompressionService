#include <utility.hpp>

namespace protocol {
	bool is_big_endian_machine()
	{
		union {
			uint32_t i;
			char c[4];
		} tmp = {0x01020304};

		return (tmp.c[0] == 1);
	}
} // namespace protocol