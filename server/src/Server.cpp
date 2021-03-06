#include <Server.hpp>
#include <Compressors.hpp>

#include <boost/thread.hpp>

#include <iostream>

/*
**	public methods
*/

void Server::listen(boost::asio::ip::tcp::endpoint const& addr)
{
	boost::asio::ip::tcp::acceptor accept_manager(_m_io_context, addr);

	while (true)
	{
		socket_shared_ptr sock = std::make_shared<boost::asio::ip::tcp::socket>(_m_io_context);
		accept_manager.accept(*sock);
		boost::thread trd(boost::bind(Server::_client_session, this, sock));
	}
}

/*
**	private methods
*/

void Server::_client_session(socket_shared_ptr sock)
{
	Stats stats;

	char *header = nullptr;
	char *payload = nullptr;
	try {
		while (true)
		{
			header = new char[protocol::Default::header_len];
			boost::asio::read(*sock, boost::asio::buffer(header, protocol::Default::header_len));

			stats.m_total_msgs_received++;

			const protocol::Default::RequestType type = protocol::Default::get_request_type(header);
			const std::size_t payload_length = protocol::Default::get_payload_length(header);

			if (payload_length >= Server::_max_payload_len)
			{
				std::cout << "message_too_large" << std::endl;
				_send_responce(sock, protocol::Default::StatusCode::message_too_large);

				delete[] header;
				header = nullptr;

				stats.m_total_msgs_send++;

				continue;
			}

			switch (type)
			{
				case protocol::Default::RequestType::compress:
				{
					std::cout << "compress" << std::endl;
					payload = new char[payload_length];
					boost::asio::read(*sock, boost::asio::buffer(payload, payload_length));

					_handle_compress_responce(sock, payload, payload_length, stats);
					break;
				}

				case protocol::Default::RequestType::ping:
				{
					std::cout << "ping" << std::endl;
					_handle_ping_responce(sock);
					break;
				}

				case protocol::Default::RequestType::get_stats:
				{
					std::cout << "get_stats" << std::endl;
					_handle_get_stats_responce(sock, stats);
					break;
				}

				case protocol::Default::RequestType::reset_stats:
				{
					std::cout << "reset_stats" << std::endl;
					_handle_reset_stats_responce(sock, stats);
					break;
				}

				default:
				{
					std::cout << "unsupported_request_type" << std::endl;
					_send_responce(sock, protocol::Default::StatusCode::unsupported_request_type);
				}
			}

			stats.m_total_msgs_send++;

			delete[] header;
			header = nullptr;

			delete[] payload;
			payload = nullptr;
		}
	} catch (std::exception const& e)
	{
		delete[] header;
		delete[] payload;

		std::cerr << e.what() << std::endl;
	}
}

void Server::_handle_compress_responce(socket_shared_ptr sock, char const* text_to_compress, std::size_t text_len, Stats& stats)
{
	stats.m_total_payload_bytes_received += text_len;
	stats.m_total_bytes_to_compress += text_len;

	try {
		std::string compressed_payload = compressors::default_compress(text_to_compress, text_len);
		const std::size_t compressed_payload_len = compressed_payload.size();

		std::cout << "compressed_payload: " << compressed_payload << std::endl;
		_send_responce(sock, protocol::Default::StatusCode::ok, compressed_payload.c_str(), compressed_payload_len);

		stats.m_total_bytes_after_compress += compressed_payload_len;
		stats.m_total_payload_bytes_send += compressed_payload_len;
	} catch (compressors::digits_in_text const& e)
	{
		std::cerr << "Exception: message_contains_digits." << std::endl;
		_send_responce(sock, protocol::Default::StatusCode::message_contains_digits);
	} catch (compressors::uppercase_in_text const& e)
	{
		std::cerr << "Exception: message_contains_uppercase." << std::endl;
		_send_responce(sock, protocol::Default::StatusCode::message_contains_uppercase);
	} catch (std::exception const& e)
	{
		std::cerr << "Exception: unknown. " << e.what() << std::endl;
		_send_responce(sock, protocol::Default::StatusCode::unknown_error);
	}
}

void Server::_handle_ping_responce(socket_shared_ptr sock)
{
	_send_responce(sock, protocol::Default::StatusCode::ok);
}

void Server::_handle_get_stats_responce(socket_shared_ptr sock, Stats const& stats)
{
	const std::size_t payload_len = 9; // uint_32t + uint32_t + uint8_t
	uint8_t *payload = new uint8_t[payload_len];

	uint8_t compression_ratio = 0;
	if (stats.m_total_bytes_after_compress)
	{
		compression_ratio = static_cast<uint8_t>(stats.m_total_bytes_to_compress / stats.m_total_bytes_after_compress);
	}
	else
	{
		compression_ratio = 0;
	}

	const std::uint32_t total_bytes_received = stats.m_total_payload_bytes_received + stats.m_total_msgs_received * protocol::Default::header_len;
	const std::uint32_t total_bytes_send = stats.m_total_payload_bytes_send + stats.m_total_msgs_send * protocol::Default::header_len;

	std::memcpy((void *)payload, (void const *)&total_bytes_received, 4);
	std::memcpy((void *)(payload + 4), (void const *)&total_bytes_send, 4);
	std::memcpy((void *)(payload + 8), (void const *)&compression_ratio, 1);

	_send_responce(sock, protocol::Default::StatusCode::ok, (char const *)payload, payload_len);

	delete[] payload;
}

void Server::_handle_reset_stats_responce(socket_shared_ptr sock, Stats& stats)
{
	stats.reset();

	_send_responce(sock, protocol::Default::StatusCode::ok);
}

void Server::_send_responce(socket_shared_ptr sock, protocol::Default::StatusCode code, char const* payload, std::size_t payload_len)
{
	const std::size_t msg_len = protocol::Default::header_len + payload_len;
	char *msg = new char[msg_len];

	protocol::Default::insert_header((void *)msg, code, payload_len);
	protocol::Default::insert_payload((void *)msg, (void const *)payload, payload_len);

	boost::asio::write(*sock, boost::asio::buffer(msg, msg_len));

	delete[] msg;
}
