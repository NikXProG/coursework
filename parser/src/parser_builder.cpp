#include "../include/parser_builder.h"
#include <fstream>
#include <string>
#include <iostream>

parser_builder::parser_builder() {
    // Реализация конструктора
}

parser_builder::source_code* parser_builder::create_buffer(std::string const &file_path) {
    

    std::ifstream inf(file_path);

    // проверка указателя на nullptr
    if (!inf.is_open()) {
        throw std::ios_base::failure("Error: opening/closing/reading file");
    }

    std::string line;
    
    parser_builder::source_code* Source;

    // длина исходного кода
    while (getline(inf, line)) Source->size++;

    Source->source_array = new parser_builder::str_code[++Source->size];

    if (Source->source_array == nullptr) {
        throw std::bad_alloc();
    }

    int i = 0;

    // смещение указателя
    inf.clear();
    inf.seekg(0);

    // backup
    while (getline(inf, line)) {
        Source->source_array[i].code = new char[line.length() + 1];

        if (Source->source_array[i].code.empty()) {
            throw std::bad_alloc();
        }
        Source->source_array[i].code = line;
        i++;
    }

    return Source;
}

