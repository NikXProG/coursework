#ifndef LEXER_H
#define LEXER_H
#include <iostream>

class lexer{
    
public:

    enum class TokenType {
        number,
        assign,
        id,
        op,
        def,
        paren,
        skip
    };

public:

    struct CodeLine {
        size_t size = 0;
        std::string value;
    };

    struct ScriptCode {
        size_t size = 0;
        CodeLine* code = nullptr;
    };

    struct Token {
        lexer::TokenType type;
        std::string value;
        size_t index_token = 0;
    };

    struct Tokens{
        size_t size = 0;
        Token* token= nullptr;
    };

public:

    lexer() noexcept = default;

    ~lexer() noexcept = default;

public:
    static std::string token_to_string(TokenType token_type);
    static TokenType string_to_token(std::string const &token_string);

};
#endif