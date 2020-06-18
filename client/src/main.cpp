#include "Client.hpp"

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <iostream>

int		main(int ac, char **av)
{
	std::cout << "client" << std::endl;

	Client client;
	client.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 2001));

	std::string line;
	std::string answer_from_server;
	while (true)
	{
		std::getline(std::cin, line);

		client.send_compress_request(line);
		answer_from_server = client.receive_msg();

		std::cout << answer_from_server << std::endl;
	}

	return 0;
}
