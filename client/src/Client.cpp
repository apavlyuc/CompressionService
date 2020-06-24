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
	_perform_request(protocol::Default::RequestType::compress, msg_to_compress);
}

void Client::send_get_stats_request()
{
	_perform_request(protocol::Default::RequestType::get_stats);
}

void Client::send_ping_request()
{
	_perform_request(protocol::Default::RequestType::ping);
}

void Client::send_reset_stats_request()
{
	_perform_request(protocol::Default::RequestType::reset_stats);
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

void Client::_perform_request(protocol::Default::RequestType type, std::string const& payload)
{
	const std::size_t header_len = protocol::Default::get_header_length();
	const std::size_t payload_len = payload.size();

	const std::size_t msg_size = header_len + payload_len;
	char *msg = new char[msg_size];

	protocol::Default::insert_header((void *)msg, type, payload_len);
	protocol::Default::insert_payload((void *)msg, (void const*)payload.c_str(), payload_len);

	_send_msg(msg, msg_size);

	delete[] msg;
}

void Client::_send_msg(char const* data, std::size_t len)
{
	_m_sock.write_some(boost::asio::buffer(data, len));
}

std::string Client::_get_server_responce()
{
	std::string ret;

	const std::size_t header_len = protocol::Default::get_header_length();
	std::size_t payload_len = 0;

	char *header = nullptr;
	char *payload = nullptr;
	try {
		header = new char[header_len];
		boost::asio::read(_m_sock, boost::asio::buffer(header, header_len));

		payload_len = protocol::Default::get_payload_length(header);
		std::cout << "payload_len: " << payload_len << std::endl;

		payload = new char[payload_len];
		boost::asio::read(_m_sock, boost::asio::buffer(payload, payload_len));

		ret = std::string(payload, payload_len);

	} catch (std::exception e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	delete[] header;
	delete[] payload;

	return ret;
}
