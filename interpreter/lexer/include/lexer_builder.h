

















/*#ifndef LEXER_BUILDER_H
#define LEXER_BUILDER_H
#include <iostream>
#include "lexer.h"

class lexer_builder{

public:

    lexer::Tokens* tokens = nullptr;

public:
 
    lexer::ScriptCode* script = nullptr;

public:

    lexer_builder() = default;
    ~lexer_builder() noexcept = default;

public:    

    lexer_builder* inizialize(std::string const &file_path);
    lexer_builder* tokenization();
    lexer_builder* build();

private:

    void AppendSemicolon();
    void checkAlpNum(std::string &accumulated_string);

};

#endif*/