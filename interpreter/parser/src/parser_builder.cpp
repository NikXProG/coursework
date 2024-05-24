#include "../include/parser_builder.h"
#include "../../lexer/include/lexer.h"
#include "../include/parser.h"
#include <fstream>
#include <iostream>
#include <stack>

void parser_builder::printAST(parser::ASTNode* node, int depth) {
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
void parser_builder::buildAST(lexer_builder* lexer) {
    this->tokens = lexer->tokens;
    size_t index = 0;
    root = new parser::ASTNode(lexer::TokenType::root, "ROOT");
    for (int i = 0; i < tokens->size; i++){
        std::cout << lexer::token_to_string(tokens->token[i].type) << " " << tokens->token[i].value << std::endl;
    }
    parseStatement(index, root);
}


void parser_builder::parseStatement(size_t& index, parser::ASTNode* parentNode) {
    while (index < tokens->size) {
        if (tokens->token[index].type == lexer::TokenType::id) {
            // Обработка идентификатора
            parser::ASTNode* idNode = new parser::ASTNode(lexer::TokenType::id, tokens->token[index].value);
            parentNode->addChild(idNode);
            index++;
        } else if (tokens->token[index].type == lexer::TokenType::assign) {
            // Обработка оператора присваивания
            parser::ASTNode* assignNode = new parser::ASTNode(lexer::TokenType::assign, "=");
            parentNode->addChild(assignNode);
            index++;
        } else if (tokens->token[index].type == lexer::TokenType::op) {
            // Обработка оператора
            parser::ASTNode* opNode = new parser::ASTNode(lexer::TokenType::op, tokens->token[index].value);
            parentNode->addChild(opNode);
            index++;
        } else if (tokens->token[index].type == lexer::TokenType::def) {
            // Обработка оператора определения
            parser::ASTNode* defNode = new parser::ASTNode(lexer::TokenType::def, ":");
            parentNode->addChild(defNode);
            index++;
        } else if (tokens->token[index].type == lexer::TokenType::paren) {
            // Обработка скобок
            parser::ASTNode* parenNode = new parser::ASTNode(lexer::TokenType::paren, tokens->token[index].value);
            parentNode->addChild(parenNode);
            index++;
        } else if (tokens->token[index].type == lexer::TokenType::skip) {
            // Пропуск пробелов
            index++;
        } else {
            throw std::runtime_error("Syntax error: unexpected token");
        }
    }
}