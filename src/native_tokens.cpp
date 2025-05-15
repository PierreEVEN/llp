#include "llp/native_tokens.hpp"

#include "llp/tokenizer.hpp"

namespace Llp
{
std::unique_ptr<CommentToken> CommentToken::consume(const TokenSet&, Location& in_location, const std::string& source, ParserError& error)
{
    if (in_location.get_index() + 1 < source.size())
    {
        // comment
        if (source[in_location.get_index()] == '/' && source[in_location.get_index() + 1] == '/')
        {
            ++++in_location;
            Location start = in_location;

            for (; in_location.get_index() < source.length(); ++in_location)
                if (source[in_location.get_index()] == '\n')
                    break;
            std::string value   = source.substr(start.get_index(), in_location.get_index() - start.get_index());
            auto        comment = std::make_unique<CommentToken>(start);
            comment->value      = value;
            comment->multiline  = false;
            return comment;
        }
        /*comment*/
        if (source[in_location.get_index()] == '/' && source[in_location.get_index() + 1] == '*')
        {
            ++++in_location;
            Location start = in_location;
            for (size_t i = start.get_index(); i < source.length() - 1; ++i)
            {
                if (source[i] == '*' && source[i + 1] == '/')
                {
                    std::string value = source.substr(start.get_index(), in_location.get_index() - start.get_index());
                    ++++in_location;
                    auto comment       = std::make_unique<CommentToken>(start);
                    comment->value     = value;
                    comment->multiline = true;
                    return comment;
                }
                if (source[i] == '\n')
                    in_location.on_new_line();
                else
                    ++in_location;
            }
            error = {start, "String comment doesn't end. '*/' expected"};
        }
    }
    return nullptr;
}

std::unique_ptr<EndlToken> EndlToken::consume(const TokenSet&, Location& in_location, const std::string& source, ParserError&)
{
    return source[in_location.get_index()] == '\n' ? std::make_unique<EndlToken>(in_location.on_new_line()) : nullptr;
}

std::unique_ptr<IncludeToken> IncludeToken::consume(const TokenSet&, Location& in_location, const std::string& source, ParserError& error)
{
    if (consume_string(in_location, source, "#include"))
    {
        if (consume_whitespace(in_location, source))
        {
            if (source[in_location.get_index()] == '"' || source[in_location.get_index()] == '<')
            {
                Location start = ++in_location;

                for (; in_location.get_index() < source.size(); ++in_location)
                {
                    if (source[in_location.get_index()] == '"' || source[in_location.get_index()] == '>')
                    {
                        auto include  = std::make_unique<IncludeToken>(start);
                        include->path = source.substr(start.get_index(), in_location.get_index() - start.get_index());
                        ++in_location;
                        return include;
                    }
                }
                error = {start, "No include directive end, expected '>' or '\"'"};
            }
            else
                error = {in_location, "Invalid token, expected '\"' or '<'"};
        }
        else
            error = {in_location, "Invalid #include directive, encountered endl"};
    }
    return nullptr;
}

std::unique_ptr<WhitespaceToken> WhitespaceToken::consume(const TokenSet&, Location& in_location, const std::string& source, ParserError&)
{
    if (std::isspace(static_cast<unsigned char>(source[in_location.get_index()])))
    {
        Location start = in_location;
        consume_whitespace(in_location, source);
        auto whitespace        = std::make_unique<WhitespaceToken>(start);
        whitespace->whitespace = source.substr(start.get_index(), in_location.get_index() - start.get_index());
        return whitespace;
    }
    return nullptr;
}

std::unique_ptr<WordToken> WordToken::consume(const TokenSet&, Location& in_location, const std::string& source, ParserError&)
{
    unsigned char start_chr = source[in_location.get_index()];
    if (std::isalpha(start_chr) || start_chr == '_')
    {
        auto start = in_location++;
        while (in_location.get_index() < source.size())
        {
            auto chr = source[in_location.get_index()];
            if (!std::isalnum(chr) && chr != '_')
                break;
            ++in_location;
        }
        auto word  = std::make_unique<WordToken>(start);
        word->word = source.substr(start.get_index(), in_location.get_index() - start.get_index());
        return word;
    }
    return nullptr;
}

std::unique_ptr<BraceBlockToken> BraceBlockToken::consume(const TokenSet& token_set, Location& in_location, const std::string& source, ParserError& error)
{
    if (source[in_location.get_index()] == '{')
    {
        auto block = std::make_unique<BraceBlockToken>(++in_location);
        while (in_location.get_index() < source.size())
        {
            if (source[in_location.get_index()] == '}')
            {
                block->end = in_location++;
                return block;
            }
            block->content.consume_next(token_set, source, in_location, error);
            if (error)
                return nullptr;
        }
        error = {in_location, "Missing block end. '}' expected"};
    }
    return nullptr;
}

std::unique_ptr<SquareBlockToken> SquareBlockToken::consume(const TokenSet& token_set, Location& in_location,
	const std::string& source, ParserError& error)
{
    if (source[in_location.get_index()] == '[')
    {
        auto block = std::make_unique<SquareBlockToken>(++in_location);
        while (in_location.get_index() < source.size())
        {
            if (source[in_location.get_index()] == ']')
            {
                block->end = in_location++;
                return block;
            }
            block->content.consume_next(token_set, source, in_location, error);
            if (error)
                return nullptr;
        }
        error = { in_location, "Missing arguments end. ')' expected" };
    }
    return nullptr;
}

std::unique_ptr<ParenthesisBlockToken> ParenthesisBlockToken::consume(const TokenSet& token_set, Location& in_location, const std::string& source, ParserError& error)
{
    if (source[in_location.get_index()] == '(')
    {
        auto block = std::make_unique<ParenthesisBlockToken>(++in_location);
        while (in_location.get_index() < source.size())
        {
            if (source[in_location.get_index()] == ')')
            {
                block->end = in_location++;
                return block;
            }
            block->content.consume_next(token_set, source, in_location, error);
            if (error)
                return nullptr;
        }
        error = { in_location, "Missing arguments end. ')' expected" };
    }
    return nullptr;
}

std::unique_ptr<StringLiteralToken> StringLiteralToken::consume(const TokenSet&, Location& in_location, const std::string& source, ParserError& error)
{
    if (source[in_location.get_index()] == '"')
    {
        Location start = ++in_location;
        while (in_location.get_index() < source.length() - 1)
        {
            if (source[in_location.get_index()] == '\\' && source[in_location.get_index() + 1] == '\\')
                ++++in_location;
            if (source[in_location.get_index()] == '"' && source[in_location.get_index() - 1] != '\\')
            {
                std::string value  = source.substr(start.get_index(), in_location.get_index() - start.get_index());
                auto        string = std::make_unique<StringLiteralToken>(start);
                string->value      = value;
                ++in_location;
                return string;
            }
            ++in_location;
        }
        error = {start, "Missing string literal end. '\"' expected"};
    }
    return nullptr;
}


std::unique_ptr<IntegerToken> IntegerToken::consume(const TokenSet&, Location& in_location, const std::string& source, ParserError& error)
{
    unsigned char start_chr = source[in_location.get_index()];
    if (std::isdigit(start_chr))
    {
        auto start = in_location;
        auto end   = start;
        while (end.get_index() < source.size())
        {
            auto chr = source[end.get_index()];
            if (!std::isdigit(chr))
                break;
            ++end;
        }
        auto        word    = std::make_unique<IntegerToken>(start);
        std::string num_str = source.substr(start.get_index(), end.get_index() - start.get_index());
        char*       end_ptr;
        word->number = strtoll(&*num_str.begin(), &end_ptr, 10);
        if (end_ptr != &*num_str.begin() + num_str.size())
        {
            error = {start, "Failed to parse integer number '" + num_str + "'"};
            return nullptr;
        }
        for (size_t i = 0; i < num_str.size(); ++i)
            ++in_location;
        return word;
    }
    return nullptr;
}

std::unique_ptr<FloatingPointToken> FloatingPointToken::consume(const TokenSet&, Location& in_location, const std::string& source, ParserError& error)
{
    unsigned char start_chr = source[in_location.get_index()];
    if (std::isdigit(start_chr) || (in_location.get_index() < source.size() - 1 && start_chr == '.' && std::isdigit(static_cast<unsigned char>(source[in_location.get_index() + 1]))))
    {
        bool     b_first_decimal = true;
        Location start           = in_location;
        Location end             = start;
        while (end.get_index() < source.size())
        {
            auto chr = source[end.get_index()];
            if (chr == '.')
            {
                if (b_first_decimal)
                    b_first_decimal = false;
                else
                {
                    error = {start, "Invalid floating point number : unexpected '.'"};
                    return nullptr;
                }
                ++end;
                continue;
            }

            if (!std::isdigit(chr) && chr != '.')
                break;
            ++end;
        }
        if (b_first_decimal)
            return nullptr;
        auto        word    = std::make_unique<FloatingPointToken>(start);
        std::string num_str = "0" + source.substr(start.get_index(), end.get_index() - start.get_index());
        char*       end_ptr;
        word->number = strtod(&*num_str.begin(), &end_ptr);
        if (end_ptr != &*num_str.begin() + num_str.size())
        {
            error = {start, "Failed to parse floating point number '" + num_str + "'"};
            return nullptr;
        }
        for (size_t i = 0; i < num_str.size() - 1; ++i)
            ++in_location;;
        return word;
    }
    return nullptr;
}

std::unique_ptr<SymbolToken> SymbolToken::consume(const TokenSet&, Location& in_location, const std::string& source, ParserError&)
{
    auto symbol    = std::make_unique<SymbolToken>(in_location);
    symbol->symbol = source[in_location.get_index()];
    ++in_location;
    return symbol;
}
} // namespace Llp