#pragma once
#include "tokenizer.hpp"
#include "token.hpp"

namespace Llp
{
	/*####[ // / * ]####*/
	DECLARE_LEXER_TOKEN(CommentToken)
		static std::unique_ptr<CommentToken> consume(const TokenSet& token_set, Location& in_location,
		                                             const std::string& source, ParserError& error);

		std::string to_string(const TokenSet&, bool) const override
		{
			return multiline ? "/*" + value + "*/" : "//" + value;
		}

		bool multiline = false;
		std::string value;
	};

	/*####[ ; ]####*/
	DECLARE_LEXER_TOKEN(SemicolonToken)
		static std::unique_ptr<SemicolonToken> consume(const TokenSet& token_set, Location& in_location,
		                                               const std::string& source, ParserError& error);

		std::string to_string(const TokenSet&, bool) const override
		{
			return ";";
		}
	};

	/*####[ = ]####*/
	DECLARE_LEXER_TOKEN(EqualsToken)
		static std::unique_ptr<EqualsToken> consume(const TokenSet& token_set, Location& in_location,
		                                            const std::string& source, ParserError& error);

		std::string to_string(const TokenSet&, bool) const override
		{
			return "=";
		}
	};

	/*####[ , ]####*/
	DECLARE_LEXER_TOKEN(ComaToken)
		static std::unique_ptr<ComaToken> consume(const TokenSet& token_set, Location& in_location,
		                                          const std::string& source, ParserError& error);

		std::string to_string(const TokenSet&, bool) const override
		{
			return ",";
		}
	};

	/*####[ \n ]####*/
	DECLARE_LEXER_TOKEN(EndlToken)
		static std::unique_ptr<EndlToken> consume(const TokenSet& token_set, Location& in_location,
		                                          const std::string& source, ParserError& error);

		std::string to_string(const TokenSet&, bool b_debug) const override
		{
			return "\n";
		}
	};

	/*####[ #include "value" ]####*/
	DECLARE_LEXER_TOKEN(IncludeToken)
		static std::unique_ptr<IncludeToken> consume(const TokenSet& token_set, Location& in_location,
		                                             const std::string& source, ParserError& error);

		std::string to_string(const TokenSet&, bool) const override
		{
			return "#include \"" + path + "\"";
		}

		bool operator==(const std::string& a) const
		{
			return path == a;
		}

		std::string path;
	};

	/*####[ Whitespace ]####*/
	DECLARE_LEXER_TOKEN(WhitespaceToken)
		static std::unique_ptr<WhitespaceToken> consume(const TokenSet& token_set, Location& in_location,
		                                                const std::string& source, ParserError& error);

		std::string to_string(const TokenSet&, bool) const override
		{
			return whitespace;
		}

		std::string whitespace;
	};

	/*####[ Word ]####*/
	DECLARE_LEXER_TOKEN(WordToken)
		static std::unique_ptr<WordToken> consume(const TokenSet& token_set, Location& in_location,
		                                          const std::string& source, ParserError& error);

		std::string to_string(const TokenSet&, bool) const override
		{
			return word;
		}

		bool operator==(const std::string& a) const
		{
			return word == a;
		}

		std::string word;
	};

	/*####[ {} ]####*/
	DECLARE_LEXER_TOKEN(BraceBlockToken)
		static std::unique_ptr<BraceBlockToken> consume(const TokenSet& token_set, Location& in_location,
		                                                const std::string& source, ParserError& error);

		std::string to_string(const TokenSet& token_set, bool b_debug) const override
		{
			return "{" + content.to_string(token_set, b_debug) + "}";
		}

		Location end;
		Tokenizer content;
	};

	/*####[ [] ]####*/
	DECLARE_LEXER_TOKEN(SquareBlockToken)
		static std::unique_ptr<SquareBlockToken> consume(const TokenSet& token_set, Location& in_location,
		                                                 const std::string& source, ParserError& error);

		std::string to_string(const TokenSet& token_set, bool b_debug) const override
		{
			return "[" + content.to_string(token_set, b_debug) + "]";
		}

		Location end;
		Tokenizer content;
	};

	/*####[ () ]####*/
	DECLARE_LEXER_TOKEN(ParenthesisBlockToken)
		static std::unique_ptr<ParenthesisBlockToken> consume(const TokenSet& token_set, Location& in_location,
		                                               const std::string& source, ParserError& error);

		std::string to_string(const TokenSet& token_set, bool b_debug) const override
		{
			return "(" + content.to_string(token_set, b_debug) + ")";
		}

		Location end;
		Tokenizer content;
	};

	/*####[ "string" ]####*/
	DECLARE_LEXER_TOKEN(StringLiteralToken)
		static std::unique_ptr<StringLiteralToken> consume(const TokenSet& token_set, Location& in_location,
		                                                   const std::string& source,
		                                                   ParserError& error);

		std::string to_string(const TokenSet&, bool) const override
		{
			return '"' + value + '"';
		}

		bool operator==(const std::string& a) const
		{
			return value == a;
		}

		std::string value;
	};

	/*####[ 000 ]####*/
	DECLARE_LEXER_TOKEN(IntegerToken)
		static std::unique_ptr<IntegerToken> consume(const TokenSet& token_set, Location& in_location,
		                                             const std::string& source, ParserError& error);

		std::string to_string(const TokenSet&, bool) const override
		{
			return std::to_string(number);
		}

		bool operator==(int64_t a) const
		{
			return number == a;
		}

		int64_t number;
	};

	/*####[ 00.00 ]####*/
	DECLARE_LEXER_TOKEN(FloatingPointToken)
		static std::unique_ptr<FloatingPointToken> consume(const TokenSet& token_set, Location& in_location,
		                                                   const std::string& source,
		                                                   ParserError& error);

		std::string to_string(const TokenSet&, bool) const override
		{
			return std::to_string(number);
		}

		double number;
	};

	/*####[ ! ]####*/
	DECLARE_LEXER_TOKEN(SymbolToken)
		static std::unique_ptr<SymbolToken> consume(const TokenSet& token_set, Location& in_location,
		                                            const std::string& source, ParserError& error);

		std::string to_string(const TokenSet&, bool) const override
		{
			return std::string() + symbol;
		}

		bool operator==(char a) const
		{
			return symbol == a;
		}

		char symbol;
	};
}
