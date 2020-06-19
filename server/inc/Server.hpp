#pragma once

#include <Protocol.hpp>

#include <boost/asio.hpp>

#include <memory>

class Server
{
	typedef std::shared_ptr<boost::asio::ip::tcp::socket> socket_shared_ptr;
public:
	Server(std::shared_ptr<IProtocol> protocol = std::make_shared<DefaultProtocol>()) : _m_protocol(protocol) {}

	void listen(boost::asio::ip::tcp::endpoint const& addr);
private:
	void _client_session(socket_shared_ptr sock);

	std::shared_ptr<IProtocol> _m_protocol;

	boost::asio::io_service _m_io_context;
};
