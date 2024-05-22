#ifndef PARSER_BUILDER_H
#define PARSER_BUILDER_H
#include "../../lexer/include/lexer_builder.h"

class parser_builder{

    parser::ASTNode* root = nullptr;

public:

    parser_builder() noexcept = default;
    
    ~parser_builder() noexcept = default;

    parser_builder* buildAST(lexer_builder* lexer);

    
};


#endif