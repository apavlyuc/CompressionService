#pragma once

#include <string>

namespace protocol {

	/*
	**	protocol:	Default
	**
	**	structure:	magic_number + payload_len + request_code
	**		magic_number:	uint32_t, always equal to 0x53545259
	**		payload_len:	uint16_t
	**		request_code:	uint16_t
	*/
	struct Default
	{
		enum class RequestType
		{
			ping = 1,
			get_stats = 2,
			reset_stats = 3,
			compress = 4
		};

		enum class StatusCode
		{
			ok = 0,
			unknown_error = 1,
			message__too_large = 2,
			unsupported_request_type = 3
		};

		static inline std::size_t get_header_length() { return Default::_header_length; }
		static std::size_t get_payload_length(char const* header);
		static RequestType get_request_type(char const* header);
		static StatusCode get_status_code(char const* header);

		static void	insert_header(void* dst, RequestType type, const uint16_t payload_length = 0);
		static void	insert_header(void* dst, StatusCode code, const uint16_t payload_length = 0);
		static void	insert_payload(void *dst, void const* src, std::size_t src_len);
	private:
		static inline std::size_t get_offset_to_payload_length_field() { return Default::_offset_to_payload_length_field; }

		static const std::size_t _header_length{8};
		static const std::size_t _offset_to_payload_length_field{4};
	};

} // namespace protocol