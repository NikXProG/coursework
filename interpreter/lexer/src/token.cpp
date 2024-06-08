#include "../include/token.h"

std::string Token::token_to_string(TokenType type) {
    switch (type) {
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::ID: return "ID";
        case TokenType::COLON: return "COLON";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::CONSOLE: return "PRINT";
        case TokenType::MULT: return "MULT";
        case TokenType::DIV: return "DIV";
        case TokenType::MINUS: return "MINUS";
        case TokenType::SPACE: return "SPACE";
        case TokenType::PLUS: return "PLUS";
        case TokenType::SEMICOLON: return "SEMICOLON";
    }
    throw std::out_of_range("Error: token value invalid");    
}

Token::TokenType Token::string_to_token(std::string const &token_string){

    if (token_string == "NUMBER"){
        return TokenType::NUMBER;
    }
    if (token_string == "ASSIGN"){
        return TokenType::ASSIGN;
    }
    if (token_string == "ID"){
        return TokenType::ID;
    }
    if(token_string == "DIV"){
        return TokenType::DIV;
    }
    if (token_string == "COLON"){
        return TokenType::COLON;
    }
    if (token_string == "RPAREN"){
        return TokenType::RPAREN;
    }
    if (token_string == "LPAREN"){
        return TokenType::LPAREN;
    }
    if (token_string == "SEMICOLON"){
        return TokenType::SEMICOLON;
    }
    if (token_string == "PLUS"){
        return TokenType::PLUS;
    }
    if (token_string == "MINUS"){
        return TokenType::MINUS;
    }
    if (token_string == "SPACE"){
        return TokenType::SPACE;
    }
    if (token_string == "MULT"){
        return TokenType::MULT;
    }
    if (token_string == "PRINT"){
        return TokenType::CONSOLE;
    }

    throw std::out_of_range("Error: Value is not token type");

}