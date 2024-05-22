#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include "..\..\lexer\include\lexer.h"
#include "..\..\lexer\include\lexer_builder.h"

class parser{

public:

    struct ASTNode {
        lexer::TokenType type; // Тип токена
        std::string value;     // Значение токена
        ASTNode** children;    // Массив указателей на дочерние узлы
        size_t size;           // Количество дочерних узлов
        size_t capacity;       // Ёмкость массива

        // Конструктор
        ASTNode(lexer::TokenType type, std::string const &val, size_t initial_capacity = 4)
            : type(type), value(val), size(0), capacity(initial_capacity) {
            children = new ASTNode*[capacity]; // Выделяем память под массив указателей
        }

        // Метод добавления дочернего узла
        void addChild(ASTNode* child) {
            if (size == capacity) {
                resize(); // Увеличиваем массив, если он заполнен
            }
            children[size++] = child;
        }

        // Деструктор
        ~ASTNode() {
            for (size_t i = 0; i < size; ++i) {
                delete children[i]; // Удаляем все дочерние узлы
            }
            delete[] children; // Удаляем сам массив указателей
        }

    private:
        // Метод увеличения массива дочерних узлов
        void resize() {
            capacity *= 2; // Увеличиваем ёмкость массива в два раза
            ASTNode** new_children = new ASTNode*[capacity]; // Создаём новый массив
            for (size_t i = 0; i < size; ++i) {
                new_children[i] = children[i]; // Копируем старые указатели в новый массив
            }
            delete[] children; // Удаляем старый массив
            children = new_children; // Присваиваем указатель на новый массив
        }
    };

};
#endif