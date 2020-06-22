#include <Default.hpp>

#include <cstring>

namespace protocol {

	std::size_t Default::get_payload_length(char const* header) const
	{
		uint16_t payload_length{0};

		std::memcpy((void *)&payload_length, (void *)(header + get_offset_to_payload_length_field()), sizeof(payload_length));
		return payload_length;
	}

	Default::RequestType Default::get_request_type(char const* header) const
	{
		uint16_t request_type = 0;

		std::memcpy((void *)&request_type, (void const *)(header + 6), sizeof(request_type));

		return static_cast<Default::RequestType>(request_type);
	}

	void	Default::insert_header(void* dst, RequestType type, const uint16_t payload_length) const
	{
		const uint32_t magic_value = 0x53545259;
		const uint16_t request_code = static_cast<uint16_t>(type);

		std::memcpy(dst, (void const *)&magic_value, sizeof(magic_value));
		std::memcpy((void *)((char *)dst + 4), (void const *)&payload_length, sizeof(payload_length));
		std::memcpy((void *)((char *)dst + 6), (void const *)&request_code, sizeof(request_code));
	}

	void	Default::insert_payload(void *dst, void const* src, std::size_t src_len) const
	{
		std::memcpy((void*)((char *)dst + Default::_header_length), src, src_len);
	}

} // namespace protocol