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
	stats.reset();

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

			stats.m_total_payload_bytes_received += payload_length;

			if (payload_length >= 1000)
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

					std::string compressed_payload;
					try {
						compressed_payload = compressors::default_compress(payload, payload_length);
					} catch (std::exception const& e)
					{
						_send_responce(sock, protocol::Default::StatusCode::unknown_error);
						break;
					}
					std::size_t new_payload_len = compressed_payload.size();

					_handle_compress_responce(sock, compressed_payload.c_str(), new_payload_len);

					stats.m_total_bytes_to_compress += payload_length;
					stats.m_total_bytes_after_compress += new_payload_len;

					stats.m_total_payload_bytes_send += new_payload_len;
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
					stats.reset();
					_handle_reset_stats_responce(sock);
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
		std::cerr << e.what() << std::endl;
		//_send_responce(sock, protocol::Default::StatusCode::unknown_error);
	}
}

void Server::_handle_compress_responce(socket_shared_ptr sock, char const* text_to_compress, std::size_t text_len)
{
	std::string compressed_text = "Compressed[" + std::string(text_to_compress, text_len) + "]";
	std::cout << "compressed_text: " << compressed_text << std::endl;
	_send_responce(sock, protocol::Default::StatusCode::ok, compressed_text.c_str(), compressed_text.size());
}

void Server::_handle_ping_responce(socket_shared_ptr sock)
{
	_send_responce(sock, protocol::Default::StatusCode::ok);
}

void Server::_handle_get_stats_responce(socket_shared_ptr sock, Stats const& stats)
{
	const std::size_t payload_len = 9; // uint_32t + uint32_t + uint8_t
	uint8_t *payload = new uint8_t[payload_len];
	std::memset(payload, 0, payload_len);

	uint8_t compression_ratio = 0;
	if (stats.m_total_bytes_after_compress)
	{
		compression_ratio = static_cast<uint8_t>(stats.m_total_bytes_to_compress / stats.m_total_bytes_after_compress);
	}
	else
	{
		compression_ratio = 0;
	}
	

	std::memcpy((void *)payload, (void const *)&stats.m_total_payload_bytes_received, 4);
	std::memcpy((void *)(payload + 4), (void const *)&stats.m_total_payload_bytes_send, 4);
	std::memcpy((void *)(payload + 8), (void const *)&compression_ratio, 1);

	_send_responce(sock, protocol::Default::StatusCode::ok, (char const *)payload, payload_len);

	delete[] payload;
}

void Server::_handle_reset_stats_responce(socket_shared_ptr sock)
{
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
