#pragma once

#include <Default.hpp>

#include <boost/asio.hpp>

class Server
{
	typedef std::shared_ptr<boost::asio::ip::tcp::socket> socket_shared_ptr;
public:

	void listen(boost::asio::ip::tcp::endpoint const& addr);
private:
	void _client_session(socket_shared_ptr sock);

	protocol::Default _m_protocol;

	boost::asio::io_service _m_io_context;
};
