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
	const std::size_t header_len = _m_protocol.get_header_length();

	try {
		while (true)
		{
			char *header = new char[header_len];
			boost::asio::read(*sock, boost::asio::buffer(header, header_len));

			const std::size_t payload_length = _m_protocol.get_payload_length(header);
			char *payload = new char[payload_length];
			boost::asio::read(*sock, boost::asio::buffer(payload, payload_length));

			std::cout << "payload: [" << std::string(payload, payload_length) << "]" << std::endl;

			boost::asio::write(*sock, boost::asio::buffer(payload, payload_length));

			delete[] header;
			delete[] payload;
		}
	} catch (std::exception const& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
