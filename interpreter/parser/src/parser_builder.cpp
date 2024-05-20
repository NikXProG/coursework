#include "../include/parser_builder.h"
#include <fstream>
#include <iostream>
#include <stack>


parser_builder::parser_builder() {
    // Реализация конструктора
}

parser_builder* parser_builder::lexer(std::string const &file_path) {

    std::ifstream inf(file_path);

    // проверка указателя на nullptr
    if (!inf.is_open()) {
        throw std::ios_base::failure("Error: opening/closing/reading file");
    }

    std::string line;
    size_t pos;

    // Первый проход для определения размера массива
    while (getline(inf, line)) {
        // обработка инструкций в одну строку ;;;; 
        while ((pos = line.find(';')) != std::string::npos) {
            if (!line.substr(0, pos).empty()) {
                parser_builder::Source.size++;
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


    parser_builder::Source.source_array = new parser_builder::str_code[parser_builder::Source.size];

    if (parser_builder::Source.source_array == nullptr) {
        throw std::bad_alloc();
    }

    // backup
    int i = 0;
    while (getline(inf, line)) {

        // обработка инструкций в одну строку ;;;; 
        while ((pos = line.find(';')) != std::string::npos) {
            std::string substring = line.substr(0, pos);
            if (!substring.empty()) {
                if (Source.source_array[i].code.empty()){
                    Source.source_array[i].code = substring;
                }
                else{
                    Source.source_array[i].code.append(substring);   
                }
                i++;
            }    

            line.erase(0, pos + 1);
        }
        if (!line.empty()){ parser_builder::Source.source_array[i].code = line; }
    }

    return this;
}

parser_builder* parser_builder::info(){
   for (size_t i = 0; i < parser_builder::Source.size ; i++) {
    std::cout << parser_builder::Source.source_array[i].code << std::endl;
   }
    return this;
}


parser_builder* parser_builder::create_stack(){

    return this;
}