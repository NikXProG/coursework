#include "../include/parser_builder.h"
#include "../include/parser.h"
#include <fstream>
#include <iostream>
#include <stack>

parser_builder* parser_builder::buildAST(lexer_builder* lexer){
    root = new parser::ASTNode();
`   
    return this;
}