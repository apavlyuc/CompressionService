#pragma once

#include <string>

namespace protocol {

	class Default
	{
		static const std::size_t _header_length{8};
		static const std::size_t _offset_to_payload_length_field{4};
	public:
		enum class RequestType
		{
			ping = 1,
			get_stats = 2,
			reset_stats = 3,
			compress = 4
		};

		inline std::size_t get_header_length() const { return Default::_header_length; }
		std::size_t get_payload_length(char const* header, std::size_t size) const;

		void	insert_header(void* dst, RequestType type, const uint16_t payload_length = 0);
	protected:
		inline std::size_t get_offset_to_payload_length_field() const { return Default::_offset_to_payload_length_field; }
	};

} // namespace protocol