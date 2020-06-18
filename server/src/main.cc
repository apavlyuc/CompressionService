#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <iostream>

typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_shared_ptr;

void	client_session(socket_shared_ptr sock)
{
	try {
		while (true)
		{
			char data[512]{};
			size_t len = sock->read_some(boost::asio::buffer(data));
			if (len > 0)
			{
				boost::asio::write(*sock, boost::asio::buffer("ok", 5));
			}
			std::cout << "msg from client: " << data << std::endl;
		}
	} catch (std::exception const& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

int		main(int ac, char **av)
{
	std::cout << "server running..." << std::endl;

	boost::asio::io_service io_context;
	boost::asio::ip::tcp::endpoint addr(boost::asio::ip::tcp::v4(), 2001);
	boost::asio::ip::tcp::acceptor accept_manager(io_context, addr);

	while (true)
	{
		socket_shared_ptr sock(new boost::asio::ip::tcp::socket(io_context));
		accept_manager.accept(*sock);
		boost::thread trd(boost::bind(client_session, sock));
	}
	return 0;
}