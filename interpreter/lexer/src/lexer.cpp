#include "../include/lexer.h"

std::string lexer::token_to_string(lexer::TokenType token_type) {
    switch (token_type) {
    case lexer::TokenType::number: return "NUMBER";
    case lexer::TokenType::assign: return "ASSIGN";
    case lexer::TokenType::def: return "DEF";
    case lexer::TokenType::id: return "ID";
    case lexer::TokenType::op: return "OP";
    case lexer::TokenType::paren: return "PAREN";
    case lexer::TokenType::skip: return "SKIP";
    }
    throw std::out_of_range("Error: token value invalid");
}

lexer::TokenType lexer::string_to_token(std::string const &token_string){
    if (token_string == "NUMBER"){
        return lexer::TokenType::number;
    }
    if (token_string == "ASSIGN"){
        return lexer::TokenType::assign;
    }
    if (token_string == "ID"){
        return lexer::TokenType::id;
    }
    if (token_string == "DEF"){
        return lexer::TokenType::def;
    }
    if (token_string == "OP"){
        return lexer::TokenType::op;
    }
    if (token_string == "PAREN"){
        return lexer::TokenType::paren;
    }
    if (token_string == "SKIP"){
        return lexer::TokenType::skip;
    }
    throw std::out_of_range("Error: Value is not token type");

}