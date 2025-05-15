#pragma once
#include "error.hpp"
#include <vector>

namespace Llp
{
	class ILexerToken;
	class TokenSet;

	// Represent a subset of the base file (the delimitation is defined by the owning object)
	class Tokenizer
	{
	public:
		[[nodiscard]] ParserError tokenize(const std::string& source, const TokenSet& token_set);

		void consume_next(const TokenSet& token_set, const std::string& source, Location& location, ParserError& error);

		[[nodiscard]] const std::vector<std::unique_ptr<ILexerToken>>& get_tokens() const
		{
			return tokens;
		}

		[[nodiscard]] std::string to_string(const TokenSet& token_set, bool b_debug = false) const;

	private:
		// Parsed tokens
		std::vector<std::unique_ptr<ILexerToken>> tokens;
	};
}
