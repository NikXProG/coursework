#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <unordered_map>
#include <regex>

class Token{

public:

    enum class TokenType {
        NUMBER,
        ID,
        SEMICOLON,
        SPACE,
        ASSIGN,
        MULT,
        DIV,
        PLUS,
        MINUS,
        CONSOLE,
        LPAREN,
        RPAREN,
        COLON,
        FUNCTION
    }; 

private:

    TokenType type;
    std::string value;
    int pos_token;   

public:

    Token() noexcept = default;

    Token(TokenType type, std::string value, int pos_token) : type(type), value(value), pos_token(pos_token) {}

    ~Token() noexcept = default;    

public:

    TokenType getType() const { return type; }
    std::string getValue() const { return value; }    
    int getPos() const { return pos_token; }   

public:

    static std::string token_to_string(TokenType type);
    static TokenType string_to_token(std::string const &token_string); 

};

#endif // TOKEN_H