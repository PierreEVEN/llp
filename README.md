# llp

![status_linux](https://github.com/PierreEVEN/llp/actions/workflows/linux.yml/badge.svg) ![status_windows](https://github.com/PierreEVEN/llp/actions/workflows/windows.yml/badge.svg)

**Little Language Processor** is a tiny helper library for parsing any data structure.

## Quick examples

> A full demo is available in [include/llp/examples/json.hpp](include/llp/examples/json.hpp)

```cpp
// Read source file from path
std::string cpp_data = Llp::FileReaderHelper::from_path("path/to/file.cpp");

// Tokenize the input file using an integrated token set
Llp::TokenSet token_set = Llp::TokenSet::preset_c_like();
Llp::Tokenizer tokenizer;
tokenizer.tokenize(cpp_data, token_set).exit_on_error(debug_path);
for (Llp::Parser parser(tokenizer); parser; ++parser)
    // process each tokens
```

## Concepts

### Token Set

A `Llp::TokenSet` contains all the token available to the tokenizer.

Custom tokens can be registered using the macro `DECLARE_LEXER_TOKEN(TokenName)` (more examples in [include/llp/native_tokens.hpp](include/llp/native_tokens.hpp)).
>⚠️ Note : custom tokens must be declared wiinthin the `Llp` namespace !

A custom `Llp::TokenSet` can be built as follows :
```cpp
Llp::TokenSet set;
set.register_token<WordToken>("Word");
set.register_token<SymbolToken>("Symbol");
```
Tokens are registered by order of priority (higher priority first). This mean that the tokenizer will produce a token of the first valid type it finds.

### Parser

The class `Llp::Parser` helps reading parsed tokens from the tokenizer.

The most important method is `<typename TokenType> TokenType* consume();`. It returns the found token if the type is corresponding, then advance to the next token.
An optional argument can be provided to this method to ask for a specific input


Example :
```cpp
Llp::Parser parser(tokenizer);

while (parser) { // True while there is remaining tokens to process
    if (Llp::WordToken* word = parser.consume<WordToken>()) {
        // We found a word token
    } else if (Llp::StringLiteralToken* string_literal = parser.consume<StringLiteralToken>()) {
        // We found a string literal token
    } else if (parser.consume<Symbol>(";")) { // return nullptr if the current symbol is not equals to ';'
        // We found a ';' symbol
    } else if (Llp::SymbolToken* symbol = parser.consume<Symbol>(";")) {
        // We found a symbol (any value)
    } else {
        // unhandled token type : error or continue
        ++parser; // Very important : don't forget to move to the next token if it wasn't consumed by any of the previous cases
    }
}
```