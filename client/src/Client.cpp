#include "Client.hpp"

#include <iostream>

/*
**	public methods
*/

void Client::connect(boost::asio::ip::tcp::endpoint const& address)
{
	_sock.connect(address);
}

void Client::send_compress_request(std::string const& msg_to_compress)
{
	_send_msg(msg_to_compress);
}

std::string Client::receive_msg()
{
	std::string msg;

	msg = _get_server_responce();

	return msg;
}

/*
**	private methods
*/

void	Client::_send_msg(std::string const& msg)
{
	_sock.write_some(boost::asio::buffer(msg, msg.size()));
}

std::string Client::_get_server_responce()
{
	std::string responce;

	try {
		// boost::system::error_code error;
		// boost::asio::streambuf receive_buffer;

		// boost::asio::read(_sock, receive_buffer, boost::asio::transfer_all(), error);
		// _sock.read_some(receive_buffer);

		char buff[1024]{};

		std::size_t len = _sock.read_some(boost::asio::buffer(buff));
		responce = buff;

		// if (false && error && error != boost::asio::error::eof)
		// {
		// 	std::cout << "Error. receive failed: " << error.message() << std::endl;
		// }
		// else
		// {
		// 	responce = boost::asio::buffer_cast<char const*>(receive_buffer.data());
		// }
	} catch (std::exception e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}
	return responce;
}
