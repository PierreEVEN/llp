#pragma once

#include <memory>
#include <string>

#include "error.hpp"

/** Helper that declare a new token type.
 *
 * Example :
 *
 * DECLARE_LEXER_TOKEN(TokenTypeName) {
 *		static std::unique_ptr<TokenTypeName> consume(
 *			Tokenizer& tokenizer,
 *			Location& in_location,
 *			const std::string& source,
 *			ParserError& error)
 *		{
 *			...
 *		}
 *
 *		std::string to_string(const TokenSet& token_set, bool b_debug) const override
 *		{
 *			return "...";
 *		}
 * }
 *
 */

namespace Llp
{
	class Tokenizer;
	class TokenSet;
	class ILexerToken;

	// We generate a dummy function for each token type so we can use the underlying function ptr as a unique compile-time identifier
	using LexerTokenTypeId = void (*)();
	static constexpr LexerTokenTypeId NULL_TOKEN = nullptr;

	// Todo : improve generator to works with shared libraries
	namespace _InternalTokenGenerator
	{
		inline size_t _token_type_id_gen_function_dummy_value = 0;

		template <typename T>
		void _token_type_id_gen_fn()
		{
			// Required otherwise the compiler could factorize this function
			_token_type_id_gen_function_dummy_value += typeid(T).hash_code();
		}
	}

	// Get token information from static type
	template <typename T>
	struct TTokenType
	{
		static constexpr const char* name = "Null";
		static constexpr LexerTokenTypeId id = NULL_TOKEN;
	};

	/**
	 * Internal helper for token type construction
	 */
	class ITokenBuilder
	{
	public:
		virtual std::unique_ptr<ILexerToken> consume(const TokenSet& token_set, Location& in_location,
		                                             const std::string& source,
		                                             ParserError& error) = 0;
		virtual LexerTokenTypeId id() const = 0;
	};

	template <typename T>
	class TTokenBuilder : public ITokenBuilder
	{
		std::unique_ptr<ILexerToken> consume(const TokenSet& token_set, Location& in_location,
		                                     const std::string& source,
		                                     ParserError& error) override
		{
			return T::consume(token_set, in_location, source, error);
		}

	public:
		LexerTokenTypeId id() const override
		{
			return TTokenType<T>::id;
		}
	};

	/// Base token type
	class ILexerToken
	{
	public:
		ILexerToken(const Location& in_location) : location(in_location)
		{
		}

		virtual ~ILexerToken() = default;
		Location location;

		virtual LexerTokenTypeId get_type() const = 0;
		virtual std::string to_string(const TokenSet& token_set, bool b_debug = false) const = 0;

	protected:
		// Try to consume the input string
		static bool consume_string(Location& loc, const std::string& str, const std::string& test)
		{
			size_t i = 0;
			for (; i < test.size() && i + loc.get_index() < str.size(); ++i)
			{
				if (test[i] != str[loc.get_index() + i])
					return false;
			}
			if (i == test.size())
			{
				loc += i;
				return true;
			}
			return false;
		}

		// Try to consume any whitespace
		static bool consume_whitespace(Location& loc, const std::string& str)
		{
			while (loc.get_index() < str.size())
			{
				if (str[loc.get_index()] == '\n')
					loc.on_new_line();
				else if (std::isspace(str[loc.get_index()]))
					++loc;
				else
					break;
			}
			return loc.get_index() != str.size();
		}
	};
} // namespace Llp
