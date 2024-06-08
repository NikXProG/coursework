






















/*#ifndef PARSER_BUILDER_H
#define PARSER_BUILDER_H

#include "../../lexer/include/lexer_builder.h"
#include "parser.h"
#include <map>
#include <string>
class parser_builder {


public:

    bool function_severity = false;

    bool function_defenition = false;

    bool variable_equal = true;
    int pos = 0;

    std::map<std::string, double> variables;

    std::map<std::string, std::string> func;

    parser::ASTNode* root = nullptr;

    std::string past_value;
    lexer::TokenType past_type; 

    lexer::Tokens* tokens;

    parser_builder() noexcept = default;
    
    ~parser_builder() noexcept = default;

    void buildAST(lexer_builder* lexer);

    void parseStatement(int index, parser::ASTNode* parentNode);

    int parseFunction(int current_index);

    void printAST(parser::ASTNode* node, int depth = 0);

    void evalAST(parser::ASTNode* node, int depth = 0, int parenDepth = 0);

    parser::ASTNode* parser_builder::getRoot() {
        return this->root;
    }

    double parse();

    // E -> T±T±T±T± ... ±T
    double expression();

    // T -> F*F*F*F ... *F | F/F/F/F ... /F
    double term();
    
    // F -> N | (E)
    double factor();
    
    bool insideAssign = false;
};

#endif*/