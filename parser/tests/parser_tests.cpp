#include <iostream>
#include <parser_builder.h>

int main() {
    auto builder = new parser_builder();
    auto parser = builder->create_buffer("../../stream.txt");

   for (size_t i = 0; i < parser->Source->size; i++) {
    std::cout << parser->Source->source_array[i].code << std::endl;
   }
    return 0;
}