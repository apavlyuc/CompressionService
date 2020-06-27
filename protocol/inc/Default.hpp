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
	namespace Default
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
			message_too_large = 2,
			unsupported_request_type = 3,
			message_contains_digits = 4,
			message_contains_uppercase = 5
		};

		std::size_t get_payload_length(char const* header);
		RequestType get_request_type(char const* header);
		StatusCode get_status_code(char const* header);

		void insert_header(void* dst, RequestType type, uint16_t payload_length = 0);
		void insert_header(void* dst, StatusCode code, uint16_t payload_length = 0);
		void insert_payload(void *dst, void const* src, std::size_t src_len);

		static const std::size_t header_len{8};
	} // namespace Default

} // namespace protocol