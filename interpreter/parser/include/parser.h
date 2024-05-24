    #ifndef PARSER_H
    #define PARSER_H
    #include <iostream>
    #include "../../lexer/include/lexer.h"
    #include "../../lexer/include/lexer_builder.h"

    class parser{

    public:


        struct ASTNode {
            lexer::TokenType type;
            std::string value;
            ASTNode** children;
            size_t size;
            size_t capacity;

            ASTNode(lexer::TokenType type, std::string const &val, size_t initial_capacity = 4)
                : type(type), value(val), size(0), capacity(initial_capacity) {
                children = new ASTNode*[capacity];
            }

            void addChild(ASTNode* child) {
                if (size == capacity) {
                    resize();
                }
                children[size++] = child;
            }

            ~ASTNode() {
                for (size_t i = 0; i < size; ++i) {
                    delete children[i];
                }
                delete[] children;
            }

        private:

            void resize() {
                capacity *= 2;
                ASTNode** new_children = new ASTNode*[capacity];
                for (size_t i = 0; i < size; ++i) {
                    new_children[i] = children[i];
                }
                delete[] children;
                children = new_children;
            }
        };

    };
    #endif