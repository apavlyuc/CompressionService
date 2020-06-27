#pragma once

#include <string>
#include <stdexcept>

namespace compressors {
	class digits_in_text : public std::invalid_argument
	{
	public:
		digits_in_text(char const* what_arg = "") : std::invalid_argument(what_arg) {}

		virtual char const* what() const noexcept {
			return "text to compress contains digits.";
		}
	};

	class uppercase_in_text : public std::invalid_argument
	{
	public:
		uppercase_in_text(char const* what_arg = "") : std::invalid_argument(what_arg) {}

		virtual char const* what() const noexcept {
			return "text to compress contains uppercase letters.";
		}
	};

	std::string	default_compress(char const* text, std::size_t text_len);
};
