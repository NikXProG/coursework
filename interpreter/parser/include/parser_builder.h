#ifndef PARSER_BUILDER_H
#define PARSER_BUILDER_H

#include "../../lexer/include/lexer_builder.h"
#include "parser.h"

class parser_builder {

public:

    parser::ASTNode* root = nullptr;

    lexer::Tokens* tokens;

    parser_builder() noexcept = default;
    
    ~parser_builder() noexcept = default;

    void buildAST(lexer_builder* lexer);

    void parseStatement(size_t& index, parser::ASTNode* parentNode);

    void printAST(parser::ASTNode* node, int depth = 0);
    
    parser::ASTNode* getRoot() { return root; }

};

#endif