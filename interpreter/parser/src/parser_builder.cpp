/*#include "../include/parser_builder.h"
#include "../../lexer/include/lexer.h"
#include "../include/parser.h"
#include <fstream>
#include <iostream>
#include <stack>
#include <unordered_set>
#include <iostream>
#include <unordered_map>

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
void parser_builder::evalAST(parser::ASTNode* node, int depth, int parenDepth) {
    if (node == nullptr) {
        return;
    }

    // Рекурсивный обход всех дочерних узлов
    for (size_t i = 0; i < node->size; ++i) {
        if (node->children[i]->type == lexer::TokenType::assign) {
            // Найден оператор присваивания, начинаем обработку элементов после него
            insideAssign = true;
            std::cout << lexer::token_to_string(node->children[i]->type) << ": " << node->children[i]->value << " " << depth << std::endl;
        } else if (node->children[i]->type == lexer::TokenType::def && node->children[i]->value == ";") {
            // Найден конец выражения, останавливаем обработку
            std::cout << lexer::token_to_string(node->children[i]->type) << ": " << node->children[i]->value << " " << depth << std::endl;
            insideAssign = false;
        } else if (insideAssign) {
            // Если мы находимся в состоянии обработки после оператора присваивания
            // Выводим все элементы на том же уровне
            std::cout << lexer::token_to_string(node->children[i]->type) << ": " << node->children[i]->value << " " << depth << std::endl;
            if (node->children[i]->size > 0) {
                evalAST(node->children[i], depth, parenDepth); // Продолжаем обработку без увеличения глубины
            }
        } else {
            // Обычный рекурсивный обход
            evalAST(node->children[i], depth + 1, parenDepth);
        }
    }
}

double parser_builder::parse(){
    double result = expression();

    if (pos != tokens->size) {
        throw std::runtime_error("Ошибка в выражении на токене: " + tokens->token[pos].value);
    }
    return result;    
}
double parser_builder::expression(){

    // находим первое слагаемое
    double first = term();
    while (pos < tokens->size) {
        std::string op = tokens->token[pos].value;
        if (op.compare("+") && op.compare("-")) {
            break;
        }
        pos++;
        // находим второе слагаемое (вычитаемое)
        double second = term();
        if (!op.compare("+")) {
            first += second;
        } else {
            first -= second;
        }
    }
    return first;
}
double parser_builder::term(){

        // находим первый множитель
        double first = factor();
        while (pos < tokens->size) {
            std::string op = tokens->token[pos].value;

            if (op.compare("*") && op.compare("/")) {
                break;
            }
            pos++;
            // находим второй множитель (делитель)
            double second = factor();

            if (!op.compare("*")) {
                first *= second;
            } else {
                first /= second;
            }
        }
        return first;    
}
double parser_builder::factor(){

    std::string next = tokens->token[pos].value;
    if (next == "(") {
        pos++;
        // если выражение в скобках, то рекурсивно переходим на обработку подвыражения типа E
        double result = expression();
        if (pos < tokens->size && tokens->token[pos].value == ")") {
            pos++;
            return result;
        } else {
            throw std::runtime_error("Ожидалась ')', но найдено " + tokens->token[pos].value);
        }
    } else {
        pos++;
        // в противном случае токен должен быть числом
        return std::stod(next);
    }    
}

void parser_builder::buildAST(lexer_builder* lexer) {
    this->tokens = lexer->tokens;
    int i = 0;
    root = new parser::ASTNode(lexer::TokenType::root, "ROOT");
    parseStatement(i, root);
}

int parser_builder::parseFunction(int current_index){

    std::string accumulator_string;
    int index_offset = 0;
    int openParenCount = 0; 
    while ( current_index < tokens->size) {

        if (function_severity){
            index_offset++;
        }

        if (function_severity && (tokens->token[current_index].type != lexer::TokenType::paren) ) {
        
            accumulator_string.append(tokens->token[current_index].value);
        }

        if (tokens->token[current_index].type == lexer::TokenType::id && tokens->token[current_index+1].value == "(" ) {
            // Обработка присваивания переменной
            function_severity = true;
            accumulator_string.append(tokens->token[current_index].value + tokens->token[current_index+1].value);

        } 
        if (tokens->token[current_index].type == lexer::TokenType::paren) {
            (tokens->token[current_index].value == "(") ? (openParenCount++) : (openParenCount--);
        } 
        if (tokens->token[current_index].type == lexer::TokenType::def) {
            // Обработка оператора определения (запятая или двоеточие или точка с запятой)
            if (tokens->token[current_index].value == ":") {
                // Обработка двоеточия
                function_defenition = true;
            } 
            else {
                // Обработка точки с запятой

                if (function_defenition){
                    function_defenition = false;
                    index_offset = 0;
                }

                function_severity  = false;
                int posDec = accumulator_string.find(":");

                if (posDec != std::string::npos){
                    func[accumulator_string.substr(0,posDec).append(")")] = accumulator_string.substr(posDec+1 ,accumulator_string.size());
                }else{
                    current_index -= index_offset;
                    return current_index;
                }

                accumulator_string.clear();

            }

        }
        current_index++;  
    }

    if (openParenCount != 0){
        throw std::runtime_error("Syntax error: invalid count paren '('or ')'. missing opening or closing parenthesis");
    }    
    return tokens->size;
}
void parser_builder::parseStatement(int current_index, parser::ASTNode* parentNode) {

    std::stack<parser::ASTNode*> stack;
    
    stack.push(parentNode); // Начинаем с корневого узла в стеке

    int openParenCount = 0; // Счетчик для открывающих скобок

    int new_corrent_index = parseFunction(current_index);

    for (int i =  new_corrent_index; i < tokens->size; i++){

        if (tokens->token[i].type == lexer::TokenType::id) {

            std::string varName = tokens->token[i].value;

            if (tokens->token[i].value == "print"){

                parser::ASTNode* argNode = new parser::ASTNode(tokens->token[i].type, varName);
                stack.top()->addChild(argNode);
                stack.push(argNode); // Добавляем новый узел в стек
                
            } else {
                // Проверка, была ли переменная объявлена ранее
                parser::ASTNode* leafNode = new parser::ASTNode(tokens->token[i].type, tokens->token[i].value);
                stack.top()->addChild(leafNode);

            }

        } else if (tokens->token[i].type == lexer::TokenType::number) {
            // Обработка числа
            parser::ASTNode* leafNode = new parser::ASTNode(tokens->token[i].type, tokens->token[i].value);
            stack.top()->addChild(leafNode);

        } else if (tokens->token[i].type == lexer::TokenType::paren) {

            if (tokens->token[i].value == "(") {
                // Обработка открывающей скобки
                openParenCount++;
                parser::ASTNode* parenNode = new parser::ASTNode(lexer::TokenType::paren, "(");
                stack.top()->addChild(parenNode);
                stack.push(parenNode); // Добавляем новый узел в стек
            } else {
                // Обработка закрывающей скобки

                openParenCount--;
                stack.pop(); // Завершаем текущий узел, переходим к предыдущему
                parser::ASTNode* parenNode = new parser::ASTNode(lexer::TokenType::paren, ")");
                stack.top()->addChild(parenNode); // Добавляем закрывающую скобку к родительскому узлу

            }

        } else if (tokens->token[i].value == ";") {
            // Обработка открывающей скобки
            stack.pop();
            parser::ASTNode* SemiColonNode = new parser::ASTNode(lexer::TokenType::def, ";");
            stack.top()->addChild(SemiColonNode);
        } else if (tokens->token[i].type == lexer::TokenType::assign) {
            // Обработка оператора присваивания
            parser::ASTNode* assignNode = new parser::ASTNode(tokens->token[i].type, tokens->token[i].value);
            stack.top()->addChild(assignNode);
            stack.push(assignNode);

        } else if (tokens->token[i].type == lexer::TokenType::op) {
            // Обработка других операторов
            parser::ASTNode* assignNode = new parser::ASTNode(tokens->token[i].type, tokens->token[i].value);
            stack.top()->addChild(assignNode);

        } else{
            // пропуск пробелов
        }
        
        
    }

    if (openParenCount != 0){
        throw std::runtime_error("Syntax error: invalid count paren '('or ')'. missing opening or closing parenthesis");
    }   

 
}


*/