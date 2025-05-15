#include "llp/token_set.hpp"

#include "llp/native_tokens.hpp"

namespace Llp
{
	TokenSet TokenSet::preset_c_like()
	{
		TokenSet set;
		set.register_token<CommentToken>("Comment");
		set.register_token<EndlToken>("Endl");
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
} // namespace Llp
