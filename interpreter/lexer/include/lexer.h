#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <regex>
#include <unordered_map> // Добавлено для std::unordered_map
#include "token.h"

class Lexer {
    
    std::unordered_map<Token::TokenType, std::regex> tokenTypesList {
        {Token::TokenType::CONSOLE, std::regex("^print")},
        {Token::TokenType::NUMBER, std::regex("^([0-9]+[.0-9]*)")},
        {Token::TokenType::ID, std::regex("^((?!print)[a-zA-Z][a-zA-Z0-9]*)(?:\\s*\\(([^()]*)\\)\\s*)?")},
        {Token::TokenType::SEMICOLON, std::regex("^\\;")},
        {Token::TokenType::SPACE, std::regex("^([ \\n\\r\\t,])")},
        {Token::TokenType::ASSIGN, std::regex("^\\=")},
        {Token::TokenType::COLON, std::regex("^\\:")},
        {Token::TokenType::MULT, std::regex("^\\*")},
        {Token::TokenType::DIV, std::regex("^\\/")},
        {Token::TokenType::PLUS, std::regex("^\\+")},
        {Token::TokenType::MINUS, std::regex("^\\-")},
        {Token::TokenType::LPAREN, std::regex("^\\(")},
        {Token::TokenType::RPAREN, std::regex("^\\)")}
    };

    std::string code;
    std::string f_stream;
    int pos = 0;
    Token* tokens = nullptr; 
    int max_size = 10;  
    int token_count = 0;

public:

    Lexer(std::string file_stream);

    Lexer() noexcept = default;

    ~Lexer() noexcept { delete[] tokens; }

public:

    Lexer* readFile();
    Lexer* lexAnalysis();
    bool nextToken();
    Lexer* Lexer::initFile(std::string const & file);
    
public:


    Token* getTokenList() const { return tokens; }
    int getTokenSize() const { return token_count; }
    std::unordered_map<Token::TokenType, std::regex> getTokenTypes() const { return tokenTypesList; }

};

#endif // LEXER_H







/*#ifndef LEXER_H
#define LEXER_H
#include <iostream>

class lexer{
    
public:

    enum class TokenType {
        number,
        assign,
        id,
        op,
        def,
        paren,
        skip,
        root
    };

public:

    struct CodeLine {
        size_t size = 0;
        std::string value;
    };


    struct ScriptCode {
        size_t size = 0;
        CodeLine* code = nullptr;
    };

    struct Token {
        lexer::TokenType type;
        std::string value;
        size_t index_token = 0;


    };

    struct Tokens{
        size_t size = 0;
        Token* token= nullptr;
    };


public:

    lexer() noexcept = default;

    ~lexer() noexcept = default;

public:

    static std::string token_to_string(TokenType token_type);
    static TokenType string_to_token(std::string const &token_string);
    static bool valueOf(TokenType token_type);
};
#endif*/