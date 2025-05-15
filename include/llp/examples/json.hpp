#pragma once
#include <unordered_map>
#include "llp/tokenizer.hpp"
#include "llp/native_tokens.hpp"
#include "llp/parser.hpp"

namespace Llp
{
	class JSonValue
	{
	public:
		enum class Type
		{
			Number,
			String,
			Boolean,
			Array,
			Object,
			Null,
			Unknown
		};

		Type data_type = Type::Unknown;

		double value_number = 0.0f;
		std::string value_string;
		bool value_bool = false;
		std::vector<JSonValue> value_array;
		std::unordered_map<std::string, JSonValue> value_object;

		std::string to_string(const std::string& space = "  ", const std::string& new_line = "\n") const
		{
			return to_string(space, new_line, 0);
		}

	private:
		std::string to_string(const std::string& space, const std::string& new_line, size_t indents) const
		{
			switch (data_type)
			{
			case Type::Number:
				return std::to_string(value_number);
			case Type::String:
				return '"' + value_string + '"';
			case Type::Boolean:
				return value_bool ? "true" : "false";
			case Type::Array:
				{
					std::string array_str = "[" + new_line;
					for (size_t i = 0; i < value_array.size(); ++i)
						array_str += indent(indents, space) + value_array[i].to_string(space, new_line, indents + 1) + (
							i < value_array.size() - 1 ? "," : "") + new_line;
					array_str += indent(indents - 1, space) + "]";
					return array_str;
				}
			case Type::Object:
				{
					std::string object_str = "{" + new_line;
					std::vector<std::string> keys;
					for (const auto& key : value_object)
						keys.emplace_back(key.first);
					for (size_t i = 0; i < keys.size(); ++i)
					{
						object_str += indent(indents, space) + '"' + keys[i] + "\":" + space + value_object.
							find(keys[i])->second.to_string(space, new_line, indents + 2) + (
								i < keys.size() - 1 ? "," : "") + new_line;
					}
					object_str += indent(indents - 1, space) + "}";
					return object_str;
				}
			case Type::Null:
				return "null";
			default:
				return "[UNHANDLED JSON TYPE]";
			}
		}

		static std::string indent(size_t indents, const std::string& space)
		{
			std::string ind;
			for (size_t i = 0; i < indents + 1; ++i)
				ind += space;
			return ind;
		}
	};

	class JSonParser
	{
	public:
		static ParserError json_from_string(const std::string& in_string, JSonValue& out_json,
		                                    const std::filesystem::path& debug_path = "")
		{
			TokenSet token_set = TokenSet::preset_json_like();
			Tokenizer tokenizer;
			tokenizer.tokenize(in_string, token_set).exit_on_error(debug_path);
			Parser parser(tokenizer);
			return parse_json_value(parser, token_set, out_json);
		}

	private:
		static ParserError parse_array_value(const Tokenizer& block, const TokenSet& token_set,
		                                     std::vector<JSonValue>& out_values)
		{
			Parser parser(block);
			do
			{
				JSonValue value;
				if (ParserError error = parse_json_value(parser, token_set, value))
					return error;
				out_values.emplace_back(value);
			}
			while (parser.consume<SymbolToken>(','));

			if (parser)
				return ParserError{
					parser.current_location(),
					std::format("Unexpected token {} : '{}'", parser.get_current_token_name(token_set),
					            parser.get_current_token()->to_string(token_set))
				};

			return {};
		}

		static ParserError parse_object_value(const Tokenizer& tokenizer, const TokenSet& token_set,
		                                      std::unordered_map<std::string, JSonValue>& out_values)
		{
			Parser parser(tokenizer);
			do
			{
				auto key = parser.consume<StringLiteralToken>();
				if (!key)
					return ParserError{parser.current_location(), "Expected json key"};

				if (!parser.consume<SymbolToken>(':'))
					return ParserError{parser.current_location(), "Expected ':'"};

				JSonValue value;
				if (ParserError error = parse_json_value(parser, token_set, value))
					return error;
				if (!out_values.emplace(key->value, value).second)
					return ParserError{parser.current_location(), std::format("Duplicated object key {}", key->value)};
			}
			while (parser.consume<SymbolToken>(','));

			if (parser)
				return ParserError{
					parser.current_location(),
					std::format("Unexpected token {} : '{}'", parser.get_current_token_name(token_set),
					            parser.get_current_token()->to_string(token_set))
				};

			return {};
		}

		static ParserError parse_json_value(Parser& parser, const TokenSet& token_set, JSonValue& out_value)
		{
			if (auto value_array = parser.consume<SquareBlockToken>())
			{
				out_value.data_type = JSonValue::Type::Array;
				if (auto error = parse_array_value(value_array->content, token_set, out_value.value_array))
					return error;
			}
			else if (auto value_object = parser.consume<BraceBlockToken>())
			{
				out_value.data_type = JSonValue::Type::Object;
				if (auto error = parse_object_value(value_object->content, token_set, out_value.value_object))
					return error;
			}
			else if (parser.consume<WordToken>("null"))
			{
				out_value.data_type = JSonValue::Type::Null;
			}
			else if (parser.consume<WordToken>("true"))
			{
				out_value.data_type = JSonValue::Type::Boolean;
				out_value.value_bool = true;
			}
			else if (parser.consume<WordToken>("false"))
			{
				out_value.data_type = JSonValue::Type::Boolean;
				out_value.value_bool = false;
			}
			else if (auto flt_value = parser.consume<FloatingPointToken>())
			{
				out_value.data_type = JSonValue::Type::Number;
				out_value.value_number = flt_value->number;
			}
			else if (auto integer_value = parser.consume<IntegerToken>())
			{
				out_value.data_type = JSonValue::Type::Number;
				out_value.value_number = static_cast<double>(integer_value->number);
			}
			else if (auto string_value = parser.consume<StringLiteralToken>())
			{
				out_value.data_type = JSonValue::Type::String;
				out_value.value_string = string_value->value;
			}
			else
			{
				return ParserError{
					parser.current_location(),
					std::format("{} can't be parsed to a valid Json value : '{}'",
					            parser.get_current_token_name(token_set),
					            parser.get_current_token()->to_string(token_set))
				};
			}
			return {};
		}
	};
}
