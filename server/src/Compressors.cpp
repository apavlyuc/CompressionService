#include <Compressors.hpp>

#include <algorithm>
#include <iterator>
#include <stdexcept>

namespace compressors {

	namespace {
		void throw_if_invalid_text(char const* text, std::size_t text_len)
		{
			if (!text || !text_len)
			{
				throw std::runtime_error("compressor get empty text(nullptr or 0 as a len).");
			}

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
					throw digits_in_text();
				}

				if (is_uppercase(text[i]))
				{
					throw uppercase_in_text();
				}
			}
		}
	}

	std::string	default_compress(char const* text, std::size_t text_len)
	{
		throw_if_invalid_text(text, text_len);

		std::string compressed_text;
		compressed_text.reserve(text_len);

		auto append_block = [](std::string& dst, char symbol, std::size_t symbol_repeating){
			if (symbol_repeating < 3)
			{
				std::back_insert_iterator<std::string> back_insert_it(dst);
				std::fill_n(back_insert_it, symbol_repeating, symbol);
			}
			else
			{
				dst += std::to_string(symbol_repeating);
				dst += symbol;
			}
		};

		char current_symbol = text[0];
		std::size_t count_of_repeatings = 1;
		for (std::size_t i = 1; i < text_len; ++i)
		{
			if (text[i] == current_symbol)
			{
				++count_of_repeatings;
				continue;
			}

			append_block(compressed_text, current_symbol, count_of_repeatings);

			current_symbol = text[i];
			count_of_repeatings = 1;
		}

		append_block(compressed_text, current_symbol, count_of_repeatings);

		return std::move(compressed_text);
	}
};
