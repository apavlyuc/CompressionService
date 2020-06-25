#pragma once

#include <Default.hpp>

#include <boost/asio.hpp>

struct Stats
{
	inline void reset()
	{
		m_total_msgs_received = 0;
		m_total_msgs_send = 0;

		m_total_payload_bytes_received = 0;
		m_total_payload_bytes_send = 0;

		m_total_bytes_to_compress = 0;
		m_total_bytes_after_compress = 0;
	}

	uint32_t m_total_msgs_received{0};
	uint32_t m_total_msgs_send{0};

	uint32_t m_total_payload_bytes_received{0};
	uint32_t m_total_payload_bytes_send{0};

	uint32_t m_total_bytes_to_compress{0};
	uint32_t m_total_bytes_after_compress{0};
};

class Server
{
	typedef std::shared_ptr<boost::asio::ip::tcp::socket> socket_shared_ptr;
public:

	void listen(boost::asio::ip::tcp::endpoint const& addr);
private:
	void _handle_compress_responce(socket_shared_ptr sock, char const* text_to_compress, std::size_t text_len);
	void _handle_ping_responce(socket_shared_ptr sock);
	void _handle_get_stats_responce(socket_shared_ptr sock, Stats const& stats);
	void _handle_reset_stats_responce(socket_shared_ptr sock);

	void _send_responce(socket_shared_ptr sock, protocol::Default::StatusCode code, char const* payload = 0, std::size_t payload_len = 0);

	void _client_session(socket_shared_ptr sock);

	boost::asio::io_service _m_io_context;
};
