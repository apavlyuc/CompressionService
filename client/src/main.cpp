#include "Client.hpp"

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <iostream>
#include <fstream>
#include <map>

namespace {
	static const std::map<std::string, uint8_t> g_modes{
			{"ping", 1},
			{"get_stats", 2},
			{"reset_stats", 3}
	};

	void check_usage(int ac, char **av)
	{
		if (ac == 1)
		{
			return;
		}

		if (ac == 2 && g_modes.count(av[1]))
		{
			return;
		}
		if (ac == 3 && std::string(av[1]) == "-f")
		{
			return;
		}
		std::cout << "wrong usage" << std::endl;
		exit(0);
	}

	void control_request_mode(Client& client, uint8_t id)
	{
		std::string answer_from_server;

		switch (id)
		{
			case 1:
			{
				client.send_ping_request();
				answer_from_server = client.receive_msg();
				break;
			}
			case 2:
			{
				client.send_get_stats_request();
				answer_from_server = client.receive_msg();
				break;
			}
			case 3:
			{
				client.send_reset_stats_request();
				answer_from_server = client.receive_msg();
				break;
			}
			default:
			{
				std::cout << "unsupported id type in ::control_request_mode" << std::endl;
				return;
			}
		}

		std::cout << "answer_from_server: " << answer_from_server << std::endl;
	}

	void compress_mode(Client& client, std::istream& is)
	{
		std::string line;
		std::string answer_from_server;
		try {
			while (std::getline(is, line))
			{
				if (line.empty())
				{
					continue;
				}

				client.send_compress_request(line);
				answer_from_server = client.receive_msg();

				std::cout << "server answer: [" << answer_from_server << "]" << std::endl;
			}
		} catch (std::exception const& e)
		{
			std::cerr << "Exception. Connection interrapted.\twhat(): " << e.what() << std::endl;
		}
	}
};

int		main(int ac, char **av)
{
	check_usage(ac, av);

	Client client;
	try {
		client.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 2001));
	} catch (std::exception const& e)
	{
		std::cerr << "Exception. Can't connect to server.\twhat(): " << e.what() << std::endl;
		return 0;
	}

	if (ac == 1)
	{
		compress_mode(client, std::cin);
	}
	else if (ac == 2)
	{
		control_request_mode(client, g_modes.at(av[1]));
	}
	else
	{
		std::ifstream ifs(av[2]);
		compress_mode(client, ifs);
	}

	return 0;
}
