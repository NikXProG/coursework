#include "../include/token.h"
#include "../include/lexer.h"
#include <fstream>
#include <thread>
#include <chrono>
#include <ranges>


Lexer::Lexer(std::string file_stream) {

    this->f_stream = file_stream;
    this->tokens = new Token[this->max_size];  

}

Lexer* Lexer::readFile(){
    if (this->f_stream.empty()){
        throw new std::runtime_error("The file is not initialized in the lexer object.");
    }
    std::string line;
    std::ifstream read_stream(this->f_stream);

    while (getline (read_stream, line)) {

        this->code.append(line);
    }

    return this;
}

Lexer* Lexer::initFile(std::string const &file) {

    if (file.empty()){
        throw new std::runtime_error("The file is not initialized in the lexer object.");
    }
    this->f_stream = file;
    return this;
}

Lexer* Lexer::lexAnalysis(){

    while(this->nextToken()){}
    return this;
}
bool Lexer::nextToken() {
    int code_size = this->code.size();
    if (this->pos >= code_size){
        return false;
    }

    std::smatch result;

    for (auto &it : tokenTypesList) {

        std::string code = this->code.substr(this->pos);

        if (std::regex_search(code, result, it.second)) {
            if ( !result.str().empty() ) {

                this->pos += result.str().size();
                if (it.first != Token::TokenType::SPACE ){

                    Token token(it.first, result.str(), this->pos);    

                    if (this->token_count >= this->max_size){
                        Token* token_buffer = new Token[max_size+5];

                        for (int i = 0; i < max_size; i++){
                            token_buffer[i] = tokens[i];
                        }
                        delete[] tokens;
                        tokens = token_buffer;
                        max_size += 5;
                    }   

                    tokens[token_count++] = token;   
                }
 
                return true;

            }
        }
    }


    throw std::runtime_error("Error: unexpected character at position " + std::to_string(this->pos));


    return true;
}








/*#include "../include/lexer.h"

std::string lexer::token_to_string(lexer::TokenType token_type) {
    switch (token_type) {
    case lexer::TokenType::number: return "NUMBER";
    case lexer::TokenType::assign: return "ASSIGN";
    case lexer::TokenType::def: return "DEF";
    case lexer::TokenType::id: return "ID";
    case lexer::TokenType::op: return "OP";
    case lexer::TokenType::paren: return "PAREN";
    case lexer::TokenType::skip: return "SKIP";
    case lexer::TokenType::root: return "ROOT";
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
    if (token_string == "ROOT"){
        return lexer::TokenType::root;
    }
    throw std::out_of_range("Error: Value is not token type");

}
*/