#include <Compressors.hpp>

#include <stdexcept>

namespace compressors {

	namespace {
		void throw_if_invalid_text(char const* text, std::size_t text_len)
		{
			auto is_digit = [](char c) {
				return (c >= '0' && c <= '9');
			};

			auto is_uppercase = [](char c) {
				return (c >= 65 && c <= 90);
			};

			for (std::size_t i = 0; i < text_len; ++i)
			{
				if (is_digit(text[i]))
				{
					throw std::runtime_error("text contain's digits");
				}

				if (is_uppercase(text[i]))
				{
					throw std::runtime_error("text contain's uppercase");
				}
			}
		}
	}

	std::string	default_compress(char const* text, std::size_t text_len)
	{
		if (text == nullptr || text_len == 0)
		{
			return "";
		}

		throw_if_invalid_text(text, text_len);

		std::string ret;
		ret.reserve(text_len);

		std::size_t count_of_repeatings{1};
		char current_symbol = text[0];
		for (std::size_t i = 1; i < text_len; ++i)
		{
			if (text[i] == current_symbol)
			{
				++count_of_repeatings;
				continue;
			}

			if (count_of_repeatings == 1)
			{
				ret += current_symbol;
			}
			else if (count_of_repeatings == 2)
			{
				ret += current_symbol;
				ret += current_symbol;
			}
			else
			{
				ret += std::to_string(count_of_repeatings);
				ret += current_symbol;
			}
			current_symbol = text[i];
			count_of_repeatings = 1;
		}

		if (count_of_repeatings == 1)
		{
			ret += current_symbol;
		}
		else if (count_of_repeatings == 2)
		{
			ret += current_symbol;
			ret += current_symbol;
		}
		else
		{
			ret += std::to_string(count_of_repeatings);
			ret += current_symbol;
		}

		return std::move(ret);
	}
};
