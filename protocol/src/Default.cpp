#include <Default.hpp>

#include <cstring>

namespace protocol {

	std::size_t Default::get_payload_length(char const* header, std::size_t size) const
	{
		uint16_t payload_length{0};

		std::memcpy((void *)&payload_length, (void *)(header + get_offset_to_payload_length_field()), sizeof(payload_length));
		return payload_length;
	}

	void	Default::insert_header(void* dst, RequestType type, const uint16_t payload_length)
	{
		const uint32_t magic_value = 0x53545259;
		const uint16_t request_code = static_cast<uint16_t>(type);

		std::memcpy(dst, (void *)&magic_value, 4);
		std::memcpy((void *)((char *)dst + 4), (void *)&payload_length, 2);
		std::memcpy((void *)((char *)dst + 6), (void *)&request_code, 2);
	}

} // namespace protocol