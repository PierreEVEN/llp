#include <iostream>
#include "llp/file_data.hpp"
#include "llp/examples/json.hpp"

int main()
{
	std::filesystem::path json_path = "tests/examples/demo.json";

	Llp::JSonValue json;
	Llp::JSonParser::json_from_string(Llp::FileReaderHelper::from_path(json_path), json).exit_on_error(json_path);

	std::cout << json.to_string() << "\n";
	std::cout << json.to_string("", "") << "\n";
}
