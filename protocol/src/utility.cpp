#include <utility.hpp>

namespace protocol {
	bool is_big_endian_machine()
	{
		static const int i = 1;

		char *ptr = (char *)&i;
		return (*ptr == 0);
	}
} // namespace protocol