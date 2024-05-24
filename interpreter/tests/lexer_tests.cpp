#include <iostream>
#include "lexer_builder.h"
#include "parser_builder.h"
void printAST(parser::ASTNode* node, int depth = 0) {
    if (node == nullptr) {
        return;
    }

    // Вывод отступа для каждого уровня узла
    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";
    }

    // Вывод типа узла и его значение
    std::cout << lexer::token_to_string(node->type) << ": " << node->value << std::endl;

    // Рекурсивный обход всех дочерних узлов
    for (size_t i = 0; i < node->size; ++i) {
        printAST(node->children[i], depth + 1);
    }
}
int main() {
    try {
        auto lex_builder = new lexer_builder();
        auto lexer = lex_builder->inizialize("../../stream.txt")->tokenization();

        auto parser = new parser_builder();
        parser->buildAST(lexer);
        parser->printAST(parser->getRoot() );

    } catch (const std::runtime_error& ex) {
        std::cout << ex.what() << std::endl;
        return -1;
    } catch (const std::out_of_range& ex) {
        std::cout << ex.what();
    } catch (const std::exception& ex) {
        std::cout << ex.what();
    }

    return 0;
}

