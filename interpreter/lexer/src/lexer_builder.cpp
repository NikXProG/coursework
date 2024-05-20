#include "../include/lexer_builder.h"
#include <fstream>
#include <iostream>
#include <string>
#include <regex>

lexer_builder* lexer_builder::inizialize(std::string const &file_path){
    
    std::ifstream inf(file_path);

    // проверка указателя на nullptr
    if (!inf.is_open()) {
        throw std::ios_base::failure("Error: opening/closing/reading file");
    }

    script = new lexer::ScriptCode();

    std::string line;
    size_t pos;
    
    // Первый проход для определения размера массива
    while (getline(inf, line)) {
        // обработка инструкций в одну строку ;;;; 
        while ((pos = line.find(';')) != std::string::npos) {
            if (!line.substr(0, pos).empty()) {
               script->size++;
            }
            line.erase(0, pos + 1);
        }

    }

    // Проверка на пустую строку после последнего `;`
    if (!line.empty()) {
        throw std::runtime_error("Syntax error: incorrect characters at the end of the program (code -1)");
    }

    // Сброс потока для повторного чтения файла
    inf.clear();
    inf.seekg(0);

    script->code = new lexer::CodeLine[script->size];

    if (script->code == nullptr) {
        throw std::bad_alloc();
    }

    // backup
    int i = 0;
    while (getline(inf, line)) {

        // обработка инструкций в одну строку ;;;; 
        while ((pos = line.find(';')) != std::string::npos) {
            std::string substring = line.substr(0, pos);
            if (!substring.empty()) {
                if (script->code[i].value.empty()){
                    script->code[i].value = substring;
                }
                else{
                    script->code[i].value.append(substring);   
                }
                script->code[i].size = script->code[i].value.size();
                i++;
            }    

            line.erase(0, pos + 1);
        }
        if (!line.empty()){ script->code[i].value = line; }
    }

    return this;
}

lexer_builder* lexer_builder::tokenization(){

    std::string accumulated_string;
    std::string line;

    tokens = new lexer::Tokens();

    for (int i= 0; i < script->size; i++){

        line = script->code[i].value;
        for (int j = 0; j < script->code[i].size; ++j) {


            if ( isalpha(line[j]) ||  ( isdigit(line[j]) && isalpha(line[j-1])) ){
                accumulated_string += line[j];

            } else if( isdigit(line[j]) || ( isdigit(line[j]) && !isalpha(line[j-1]) ) ){
                accumulated_string += line[j];

            }else{
  
                if (!accumulated_string.empty()){
                    tokens->size++;
                    accumulated_string.clear();
                }

                if (std::strchr("=()+*-:/, ", line[j]))  {
                    tokens->size++;
                } else{
                    throw  std::runtime_error("Syntax error: invalid character in line" + line[j]);
                }
 
            }             
        }
        if (!accumulated_string.empty()){
            tokens->size++;
            accumulated_string.clear();
        }
    }


    tokens->token = new lexer::Token[tokens->size];

    for (int i= 0; i < script->size; i++){
        line = script->code[i].value;
        for (size_t j = 0; j < script->code[i].size; ++j) {
            if ( isalpha(line[j]) ||  ( isdigit(line[j]) && isalpha(line[j-1])) ){
                accumulated_string += line[j];
            }else if( isdigit(line[j]) || ( isdigit(line[j]) && !isalpha(line[j-1]) ) ){
                accumulated_string += line[j];
            }else{
                checkAlpNum(accumulated_string);
                if (line[j] == '='){
                    tokens->token[tokens->token->index_token].type = lexer::string_to_token("ASSIGN");
                    tokens->token[tokens->token->index_token++].value = line[j];                   

                } else if(line[j] == ' '){
                    tokens->token[tokens->token->index_token].type = lexer::string_to_token("SKIP"); 
                    tokens->token[tokens->token->index_token++].value = line[j];  
                }
                else if((line[j] == '(') || (line[j] == ')')){
                    tokens->token[tokens->token->index_token].type = lexer::string_to_token("PAREN"); 
                    tokens->token[tokens->token->index_token++].value = line[j];  
                }
                else if( (line[j] == ':') || (line[j] == ',') ){
                    tokens->token[tokens->token->index_token].type = lexer::string_to_token("DEF"); 
                    tokens->token[tokens->token->index_token++].value = line[j];  
                } 
                else if((line[j] == '-') || (line[j] == '+') || (line[j] == '/') || (line[j] == '*')){
                    tokens->token[tokens->token->index_token].type = lexer::string_to_token("OP"); 
                    tokens->token[tokens->token->index_token++].value = line[j];  
                }                 
                else{
                    throw  std::runtime_error("Syntax error: invalid character in line" + line[j]);
                }
 
            }             
        }
        checkAlpNum(accumulated_string);
    }   

    return this;
}

void lexer_builder::checkAlpNum(std::string &accumulated_string){
    if (!accumulated_string.empty()){
        tokens->token[tokens->token->index_token].type = ((isalpha(accumulated_string[0])) ? lexer::string_to_token("ID") : lexer::string_to_token("NUMBER"));
        tokens->token[tokens->token->index_token++].value = accumulated_string;
        accumulated_string.clear();
    }

}

