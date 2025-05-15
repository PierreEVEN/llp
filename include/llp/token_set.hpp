#pragma once
#include <unordered_map>
#include <vector>

#include "token.hpp"

namespace Llp
{
	class TokenSet
	{
	public:
		[[nodiscard]] const char* get_token_name(LexerTokenTypeId id) const
		{
			if (id == NULL_TOKEN)
				return "Null";
			if (auto it = token_names.find(id); it != token_names.end())
				return it->second;
			return "Undefined_Token_Id";
		}


		template <typename T>
		void register_token(const char* name)
		{
			token_types.emplace_back(std::make_unique<TTokenBuilder<T>>());
			token_names.emplace(TTokenType<T>::id, name);
		}

		template <typename T, typename Other>
		void register_token_before(const char* name)
		{
			for (auto it = token_types.begin(); it != token_types.end(); ++it)
				if ((*it)->id() == TTokenType<Other>::id)
				{
					token_types.insert(it, std::make_unique<TTokenBuilder<T>>());
					token_names.emplace(TTokenType<T>::id, name);
					break;
				}
		}

		template <typename T>
		void remove_token()
		{
			for (auto it = token_types.begin(); it != token_types.end(); ++it)
				if ((*it)->id() == TTokenType<T>::id)
				{
					token_names.erase(TTokenType<T>::id);
					token_types.erase(it);
					break;
				}
		}

		[[nodiscard]] std::unique_ptr<ILexerToken> parse(const std::string& source, Location& location, ParserError& error) const
		{
			if (error)
				return nullptr;
			for (const auto& type : token_types)
			{
				if (auto token = type->consume(*this, location, source, error))
				{
					if (error)
						return nullptr;
					return token;
				}
			}
			if (!error)
				error = {
					location,
					std::format("Parsing failed {}:{} : unidentified token '{}'", location.get_line(), location.get_column(),
					            source[location.get_index()])
				};
			return nullptr;
		}

		[[nodiscard]] static TokenSet preset_c_like();
		[[nodiscard]] static TokenSet preset_json_like();

	private:
		std::vector<std::unique_ptr<ITokenBuilder>> token_types;
		std::unordered_map<LexerTokenTypeId, const char*> token_names;
	};
} // namespace Llp
