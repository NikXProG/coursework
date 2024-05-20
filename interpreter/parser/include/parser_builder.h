#ifndef PARSER_BUILDER_H
#define PARSER_BUILDER_H
#include <string>
#include <stack>

class lexer{
        
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


};

class parser_builder{
public:
    
    //create base struct
    struct Token{
        size_t size = 0;
        std::string* token_array = nullptr;

    };

    struct str_code {
        std::string code;
        bool flag_def;
    };

    struct source_code {
        size_t size = 0;
        std::string type = "Code";
        str_code* source_array;
    };

    
    source_code Source;
    
    std::stack<int> stack; 

public:

    parser_builder();
    
    ~parser_builder() noexcept = default;

    parser_builder* lexer(std::string const &file_path);
    
    parser_builder* info();

    parser_builder* create_stack();


    
};


#endif