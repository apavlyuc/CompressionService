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

	if (msg.empty() == false)
	{
		protocol::Default::StatusCode status = protocol::Default::get_status_code(msg.c_str());

		switch (status)
		{
			case protocol::Default::StatusCode::unknown_error:
			{
				std::cerr << "server status: unknown_error" << std::endl;
				msg.clear();
				break;
			}
			case protocol::Default::StatusCode::message_too_large:
			{
				std::cerr << "server status: message_too_large" << std::endl;
				msg.clear();
				break;
			}
			case protocol::Default::StatusCode::unsupported_request_type:
			{
				std::cerr << "server status: unsupported_request_type" << std::endl;
				msg.clear();
				break;
			}
			case protocol::Default::StatusCode::message_contains_digits:
			{
				std::cerr << "server status: message_contains_digits" << std::endl;
				msg.clear();
				break;
			}
			case protocol::Default::StatusCode::message_contains_uppercase:
			{
				msg.clear();
				std::cerr << "server status: message_contains_uppercase" << std::endl;
				break;
			}
			default:
			{
				std::cout << "server status: ok" << std::endl;
				msg = msg.substr(protocol::Default::header_len);
			}
		}
	}

	return std::move(msg);
}

/*
**	private methods
*/

void Client::_perform_request(protocol::Default::RequestType type, std::string const& payload)
{
	const std::size_t payload_len = payload.size();

	const std::size_t msg_size = protocol::Default::header_len + payload_len;
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

	char *header = nullptr;
	char *payload = nullptr;
	std::size_t payload_len = 0;
	try {
		header = new char[protocol::Default::header_len];
		boost::asio::read(_m_sock, boost::asio::buffer(header, protocol::Default::header_len));

		payload_len = protocol::Default::get_payload_length(header);

		payload = new char[payload_len];
		boost::asio::read(_m_sock, boost::asio::buffer(payload, payload_len));

		ret = std::string(header, protocol::Default::header_len);
		ret += std::string(payload, payload_len);

	} catch (std::exception e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	delete[] header;
	delete[] payload;

	return ret;
}
