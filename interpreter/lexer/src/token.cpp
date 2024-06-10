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
        case TokenType::LOAD: return "LOAD";
        case TokenType::SAVE: return "SAVE";
        case TokenType::RAND: return "RAND";
        case TokenType::CONCAT: return "CONCAT";
        case TokenType::FREE: return "FREE";
        case TokenType::REMOVE: return "REMOVE";
        case TokenType::COPY: return "COPY";
        case TokenType::SORT: return "SORT";
        case TokenType::PERMUTE: return "PERMUTE";
        case TokenType::XOR: return "XOR";
        case TokenType::INTERSECT: return "INTERSECT";
        case TokenType::STAT: return "STAT";
        case TokenType::PRINT_ALL: return "PRINT_ALL";
        case TokenType::PRINT_EL_LST: return "PRINT_EL_LST";
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
    if (token_string == "LOAD"){
        return TokenType::LOAD;
    }
    if (token_string == "SAVE"){
        return TokenType::SAVE;
    }
    if (token_string == "RAND"){
        return TokenType::LOAD;
    }
    if (token_string == "CONCAT"){
        return TokenType::SAVE;
    }
    if (token_string == "FREE"){
        return TokenType::FREE;
    }
    if (token_string == "REMOVE"){
        return TokenType::REMOVE;
    }
    if (token_string == "COPY"){
        return TokenType::COPY;
    }
    if (token_string == "SORT"){
        return TokenType::SORT;
    }
    if (token_string == "PERMUTE"){
        return TokenType::PERMUTE;
    }
    if (token_string == "PERMUTE"){
        return TokenType::PERMUTE;
    }
    if (token_string == "XOR"){
        return TokenType::XOR;
    }
    if (token_string == "INTERSECT"){
        return TokenType::INTERSECT;
    }    
    throw std::out_of_range("Error: Value is not token type");

}