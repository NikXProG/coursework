#include <iostream>
#include <parser_builder.h>

int main(){
    
    auto builder = new parser_builder();
    auto parser = builder->create_buffer("file.txt");

    for (int i = 0; i < parser->size; i++){
        std::cout << parser->source_array[i].code << std::endl;
    }
}