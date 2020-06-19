#include <Server.hpp>

#include <iostream>

int		main(int ac, char **av)
{
	std::cout << "server running..." << std::endl;

	Server server;

	boost::asio::ip::tcp::endpoint addr(boost::asio::ip::tcp::v4(), 2001);
	server.listen(addr);

	return 0;
}