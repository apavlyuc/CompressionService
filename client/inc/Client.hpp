#pragma once

#include <Default.hpp>

#include <boost/asio.hpp>

#include <string>

class Client
{
public:
	Client() : _m_sock(_m_context) {}

	void	connect(boost::asio::ip::tcp::endpoint const& address);

	void	send_ping_request();
	void	send_get_stats_request();
	void	send_reset_stats_request();
	void	send_compress_request(std::string const& msg_to_compress);

	std::string	receive_msg();
private:
	void		_perform_request(protocol::Default::RequestType type, std::string const& payload = "");
	void		_send_msg(char const* data, std::size_t len);
	std::string	_get_server_responce();

	protocol::Default _m_protocol;

	boost::asio::io_context _m_context;
	boost::asio::ip::tcp::socket _m_sock;
};
