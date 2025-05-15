#include "llp/token.hpp"

#include "llp/tokenizer.hpp"

#include <format>

namespace Llp
{
	ParserError Tokenizer::tokenize(const std::string& source, const TokenSet& token_set)
	{
		tokens.clear();
		Location location;
		ParserError error;
		while (location.index < source.size())
		{
			consume_next(token_set, source, location, error);
			if (error)
				break;
		}
		return error;
	}
} // namespace Llp
