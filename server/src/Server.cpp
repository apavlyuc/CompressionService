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
	try {
		std::string data;
		while (true)
		{
			data.resize(512);
			size_t len = sock->read_some(boost::asio::buffer(data));
			if (len > 0)
			{
				boost::asio::write(*sock, boost::asio::buffer("ok", 5));
			}
			data.resize(len);
			std::cout << "msg from client: " << data << std::endl;
		}
	} catch (std::exception const& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
