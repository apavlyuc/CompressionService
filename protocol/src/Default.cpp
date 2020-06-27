#include <Default.hpp>
#include <utility.hpp>

#include <cstring>

namespace protocol {

	const bool g_is_big_endian_machin = is_big_endian_machine();

	namespace Default {
		namespace {
			const std::size_t _g_offset_to_payload_length_field{4};
		}

		std::size_t get_payload_length(char const* header)
		{
			uint16_t payload_length{0};

			std::memcpy((void *)&payload_length, (void *)(header + _g_offset_to_payload_length_field), sizeof(payload_length));

			if (g_is_big_endian_machin == false)
			{
				change_endian(payload_length);
			}

			return payload_length;
		}

		RequestType get_request_type(char const* header)
		{
			uint16_t request_type = 0;

			std::memcpy((void *)&request_type, (void const *)(header + 6), sizeof(request_type));

			if (g_is_big_endian_machin == false)
			{
				change_endian(request_type);
			}

			return static_cast<RequestType>(request_type);
		}

		StatusCode get_status_code(char const* header)
		{
			uint16_t status_code = 0;

			std::memcpy((void *)&status_code, (void const *)(header + 6), sizeof(status_code));

			if (g_is_big_endian_machin == false)
			{
				change_endian(status_code);
			}

			return static_cast<StatusCode>(status_code);
		}

		void	insert_header(void* dst, RequestType type, uint16_t payload_length)
		{
			uint32_t magic_value = 0x53545259;
			uint16_t request_code = static_cast<uint16_t>(type);

			if (g_is_big_endian_machin == false)
			{
				change_endian(magic_value);
				change_endian(request_code);
				change_endian(payload_length);
			}

			std::memcpy(dst, (void const *)&magic_value, sizeof(magic_value));
			std::memcpy((void *)((char *)dst + 4), (void const *)&payload_length, sizeof(payload_length));
			std::memcpy((void *)((char *)dst + 6), (void const *)&request_code, sizeof(request_code));
		}

		void	insert_header(void* dst, StatusCode code, uint16_t payload_length)
		{
			uint32_t magic_value = 0x53545259;
			uint16_t status_code = static_cast<uint16_t>(code);

			if (g_is_big_endian_machin == false)
			{
				change_endian(magic_value);
				change_endian(status_code);
				change_endian(payload_length);
			}

			std::memcpy(dst, (void const *)&magic_value, sizeof(magic_value));
			std::memcpy((void *)((char *)dst + 4), (void const *)&payload_length, sizeof(payload_length));
			std::memcpy((void *)((char *)dst + 6), (void const *)&status_code, sizeof(status_code));
		}

		void	insert_payload(void *dst, void const* src, std::size_t src_len)
		{
			std::memcpy((void*)((char *)dst + header_len), src, src_len);
		}
	} // namespace Default

} // namespace protocol