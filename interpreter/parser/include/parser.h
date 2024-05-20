#ifndef PARSER_H
#define PARSER_H
#include <iostream>

class parser{

    enum class TokenType {
        number,
        assign,
        end,
        id,
        op,
        paren,
        skip,
        mismatch
    };

    struct Token {
        TokenType type;
        std::string value;
    };

    struct Tokens{
        size_t size = 0;
        Token* array;
    };

public:

    
    void reading_from_source();
    //virtual ~parser() noexcept = default;

    //virtual void run(std::string const &file_path) noexcept = 0;

    

};
#endif