#include <iostream>

#include "llp/lexer.hpp"
#include "llp/file_data.hpp"
#include "llp/parser.hpp"

int main() {
    Llp::FileReader reader("tests/examples/demo.json");
    reader.read();

    Llp::Lexer lexer;
    lexer.run(reader.raw_stream());

    std::cout << lexer.get_root().to_string(true) << "\n";

    Llp::Parser parser(lexer.get_root());
}
