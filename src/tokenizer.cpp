#include "llp/tokenizer.hpp"
#include "llp/token.hpp"
#include "llp/token_set.hpp"

namespace Llp
{
	void Tokenizer::consume_next(const TokenSet& token_set, const std::string& source, Location& location, ParserError& error)
	{
		if (auto token = token_set.parse(source, location, error))
			tokens.emplace_back(std::move(token));
	}

	std::string Tokenizer::to_string(const TokenSet& token_set, bool b_debug) const
	{
		std::string str;
		for (const auto& token : tokens)
			if (b_debug)
				str += std::format("{}[{}], ", token_set.get_token_name(token->get_type()), token->to_string(token_set, b_debug));
			else
				str += token->to_string(token_set, b_debug);
		return str;
	}
}
