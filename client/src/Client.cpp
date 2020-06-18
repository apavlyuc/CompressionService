#include "Client.hpp"

#include <iostream>
#include <cstring> // memcpy, memset
#include <string_view>

namespace {
	enum class RequestType
	{
		ping = 1,
		get_stats = 2,
		reset_stats = 3,
		compress = 4
	};

	std::string	get_header_for_msg(RequestType type, std::string const& msg = "")
	{
		const uint32_t magic_value = 0x53545259;
		const uint16_t payload_length = msg.size();
		const uint16_t request_code = static_cast<uint16_t>(type);

		const std::size_t size_to_allocate = sizeof(magic_value) + sizeof(payload_length) + sizeof(request_code) + 1;

		char *header = new char(size_to_allocate);
		std::memset((void *)header, 0, size_to_allocate);

		std::memcpy((void *)header, (void *)&magic_value, 4);
		std::memcpy((void *)(header + 4), (void *)&payload_length, 2);
		std::memcpy((void *)(header + 6), (void *)&request_code, 2);

		std::string_view view(header, 6);

		//delete header;
		return std::string{view};
	}
} // namespace

/*
**	public methods
*/

void Client::connect(boost::asio::ip::tcp::endpoint const& address)
{
	_sock.connect(address);
}

void Client::send_compress_request(std::string const& msg_to_compress)
{
	std::string const& msg_header = get_header_for_msg(RequestType::compress, msg_to_compress);

	std::cout << "header size: " << msg_header.size() << std::endl;
	std::cout << "msg size: " << msg_to_compress.size() << std::endl;

	_send_msg(msg_header + msg_to_compress);
}

void Client::send_get_stats_request()
{
	_send_msg(get_header_for_msg(RequestType::get_stats));
}

void Client::send_ping_request()
{
	_send_msg(get_header_for_msg(RequestType::ping));
}

void Client::send_reset_stats_request()
{
	_send_msg(get_header_for_msg(RequestType::reset_stats));
}

std::string Client::receive_msg()
{
	std::string msg;

	msg = _get_server_responce();

	return std::move(msg);
}

/*
**	private methods
*/

void	Client::_send_msg(std::string const& msg)
{
	std::cout << "total msg size: " << msg.size() << std::endl;

	_sock.write_some(boost::asio::buffer(msg, msg.size()));
}

std::string Client::_get_server_responce()
{
	std::string responce;

	try {
		char buff[1024]{};

		std::size_t len = _sock.read_some(boost::asio::buffer(buff));
		responce = buff;
	} catch (std::exception e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}
	return responce;
}
