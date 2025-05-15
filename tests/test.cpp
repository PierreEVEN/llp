#include <iostream>
#include "llp/file_data.hpp"
#include "llp/examples/json.hpp"

int main()
{
	{
		std::filesystem::path json_path = "tests/examples/demo.json";

		Llp::JSonValue json;
		Llp::JSonParser::json_from_string(Llp::FileReaderHelper::from_path(json_path), json).exit_on_error(json_path);

		std::cout << json.to_string() << "\n";
		std::cout << json.to_string("", "") << "\n";
	}

	{
		std::filesystem::path hpp_path = "tests/examples/demo.hpp";

		Llp::TokenSet token_set = Llp::TokenSet::preset_c_like();
		Llp::Tokenizer tokenizer;
		tokenizer.tokenize(Llp::FileReaderHelper::from_path(hpp_path), token_set).exit_on_error(hpp_path);

		Llp::Parser parser(tokenizer);

		std::cout << tokenizer.to_string(token_set) << "\n";
		std::cout << tokenizer.to_string(token_set, true) << "\n";
	}
}
