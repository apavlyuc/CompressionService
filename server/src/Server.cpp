#include <Server.hpp>

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
	std::size_t msg_compressed_count = 0;

	char *header = nullptr;
	char *payload = nullptr;
	try {
		while (true)
		{
			header = new char[protocol::Default::header_len];
			boost::asio::read(*sock, boost::asio::buffer(header, protocol::Default::header_len));

			const protocol::Default::RequestType type = protocol::Default::get_request_type(header);
			const std::size_t payload_length = protocol::Default::get_payload_length(header);

			switch (type)
			{
				case protocol::Default::RequestType::compress:
				{
					std::cout << "compress" << std::endl;
					payload = new char[payload_length];
					boost::asio::read(*sock, boost::asio::buffer(payload, payload_length));
					++msg_compressed_count;
					_handle_compress_responce(sock, payload, payload_length);
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
					_handle_get_stats_responce(sock, msg_compressed_count);
					break;
				}

				case protocol::Default::RequestType::reset_stats:
				{
					std::cout << "reset_stats" << std::endl;
					msg_compressed_count = 0;
					_handle_reset_stats_responce(sock);
					break;
				}

				default:
				{
					std::cout << "unsupported_request_type" << std::endl;
					_send_responce(sock, protocol::Default::StatusCode::unsupported_request_type);
				}
			}

			delete[] header;
			header = nullptr;

			delete[] payload;
			payload = nullptr;
		}
	} catch (std::exception const& e)
	{
		std::cerr << e.what() << std::endl;
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

void Server::_handle_get_stats_responce(socket_shared_ptr sock, std::size_t stats)
{
	std::string payload = std::to_string(stats);
	_send_responce(sock, protocol::Default::StatusCode::ok, payload.c_str(), payload.size());
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
