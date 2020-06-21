#include <Client.hpp>
#include <Default.hpp>

#include <iostream>
#include <cstring> // memcpy, memset

/*
**	public methods
*/

void Client::connect(boost::asio::ip::tcp::endpoint const& address)
{
	_m_sock.connect(address);
}

void Client::send_compress_request(std::string const& msg_to_compress)
{
	const std::size_t header_len = _m_protocol.get_header_length();
	const std::size_t payload_len = msg_to_compress.size();

	const std::size_t msg_size = header_len + payload_len;

	char *msg = new char[msg_size];

	_m_protocol.insert_header((void *)msg, protocol::Default::RequestType::compress, payload_len);
	std::memcpy(msg + header_len, (void const*)msg_to_compress.data(), payload_len);

	_send_msg(msg, msg_size);

	delete[] msg;
}

void Client::send_get_stats_request()
{
	const std::size_t header_len = _m_protocol.get_header_length();

	char *msg = new char[header_len];

	_m_protocol.insert_header((void *)msg, protocol::Default::RequestType::get_stats);

	_send_msg(msg, header_len);

	delete[] msg;
}

void Client::send_ping_request()
{
	const std::size_t header_len = _m_protocol.get_header_length();

	char *msg = new char[header_len];

	_m_protocol.insert_header((void *)msg, protocol::Default::RequestType::ping);

	_send_msg(msg, header_len);

	delete[] msg;
}

void Client::send_reset_stats_request()
{
	const std::size_t header_len = _m_protocol.get_header_length();

	char *msg = new char[header_len];

	_m_protocol.insert_header((void *)msg, protocol::Default::RequestType::reset_stats);

	_send_msg(msg, header_len);

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
	_m_sock.write_some(boost::asio::buffer(data, len));
}

std::string Client::_get_server_responce()
{
	std::string responce;

	try {
		char buff[1024]{};

		std::size_t len = _m_sock.read_some(boost::asio::buffer(buff));
		responce = buff;
	} catch (std::exception e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}
	return responce;
}
