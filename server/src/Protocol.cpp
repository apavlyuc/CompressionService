#include <Protocol.hpp>

std::size_t DefaultProtocol::get_payload_length(std::string const& header) const
{
	return 0;
}
