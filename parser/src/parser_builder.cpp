#include "../include/parser_builder.h"
#include <fstream>
#include <string>
#include <iostream>

parser_builder::parser_builder() {
    // Реализация конструктора
}

parser_builder* parser_builder::create_buffer(std::string const &file_path) {
    

    std::ifstream inf(file_path);

    // проверка указателя на nullptr
    if (!inf.is_open()) {
        throw std::ios_base::failure("Error: opening/closing/reading file");
    }

    std::string line;
    

    // длина исходного кода
    while (getline(inf, line)) parser_builder::Source->size++;

    parser_builder::Source->source_array = new parser_builder::str_code[++parser_builder::Source->size];

    if (parser_builder::Source->source_array == nullptr) {
        throw std::bad_alloc();
    }

    int i = 0;

    // смещение указателя
    inf.clear();
    inf.seekg(0);

    // backup
    while (getline(inf, line)) {
        parser_builder::Source->source_array[i].code = new char[line.length() + 1];

        if (parser_builder::Source->source_array[i].code.empty()) {
            throw std::bad_alloc();
        }
        parser_builder::Source->source_array[i].code = line;
        i++;
    }

    return this;
}

parser_builder* parser_builder::info(){
   for (size_t i = 0; i < parser_builder::Source->size ; i++) {
    std::cout << parser_builder::Source->source_array[i].code << std::endl;
   }
    return this;
}