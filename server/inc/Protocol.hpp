#pragma once

#include <string>

struct IProtocol
{
	virtual ~IProtocol() = default;

	virtual std::size_t	get_header_length() const = 0;
	virtual std::size_t get_payload_length(std::string const& header) const = 0;
};

struct DefaultProtocol : IProtocol
{
	virtual std::size_t get_header_length() const override { return _header_length; }
	virtual std::size_t get_payload_length(std::string const& header) const override;
private:
	static const std::size_t _header_length{8};
};
