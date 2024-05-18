#ifndef PARSER_BUILDER_H
#define PARSER_BUILDER_H
#include <string>

class parser_builder{

private:

    //create base struct
    struct Token {
        size_t size = 0;
        std::string type = "Token";
        std::string* token_array = nullptr;
    };

    struct str_code {
        std::string code;
        bool flag_def;
    };

    struct source_code {
        size_t size = 0;
        std::string type = "Code";
        str_code* source_array = nullptr;
    };
public:
    source_code* Source = nullptr;

public:

    parser_builder();
    
    ~parser_builder() noexcept = default;

    parser_builder* create_buffer(std::string const &file_path);
    
    parser_builder* info();
    
};
#endif