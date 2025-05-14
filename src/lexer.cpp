#include "llp/lexer.hpp"

#include "llp/native_tokens.hpp"
#include "llp/tokens.hpp"

namespace Llp
{
	TokenSet TokenSet::preset_c_like()
	{
		TokenSet set;
		set.register_token<CommentToken>("Comment");
		set.register_token<SemicolonToken>("Semicolon");
		set.register_token<EndlToken>("Endl");
		set.register_token<EqualsToken>("Equals");
		set.register_token<FloatingPointToken>("FloatingPoint");
		set.register_token<IntegerToken>("Integer");
		set.register_token<IncludeToken>("Include");
		set.register_token<StringLiteralToken>("StringLiteral");
		set.register_token<WhitespaceToken>("Whitespace");
		set.register_token<WordToken>("Word");
		set.register_token<ParenthesisBlockToken>("Arguments");
		set.register_token<BraceBlockToken>("Block");
		set.register_token<SymbolToken>("Symbol");
		return set;
	}

	TokenSet TokenSet::preset_json_like()
	{
		TokenSet set;
		set.register_token<FloatingPointToken>("FloatingPoint");
		set.register_token<IntegerToken>("Integer");
		set.register_token<StringLiteralToken>("StringLiteral");
		set.register_token<WhitespaceToken>("Whitespace");
		set.register_token<BraceBlockToken>("BraceBlock");
		set.register_token<SquareBlockToken>("SquareBlock");
		set.register_token<WordToken>("Word");
		set.register_token<SymbolToken>("Symbol");
		return set;
	}

	ParserError Lexer::run(const std::string& source, const TokenSet& token_set)
	{
		Location location;
		ParserError error;
		while (location.index < source.size())
		{
			root_block.consume_next(token_set, source, location, error);
			if (error)
				break;
		}
		return error;
	}
} // namespace Llp
