#include "Client.hpp"

#include <iostream>
#include <cstring> // memcpy, memset

namespace {
	enum class RequestType
	{
		ping = 1,
		get_stats = 2,
		reset_stats = 3,
		compress = 4
	};

	const std::size_t g_header_size = sizeof(uint32_t) + sizeof(uint16_t) + sizeof(uint16_t);

	void	insert_header(void* dst, RequestType type, const uint16_t payload_length = 0)
	{
		const uint32_t magic_value = 0x53545259;
		const uint16_t request_code = static_cast<uint16_t>(type);

		std::memcpy(dst, (void *)&magic_value, 4);
		std::memcpy((void *)((char *)dst + 4), (void *)&payload_length, 2);
		std::memcpy((void *)((char *)dst + 6), (void *)&request_code, 2);
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
	std::size_t msg_size = g_header_size + msg_to_compress.size();
	char *msg = new char[msg_size];

	insert_header(msg, RequestType::compress, msg_to_compress.size());
	std::memcpy(msg + g_header_size, (void const*)msg_to_compress.data(), msg_to_compress.size());

	_send_msg(msg, msg_size);

	delete[] msg;
}

void Client::send_get_stats_request()
{
	char *msg = new char[g_header_size];

	insert_header(msg, RequestType::get_stats);

	_send_msg(msg, g_header_size);

	delete[] msg;
}

void Client::send_ping_request()
{
	char *msg = new char[g_header_size];

	insert_header(msg, RequestType::ping);

	_send_msg(msg, g_header_size);

	delete[] msg;
}

void Client::send_reset_stats_request()
{
	char *msg = new char[g_header_size];

	insert_header(msg, RequestType::reset_stats);

	_send_msg(msg, g_header_size);

	delete[] msg;
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

void	Client::_send_msg(char const* data, std::size_t len)
{
	_sock.write_some(boost::asio::buffer(data, len));
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
