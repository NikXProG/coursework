#ifndef PARSER_H
#define PARSER_H
#include <any>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cctype>
#include <sstream>
#include <fstream>
#include <random>
#include "recursive_descent_parser.h"
#include "./AST/expression_node.h"
#include "./AST/statement_node.h"
#include "./AST/bin_operator_node.h"
#include "./AST/unar_operator_node.h"
#include "./AST/variable_node.h"
#include "./AST/number_node.h"
#include "../../lexer/include/lexer.h"
#include "../../lexer/include/token.h"

class Parser {
    
    Token* tokens = nullptr;
    int pos = 0;
    int tokens_size;
    bool isFunc = false;
    std::string current_func;
    std::string current_func_def;

private:

    std::unordered_map<std::string, std::any> stack_variable;
    std::unordered_map<std::string, std::string> stack_function;
    std::unordered_map<std::string, std::string> stack_function_variable;
    std::unordered_map<std::string, std::vector<int>> array;

public:

    Parser() noexcept = default;

    Parser(Lexer& lexer) {
        this->tokens = lexer.getTokenList();
        this->tokens_size = lexer.getTokenSize();
        srand(time(nullptr));
    }

    Token* match(std::vector<Token::TokenType> tokenType) {
        if (this->pos < this->tokens_size) {

            Token* currentToken = &this->tokens[this->pos]; 

            for (auto& it: tokenType) {

                if ( currentToken->getType() == it ) { 
                    this->pos += 1;
                    return currentToken; 
                }
            }
        }
        return nullptr;
    }


    Token require(std::vector<Token::TokenType> tokenType) {

        auto tokenPtr = this->match(tokenType);
        if (tokenPtr == nullptr) {
            throw std::runtime_error("Error: unexpected character at position " + std::to_string(this->pos));
        }
        return *tokenPtr;
    }


    ExpressionNode* parse() {

        StatementNode* root = new StatementNode(); 

        while (this->pos < this->tokens_size){

            auto codeString = this->parseExpression();
            this->require({Token::TokenType::SEMICOLON}); 
            root->addNode(codeString); 

        }

        return root; // Возвращаем указатель на созданный объект
    }

    ExpressionNode* parseSystemBoxFunction() {    

        auto opLog = this->match({Token::TokenType::CONSOLE});
        
        if (opLog != nullptr){
            return new UnarOperationNode(opLog, this->parseRightExpression());
        }


        auto opFunc = this->match({
        Token::TokenType::LOAD, 
        Token::TokenType::SAVE, 
        Token::TokenType::RAND, 
        Token::TokenType::CONCAT, 
        Token::TokenType::FREE,
        Token::TokenType::REMOVE, 
        Token::TokenType::COPY, 
        Token::TokenType::SORT,
        Token::TokenType::PERMUTE,
        Token::TokenType::XOR,
        Token::TokenType::INTERSECT, 
        Token::TokenType::STAT,
        Token::TokenType::PRINT_ALL,
        Token::TokenType::PRINT_EL_LST                    
        });

        if (opFunc != nullptr){

            return new VariableNode(opFunc); 
        }


        throw std::runtime_error("invalid operator " + std::to_string(this->pos) );
    }   

    ExpressionNode* parseExpression() {

        auto id = this->match({Token::TokenType::ID});
        // если не переменная, то смотрим на print
        if (id == nullptr) {
            
            auto printNode = this->parseSystemBoxFunction();
            return printNode; 
        }

        // уменьшаем так как отработали лишнее действие поверкой 
        this->pos -= 1;

        // пришли сюда значит переменная либо число. Парсим
        auto variableNode = parseIdOrNumber();


        auto assignOperator = this->match({Token::TokenType::ASSIGN, Token::TokenType::COLON});

        if (assignOperator != nullptr) {

            auto rightExpressionNode = this->parseRightExpression();
            BinOperatorNode* assignNode = new BinOperatorNode(assignOperator, variableNode, rightExpressionNode);
            return assignNode;
        }
        
        throw std::runtime_error("An assignment operator is expected after the variable");
    }

    ExpressionNode* parseRightExpression() { 

        auto leftNode = this->parseParentheses();

        auto op = this->match({Token::TokenType::PLUS, Token::TokenType::MINUS, Token::TokenType::MULT, Token::TokenType::DIV});

        while (op != nullptr){

            auto rightNode = this->parseParentheses();
            
            BinOperatorNode* binaryNode = new BinOperatorNode(op, leftNode, rightNode); 
            leftNode = binaryNode; 

            op = this->match({Token::TokenType::PLUS, Token::TokenType::MINUS, Token::TokenType::MULT, Token::TokenType::DIV});
    
        }

        return leftNode; 
    }
    
    ExpressionNode* parseParentheses() { // Changed return type to pointer
        if (this->match({Token::TokenType::LPAREN}) != nullptr){
            auto node = this->parseRightExpression();
            this->require({Token::TokenType::RPAREN});
            return node;
        } else {
            // если нет скобок поверяем на цифру или переменную
            return this->parseIdOrNumber();
        }
    }

    ExpressionNode* parseIdOrNumber() { // Changed return type to pointer

        auto number = this->match({Token::TokenType::NUMBER});

        if (number != nullptr){
            return new NumberNode(number);
        }

        auto id = this->match({Token::TokenType::ID});

        if (id != nullptr){

            return new VariableNode(id); 
        }
        
        auto colon = this->match({Token::TokenType::SEMICOLON});

        if (colon != nullptr){
            Token* special_tpk = new Token(Token::TokenType::ID, "\n", this->pos);
            this->pos-=1;
            return new VariableNode(special_tpk); 
        }        
        
        throw std::runtime_error("Waiting number or defenition variable in position " + this->pos);
    }


    std::string toUpper(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
            return std::toupper(c);
        });
        return result;
    }

    std::vector<std::string> split(const std::string &str, const std::string &delimiters,  bool saveDelimiters = false) {
        std::vector<std::string> tokens;
        size_t start = 0, end = 0;
        while ((end = str.find_first_of(delimiters, start)) != std::string::npos) {
            if (end != start) {
                tokens.push_back(str.substr(start, end - start)); // Записываем подстроку без разделителя
            }
            if (saveDelimiters) {
                tokens.push_back(str.substr(end, 1)); // Записываем разделитель, если флаг установлен
            }
            start = end + 1;
        }
        if (start < str.length()) {
            tokens.push_back(str.substr(start)); // Записываем последнюю часть строки
        }
        return tokens;
    }

    std::any run(ExpressionNode* node) {
        if (auto numNode = dynamic_cast<NumberNode*>(node)) {
            if (numNode->number->getValue().find('.') != std::string::npos){
                return std::stof(numNode->number->getValue());
            }else{
                return std::stoi(numNode->number->getValue());
            }

        } else if (auto unarNode = dynamic_cast<UnarOperationNode*>(node)) {
            switch (unarNode->op->getType()) {
                case Token::TokenType::CONSOLE:

                    if (this->run(unarNode->operand).type() == typeid(float)){
                        std::cout << std::any_cast<float>(this->run(unarNode->operand)) << std::endl;
                    }

                    else if (this->run(unarNode->operand).type() == typeid(int)){
                        std::cout << std::any_cast<int>(this->run(unarNode->operand)) << std::endl;
                    }
                    else {
                        if (this->run(unarNode->operand).type() == typeid(std::string) && !std::any_cast<std::string>(this->run(unarNode->operand)).compare("\n")){
                            std::cout << std::endl;
                        }else{
                            throw new std::runtime_error("invalid token in function print");
                        }
                    }
                    return {};
                default:
                    throw std::runtime_error("Unsupported unary operator");
            }
        } else if (auto binNode = dynamic_cast<BinOperatorNode*>(node)) {
            //std::cout << binNode->op->getValue() << std::endl;
            
            
            switch (binNode->op->getType()) {
                case Token::TokenType::PLUS:

                   if (this->isFunc) {

                        std::string leftExpression;
                        std::string rightExpression;

                        if (this->run(binNode->leftNode).type() == typeid(std::string))
                            leftExpression = std::any_cast<std::string>(this->run(binNode->leftNode));
                        
                        if (this->run(binNode->leftNode).type() == typeid(int))
                            leftExpression = std::to_string(std::any_cast<int>(this->run(binNode->rightNode)));
                        
                        if (this->run(binNode->leftNode).type() == typeid(float))
                            leftExpression = std::to_string(std::any_cast<float>(this->run(binNode->rightNode)));

                        if (this->run(binNode->rightNode).type() == typeid(std::string))
                            rightExpression = std::any_cast<std::string>(this->run(binNode->rightNode));
                        
                        if (this->run(binNode->rightNode).type() == typeid(int))
                            rightExpression = std::to_string(std::any_cast<int>(this->run(binNode->rightNode)));
                      
                        if (this->run(binNode->rightNode).type() == typeid(float))
                            rightExpression = std::to_string(std::any_cast<float>(this->run(binNode->rightNode)));

                        // If both expressions are not empty, then the multiplication is between two variables
                        if (!leftExpression.empty() && !rightExpression.empty()) {
                            this->current_func_def = leftExpression + binNode->op->getValue() + rightExpression;
                            return "(" + leftExpression + binNode->op->getValue() + rightExpression + ")";
                        }
                    }

                    if ((this->run(binNode->leftNode).type() == typeid(float)) && (this->run(binNode->rightNode).type() == typeid(float))){
                        return std::any_cast<float>(this->run(binNode->leftNode)) + std::any_cast<float>(this->run(binNode->rightNode));
                    }
                    if ((this->run(binNode->leftNode).type() == typeid(int)) && (this->run(binNode->rightNode).type() == typeid(int))){
                        return std::any_cast<int>(this->run(binNode->leftNode)) + std::any_cast<int>(this->run(binNode->rightNode));
                    }
                    if ((this->run(binNode->leftNode).type() == typeid(float)) && (this->run(binNode->rightNode).type() == typeid(int))){
                        float leftValue = std::any_cast<float>(this->run(binNode->leftNode));
                        int rightValue = std::any_cast<int>(this->run(binNode->rightNode));
                        return leftValue + static_cast<float>(rightValue);
                    }
                    if ((this->run(binNode->leftNode).type() == typeid(int)) && (this->run(binNode->rightNode).type() == typeid(float))){
                        int leftValue = std::any_cast<int>(this->run(binNode->leftNode));
                        float rightValue = std::any_cast<float>(this->run(binNode->rightNode));
                        return static_cast<float>(leftValue) + rightValue;
                    }
                    break;
                case Token::TokenType::MINUS:

                    if (this->isFunc) {

                        std::string leftExpression;
                        std::string rightExpression;

                        if (this->run(binNode->leftNode).type() == typeid(std::string))
                            leftExpression = std::any_cast<std::string>(this->run(binNode->leftNode));
                        
                        if (this->run(binNode->leftNode).type() == typeid(int))
                            leftExpression = std::to_string(std::any_cast<int>(this->run(binNode->leftNode)));
                        
                        if (this->run(binNode->leftNode).type() == typeid(float))
                            leftExpression = std::to_string(std::any_cast<float>(this->run(binNode->leftNode)));

                        if (this->run(binNode->rightNode).type() == typeid(std::string))
                            rightExpression = std::any_cast<std::string>(this->run(binNode->rightNode));
                        
                        if (this->run(binNode->rightNode).type() == typeid(int))
                            rightExpression = std::to_string(std::any_cast<int>(this->run(binNode->rightNode)));
                      
                        if (this->run(binNode->rightNode).type() == typeid(float))
                            rightExpression = std::to_string(std::any_cast<float>(this->run(binNode->rightNode)));

                        // If both expressions are not empty, then the multiplication is between two variables
                        if (!leftExpression.empty() && !rightExpression.empty()) {
                            this->current_func_def = leftExpression + binNode->op->getValue() + rightExpression;
                            return "(" + leftExpression + binNode->op->getValue() + rightExpression + ")";
                        }
                    }


                    if ((this->run(binNode->leftNode).type() == typeid(float)) && (this->run(binNode->rightNode).type() == typeid(float))){
                        return std::any_cast<float>(this->run(binNode->leftNode)) - std::any_cast<float>(this->run(binNode->rightNode));
                    }
                    if ((this->run(binNode->leftNode).type() == typeid(int)) && (this->run(binNode->rightNode).type() == typeid(int))){
                        return std::any_cast<int>(this->run(binNode->leftNode)) - std::any_cast<int>(this->run(binNode->rightNode));
                    }

                    if ((this->run(binNode->leftNode).type() == typeid(float)) && (this->run(binNode->rightNode).type() == typeid(int))){
                        float leftValue = std::any_cast<float>(this->run(binNode->leftNode));
                        int rightValue = std::any_cast<int>(this->run(binNode->rightNode));
                        return leftValue - static_cast<float>(rightValue);
                    }
                    if ((this->run(binNode->leftNode).type() == typeid(int)) && (this->run(binNode->rightNode).type() == typeid(float))){
                        int leftValue = std::any_cast<int>(this->run(binNode->leftNode));
                        float rightValue = std::any_cast<float>(this->run(binNode->rightNode));
                        return static_cast<float>(leftValue) - rightValue;
                    }
                    break;
                case Token::TokenType::MULT:
                    if (this->isFunc) {

                        std::string leftExpression;
                        std::string rightExpression;

                        if (this->run(binNode->leftNode).type() == typeid(std::string))
                            leftExpression = std::any_cast<std::string>(this->run(binNode->leftNode));
                        
                        if (this->run(binNode->leftNode).type() == typeid(int))
                            leftExpression = std::to_string(std::any_cast<int>(this->run(binNode->leftNode)));
                        
                        if (this->run(binNode->leftNode).type() == typeid(float))
                            leftExpression = std::to_string(std::any_cast<float>(this->run(binNode->leftNode)));

                        if (this->run(binNode->rightNode).type() == typeid(std::string))
                            rightExpression = std::any_cast<std::string>(this->run(binNode->rightNode));
                        
                        if (this->run(binNode->rightNode).type() == typeid(int))
                            rightExpression = std::to_string(std::any_cast<int>(this->run(binNode->rightNode)));
                      
                        if (this->run(binNode->rightNode).type() == typeid(float))
                            rightExpression = std::to_string(std::any_cast<float>(this->run(binNode->rightNode)));

                        // If both expressions are not empty, then the multiplication is between two variables
                        if (!leftExpression.empty() && !rightExpression.empty()) {
                            this->current_func_def = leftExpression + binNode->op->getValue() + rightExpression;
                            return "(" + leftExpression + binNode->op->getValue() + rightExpression + ")";
                        }
                    }
                    
                    if ((this->run(binNode->leftNode).type() == typeid(float)) && (this->run(binNode->rightNode).type() == typeid(float))){
                        return std::any_cast<float>(this->run(binNode->leftNode)) * std::any_cast<float>(this->run(binNode->rightNode));
                    }
                    
                    if ((this->run(binNode->leftNode).type() == typeid(int)) && (this->run(binNode->rightNode).type() == typeid(int))){
                        return std::any_cast<int>(this->run(binNode->leftNode)) * std::any_cast<int>(this->run(binNode->rightNode));
                    }    

                    if ((this->run(binNode->leftNode).type() == typeid(float)) && (this->run(binNode->rightNode).type() == typeid(int))){
                        float leftValue = std::any_cast<float>(this->run(binNode->leftNode));
                        int rightValue = std::any_cast<int>(this->run(binNode->rightNode));
                        return leftValue * static_cast<float>(rightValue);
                    }

                    if ((this->run(binNode->leftNode).type() == typeid(int)) && (this->run(binNode->rightNode).type() == typeid(float))){
                        int leftValue = std::any_cast<int>(this->run(binNode->leftNode));
                        float rightValue = std::any_cast<float>(this->run(binNode->rightNode));
                        return static_cast<float>(leftValue) * rightValue;
                    }          
                    break;
                case Token::TokenType::DIV:

                    if (this->isFunc) {

                        std::string leftExpression;
                        std::string rightExpression;

                        if (this->run(binNode->leftNode).type() == typeid(std::string))
                            leftExpression = std::any_cast<std::string>(this->run(binNode->leftNode));
                        
                        if (this->run(binNode->leftNode).type() == typeid(int))
                            leftExpression = std::to_string(std::any_cast<int>(this->run(binNode->leftNode)));
                        
                        if (this->run(binNode->leftNode).type() == typeid(float))
                            leftExpression = std::to_string(std::any_cast<float>(this->run(binNode->leftNode)));

                        if (this->run(binNode->rightNode).type() == typeid(std::string))
                            rightExpression = std::any_cast<std::string>(this->run(binNode->rightNode));
                        
                        if (this->run(binNode->rightNode).type() == typeid(int))
                            rightExpression = std::to_string(std::any_cast<int>(this->run(binNode->rightNode)));
                      
                        if (this->run(binNode->rightNode).type() == typeid(float))
                            rightExpression = std::to_string(std::any_cast<float>(this->run(binNode->rightNode)));

                        // If both expressions are not empty, then the multiplication is between two variables
                        if (!leftExpression.empty() && !rightExpression.empty()) {
                            this->current_func_def = leftExpression + binNode->op->getValue() + rightExpression;
                            return "(" + leftExpression + binNode->op->getValue() + rightExpression + ")";
                        }
                    }

                    if ((this->run(binNode->leftNode).type() == typeid(float)) && (this->run(binNode->rightNode).type() == typeid(float))){
                        return std::any_cast<float>(this->run(binNode->leftNode)) / std::any_cast<float>(this->run(binNode->rightNode));
                    }
                    
                    if ((this->run(binNode->leftNode).type() == typeid(int)) && (this->run(binNode->rightNode).type() == typeid(int))){
                        return std::any_cast<int>(this->run(binNode->leftNode)) / std::any_cast<int>(this->run(binNode->rightNode));
                    }    

                    if ((this->run(binNode->leftNode).type() == typeid(float)) && (this->run(binNode->rightNode).type() == typeid(int))){
                        float leftValue = std::any_cast<float>(this->run(binNode->leftNode));
                        int rightValue = std::any_cast<int>(this->run(binNode->rightNode));
                        return leftValue / static_cast<float>(rightValue);
                    }
                    
                    if ((this->run(binNode->leftNode).type() == typeid(int)) && (this->run(binNode->rightNode).type() == typeid(float))){
                        int leftValue = std::any_cast<int>(this->run(binNode->leftNode));
                        float rightValue = std::any_cast<float>(this->run(binNode->rightNode));
                        return static_cast<float>(leftValue) / rightValue;
                    }          
                    break;
                case Token::TokenType::ASSIGN: {
                    
                    auto result = this->run(binNode->rightNode);
                    auto variable = dynamic_cast<VariableNode*>(binNode->leftNode);

                    std::string id = variable->id->getValue();
                    std::string str = id.substr(id.size() - 3, id.size());

                    if (str[0] == '(' && str[2] == ')') {
                        if (str[1] == 'f') {
                            try {

                                if (result.type() == typeid(int)){
                                    int floatResult = std::any_cast<int>(result);
                                    id.erase(id.size() - 3, 3); // Corrected to erase the last 3 characters
                                    this->stack_variable[id] = floatResult;                                   
                                }
                                else if (result.type() == typeid(float)){
                                    float floatResult = std::any_cast<float>(result);
                                    id.erase(id.size() - 3, 3); // Corrected to erase the last 3 characters
                                    this->stack_variable[id] = floatResult;
                                }else{
                                    throw std::runtime_error("Type mismatch: expected float and int but got a different type.");
                                }


                            } catch (const std::bad_any_cast& e) {
                                throw std::runtime_error("Type mismatch: expected float and int but got a different type.");
                            }
                        } else if (str[1] == 'i') {
                            try {


                                if (result.type() == typeid(int)){
                                    id.erase(id.size() - 3, 3); // Corrected to erase the last 3 characters
                                    this->stack_variable[id] = std::any_cast<int>(result);                              
                                }
                                else if (result.type() == typeid(float)){
                                    id.erase(id.size() - 3, 3); // Corrected to erase the last 3 characters
                                    this->stack_variable[id] = std::any_cast<float>(result);
                                }else{
                                    throw std::runtime_error("Type mismatch: expected float and int but got a different type.");
                                }


                            } catch (const std::bad_any_cast& e) {
                                throw std::runtime_error("Type mismatch: expected int but got a different type.");
                            }
                        } 
                    } else {
                        this->stack_variable[id] = result;
                    }
                    return result;
                }
                case Token::TokenType::COLON: {
                        
                    auto variable = dynamic_cast<VariableNode*>(binNode->leftNode);
                    this->isFunc = true;
                    stack_function[variable->id->getValue()] = "";
                    this->current_func = variable->id->getValue();
                    auto result = this->run(binNode->rightNode);

                    return {};
                }
                default:
                    throw std::runtime_error("Unsupported binary operator");
            }

        } else if (auto varNode = dynamic_cast<VariableNode*>(node)) {

            std::string varNodeValue = varNode->id->getValue();
            auto it = this->stack_variable.find(varNodeValue);

            if (it != this->stack_variable.end()) {
                return it->second;
            } else if( toUpper(varNodeValue).find("LOAD") != std::string::npos){
                auto args_load = split(varNodeValue.substr(toUpper(varNodeValue).find("LOAD")+5), ", ");

                std::ifstream array_in("../../" + args_load.at(1)); 


                if (!array_in.is_open()) {
                    throw std::ios_base::failure("Error: failed to open file");
                }

                int number;
                std::vector<int> numbers;

                while (array_in >> number) {
                    numbers.push_back(number);
                }

                array_in.close();
                
                this->array[args_load.at(0)] = numbers;

                return {};
            }
            else if( toUpper(varNodeValue).find("SAVE") != std::string::npos){
                auto args_load = split(varNodeValue.substr(toUpper(varNodeValue).find("SAVE")+5), ", ");
                
                std::ofstream array_out("../../" + args_load.at(1));
                
                if (!array_out.is_open()) {
                    throw std::ios_base::failure("Error: failed to open file");
                }

                if (this->array.find(args_load.at(0)) == this->array.end()){
                    throw std::runtime_error("Error: array \"" + args_load.at(0) + "\" is not declared");
                }

                // Записываем числа в файл
                for (size_t i = 0; i < this->array[args_load.at(0)].size(); ++i) {
                    array_out << this->array[args_load.at(0)][i];
                    if (i != this->array[args_load.at(0)].size() - 1) {
                        array_out << " "; // Добавляем пробел между числами, кроме последнего
                    } else {
                        array_out << std::endl; // Добавляем новую строку после последнего числа
                    }
                }

                array_out.close(); // Закрываем файл    
                return {};
            }
            else if( toUpper(varNodeValue).find("RAND") != std::string::npos){

                auto args_load = split(varNodeValue.substr(toUpper(varNodeValue).find("RAND")+5), ", ");
                
                if (this->array.find(args_load.at(0)) == this->array.end()){
                    throw std::runtime_error("Error: array \"" + args_load.at(0) + "\" is not declared");
                }
            
                this->array[args_load.at(0)].clear();
                
                int count;
                if (this->stack_variable.find(args_load.at(1)) != this->stack_variable.end()){
                    count = std::any_cast<int>(this->stack_variable[args_load.at(1)]);
                }else{
                    try{
                        count = std::stoi(args_load.at(1));
                    }catch(...){
                        throw std::runtime_error("variable " + args_load.at(1) + " is not declared or is not of type number");
                    }
                    
                }

                int lb;
                if (this->stack_variable.find(args_load.at(2)) != this->stack_variable.end()){
                    lb = std::any_cast<int>(this->stack_variable[args_load.at(2)]);
                }else{
                    try{
                        lb = std::stoi(args_load.at(2));
                    }catch(...){
                        throw std::runtime_error("variable " + args_load.at(2) + " is not declared or is not of type number");
                    }
                    
                }

                int rb;
                if (this->stack_variable.find(args_load.at(3)) != this->stack_variable.end()){
                    rb = std::any_cast<int>(this->stack_variable[args_load.at(3)]);
                }else{
                    try{
                        rb = std::stoi(args_load.at(3));
                    }catch(...){
                        throw std::runtime_error("variable " + args_load.at(3) + " is not declared or is not of type number");
                    }
                    
                }


                for (int i = 0; i < count; i++) {
                    this->array[args_load.at(0)].push_back(rand() % (rb - lb + 1) + lb); // Генерируем случайное число из заданного отрезка
                }

                return {};
            }
            else if( toUpper(varNodeValue).find("CONCAT") != std::string::npos){

                auto args_load = split(varNodeValue.substr(toUpper(varNodeValue).find("CONCAT")+7), ", ");
                
                if (this->array.find(args_load.at(0)) == this->array.end()){
                    throw std::runtime_error("Error: array \"" + args_load.at(0) + "\" is not declared");
                }           

                if (this->array.find(args_load.at(1)) == this->array.end()){
                    throw std::runtime_error("Error: array \"" + args_load.at(1) + "\" is not declared");
                }


                for (int i = 0; i < this->array[args_load.at(1)].size(); i++){
                    this->array[args_load.at(0)].push_back(this->array[args_load.at(1)][i]);
                }


                return {};
            }    
            else if( toUpper(varNodeValue).find("FREE") != std::string::npos){

                auto arg = varNodeValue.substr(varNodeValue.find('(')+1, varNodeValue.find(')') - varNodeValue.find('(') - 1 );   
                
                if (this->array.find(arg) != this->array.end()){
                    this->array[arg].clear();
                }else{
                    throw std::runtime_error("Error: array \"" + arg + "\" is not declared");
                }        

                return {};
            }    
            else if( toUpper(varNodeValue).find("REMOVE") != std::string::npos){
                
                auto args_load = split(varNodeValue.substr(toUpper(varNodeValue).find("REMOVE")+7), ", ");
                
                if (this->array.find(args_load.at(0)) == this->array.end()){
                    throw std::runtime_error("Error: array \"" + args_load.at(0) + "\" is not declared");
                }     

                int start_index;
                if (this->stack_variable.find(args_load.at(1)) != this->stack_variable.end()){
                    start_index = std::any_cast<int>(this->stack_variable[args_load.at(1)]);
                }else{
                    try{
                        start_index = std::stoi(args_load.at(1));
                    }catch(...){
                        throw std::runtime_error("variable " + args_load.at(1) + " is not declared or is not of type number");
                    }
                    
                }

                int count;
                if (this->stack_variable.find(args_load.at(2)) != this->stack_variable.end()){
                    count = std::any_cast<int>(this->stack_variable[args_load.at(2)]);
                }else{
                    try{
                        count = std::stoi(args_load.at(2));
                    }catch(...){
                        throw std::runtime_error("variable " + args_load.at(2) + " is not declared or is not of type number");
                    }
                    
                }

                if (start_index < 0 || start_index >= this->array[args_load.at(0)].size() ||
                    start_index + count > this->array[args_load.at(0)].size()) {
                    throw std::out_of_range("Error: specified range is out of vector bounds.");
                }

                this->array[args_load.at(0)].erase(
                    this->array[args_load.at(0)].begin() + start_index - 1,
                    this->array[args_load.at(0)].begin() + start_index + count -1
                );          

                return {};
            }    
            else if( toUpper(varNodeValue).find("COPY") != std::string::npos){
                
                auto args_load = split(varNodeValue.substr(toUpper(varNodeValue).find("COPY")+5), ", ");
                
                if (this->array.find(args_load.at(0)) == this->array.end()){
                    throw std::runtime_error("Error: array \"" + args_load.at(0) + "\" is not declared");
                }  

                if (this->array.find(args_load.at(3)) == this->array.end()){
                    throw std::runtime_error("Error: array \"" + args_load.at(3) + "\" is not declared");
                }   

                int lb;
                if (this->stack_variable.find(args_load.at(1)) != this->stack_variable.end()){
                    lb = std::any_cast<int>(this->stack_variable[args_load.at(1)]);
                }else{
                    try{
                        lb = std::stoi(args_load.at(1));
                    }catch(...){
                        throw std::runtime_error("variable " + args_load.at(1) + " is not declared or is not of type number");
                    }
                    
                }

                int rb;
                if (this->stack_variable.find(args_load.at(2)) != this->stack_variable.end()){
                    rb = std::any_cast<int>(this->stack_variable[args_load.at(2)]);
                }else{
                    try{
                        rb = std::stoi(args_load.at(2));
                    }catch(...){
                        throw std::runtime_error("variable " + args_load.at(2) + " is not declared or is not of type number");
                    }
                    
                }       

                this->array[args_load.at(3)].clear();

                for (int i = lb; i < rb; i++){
                    this->array[args_load.at(3)].push_back(this->array[args_load.at(0)][i]);
                }

                return {};
            }   
            else if( toUpper(varNodeValue).find("SORT") != std::string::npos){
                
                auto arg = varNodeValue.substr(5);   
                auto sign = arg.back();  
                arg.pop_back();

                if (this->array.find(arg) == this->array.end()){
                    throw std::runtime_error("Error: array \"" + arg + "\" is not declared");
                }  

                // Сортируем массив в зависимости от знака
                if (sign == '+') {
                    std::sort(this->array[arg].begin(), this->array[arg].end());
                } else if (sign == '-') {
                    std::sort(this->array[arg].begin(), this->array[arg].end(), std::greater<int>());
                } else {
                    throw std::runtime_error("Error: unknown sorting order \"" + std::string(1, sign) + "\"");
                }

                return {};
            }   
            else if( toUpper(varNodeValue).find("PERMUTE") != std::string::npos){
                
                auto arg = varNodeValue.substr(8);   
                
                if (this->array.find(arg) == this->array.end()){
                    throw std::runtime_error("Error: array \"" + arg + "\" is not declared");
                }  


                for (int i = 0; i < this->array[arg].size(); i++) {
                    std::sort(this->array[arg].begin(), this->array[arg].end());

                    auto rnd = (rand() % this->array[arg].size());

                    if (rnd != 0) {
                        this->array[arg][rnd] ^= this->array[arg][0];
                        this->array[arg][0] ^= this->array[arg][rnd];
                        this->array[arg][rnd] ^= this->array[arg][0];
                    }
                }       
                
                return {};
            }   
            else if( toUpper(varNodeValue).find("XOR") != std::string::npos){
                auto args_load = split(varNodeValue.substr(toUpper(varNodeValue).find("XOR")+4, varNodeValue.size()), ", ");

                if (this->array.find(args_load.at(0)) == this->array.end()){
                    throw std::runtime_error("Error: array \"" + args_load.at(0) + "\" is not declared");
                }  

                if (this->array.find(args_load.at(1)) == this->array.end()){
                    throw std::runtime_error("Error: array \"" + args_load.at(1) + "\" is not declared");
                }   

                std::vector<int> xor_array;

                // Проходим по элементам массива A
                for (auto& element : this->array[args_load.at(0)]) {
                    // Проверяем, есть ли текущий элемент массива A в массиве B
                    auto it = std::find(this->array[args_load.at(1)].begin(), this->array[args_load.at(1)].end(), element);
                    // Если элемента нет в массиве B, добавляем его в результат
                    if (it == this->array[args_load.at(1)].end()) {
                        xor_array.push_back(element);
                    }
                }


                // Проходим по элементам массива B
                for (auto& element : this->array[args_load.at(1)]) {
                    // Проверяем, есть ли текущий элемент массива B в массиве A
                    auto it = std::find(this->array[args_load.at(0)].begin(), this->array[args_load.at(0)].end(), element);
                    // Если элемента нет в массиве A, добавляем его в результат
                    if (it == this->array[args_load.at(0)].end()) {
                        xor_array.push_back(element);
                    }
                }

                // Обновляем массив A результатом симметрической разности
                this->array[args_load.at(0)] = xor_array;

                return {};
                
            }
            else if( toUpper(varNodeValue).find("INTERSECT") != std::string::npos){
                auto args_load = split(varNodeValue.substr(toUpper(varNodeValue).find("INTERSECT")+10), ", ");

                if (this->array.find(args_load.at(0)) == this->array.end()){
                    throw std::runtime_error("Error: array \"" + args_load.at(0) + "\" is not declared");
                }  

                if (this->array.find(args_load.at(1)) == this->array.end()){
                    throw std::runtime_error("Error: array \"" + args_load.at(1) + "\" is not declared");
                }   

                std::vector<int> intersect_array;

                // Проходим по элементам массива A
                for (auto& element : this->array[args_load.at(0)]) {
                    // Проверяем, есть ли текущий элемент массива A в массиве B
                    auto it = std::find(this->array[args_load.at(1)].begin(), this->array[args_load.at(1)].end(), element);
                    // Если элемента нет в массиве B, добавляем его в результат
                    if (it != this->array[args_load.at(1)].end()) {
                        intersect_array.push_back(element);
                    }
                }


                // Проходим по элементам массива B
                for (auto& element : this->array[args_load.at(1)]) {
                    // Проверяем, есть ли текущий элемент массива B в массиве A
                    auto it = std::find(this->array[args_load.at(0)].begin(), this->array[args_load.at(0)].end(), element);
                    // Если элемента нет в массиве A, добавляем его в результат
                    if (it != this->array[args_load.at(0)].end()) {
                        intersect_array.push_back(element);
                    }
                }

                // Обновляем массив A результатом симметрической разности
                this->array[args_load.at(0)] = intersect_array;

                return {};
                
            }
            else if (toUpper(varNodeValue).find("PRINT") != std::string::npos) {
                auto args_load = split(varNodeValue.substr(toUpper(varNodeValue).find("PRINT")+6), ", ");
                if (this->array.find(args_load.at(0)) == this->array.end()){
                    throw std::runtime_error("Error: array \"" + args_load.at(0) + "\" is not declared");
                }  

                if (!args_load.at(1).compare("all")){
                    for (int i = 0; i < this->array[args_load.at(0)].size(); i++){
                        std::cout << this->array[args_load.at(0)][i] << " ";
                    }
                    std::cout << std::endl;
                }else{
                
                    int lb;
                    if (this->stack_variable.find(args_load.at(1)) != this->stack_variable.end()){
                        lb = std::any_cast<int>(this->stack_variable[args_load.at(1)]);
                    }else{
                        try{
                            lb = std::stoi(args_load.at(1));
                        }catch(...){
                            throw std::runtime_error("variable " + args_load.at(1) + " is not declared or is not of type number");
                        }
                        
                    }

                    int rb;
                    if (this->stack_variable.find(args_load.at(2)) != this->stack_variable.end()){
                        rb = std::any_cast<int>(this->stack_variable[args_load.at(2)]);
                    }else{
                        try{
                            rb = std::stoi(args_load.at(2));
                        }catch(...){
                            throw std::runtime_error("variable " + args_load.at(2) + " is not declared or is not of type number");
                        }
                        
                    }     

                    for (int i = lb-1; i < rb-1; i++){
                        std::cout << this->array[args_load.at(0)][i] << " ";
                    }
                    std::cout << std::endl;                    
                }
                return {};
            }
            else if (toUpper(varNodeValue).find("STAT") != std::string::npos) {

                auto arg = varNodeValue.substr(5);   

                if (this->array.find(arg) == this->array.end()) {
                    throw std::runtime_error("Error: array \"" + arg + "\" is not declared");
                }  

                auto max_it = std::max_element(this->array[arg].begin(), this->array[arg].end());
                auto max_element = *max_it;
                auto max_index = std::distance(this->array[arg].begin(), max_it);

                auto min_it = std::min_element(this->array[arg].begin(), this->array[arg].end());
                auto min_element = *min_it;
                auto min_index = std::distance(this->array[arg].begin(), min_it);

                std::unordered_map<int, size_t> frequency_map;
                for (int num : this->array[arg]) {
                    frequency_map[num]++;
                }
                auto most_frequent_it = std::max_element(frequency_map.begin(), frequency_map.end(), 
                    [](const auto& a, const auto& b) { return a.second < b.second; });
                auto most_frequent = most_frequent_it->first;

                double sum = 0;
                for (int num : this->array[arg]) {
                    sum += num;
                }

                auto average = sum / this->array[arg].size();

                double max_deviation = 0;
                for (int num : this->array[arg]) {
                    double deviation = std::abs(num - average);
                    if (deviation > max_deviation) {
                        max_deviation = deviation;
                    }
                }

                std::cout << "Data \"" + arg + "\" array:" << std::endl;
                std::cout << "1) Array size: " << this->array[arg].size() << std::endl;
                std::cout << "2) Max element: " << max_element << " (" << max_index << ")" << std::endl;
                std::cout << "3) Min element: " << min_element << " (" << min_index << ")" << std::endl;
                std::cout << "4) Most frequent element: " << most_frequent << std::endl;
                std::cout << "5) Average value: " << average << std::endl;
                std::cout << "6) Max deviation: " << max_deviation << std::endl;

                return {};
            }
            else if(this->isFunc){
                return varNodeValue;
            } else if(!varNodeValue.compare("\n")){
                return varNodeValue;
            }
            else{
                
                std::string id_func = varNodeValue.substr(0, varNodeValue.find('('));
                auto arg_value_lst = split(varNodeValue.substr(varNode->id->getValue().find('(') + 1, varNodeValue.find(')') - varNodeValue.find('(') - 1), " ,");
                
                auto arg_value_lst2 = split(varNodeValue.substr(varNode->id->getValue().find('(') + 1, varNodeValue.find(')') - varNodeValue.find('(') - 1), "*/-+ ", true);
                for (auto& pair : this->stack_function) {

                    if (pair.first.find(id_func) != std::string::npos) {
    
                        auto arg_lst = split(pair.first.substr(pair.first.find('(') + 1, pair.first.find(')') - pair.first.find('(') - 1), ", ");
                        
                        for (auto& arg_value : arg_value_lst) {

                            auto arg_value_sub = split(arg_value, "*/-+ ", true);                           
                            for (auto& subarg_value : arg_value_sub) {

                                auto found = this->stack_variable.find(subarg_value);
                                if (found != this->stack_variable.end()) {
                                    if (found->second.type() == typeid(float)){
                                        subarg_value = std::to_string(std::any_cast<float>(found->second));                  
                                    }else{
                                        subarg_value = std::to_string(std::any_cast<int>(found->second));
                                    }

                                }
                            }  
                            RecursiveDescentParser eval(arg_value_sub);
                            float result = eval.parse();
                            arg_value = std::to_string(result);
                        }  



              
                        //RecursiveDescentParser eval1(arg_value_lst2);
                        //float result1 = eval1.parse();
                        //std::cout << result1;

                        if (arg_lst.capacity() != arg_value_lst.capacity()){
                            throw std::runtime_error("Invalid count parameter function");
                        }



                        for (auto& arg_value : arg_value_lst) {
                            auto found = this->stack_variable.find(arg_value);
                            if (found != this->stack_variable.end()) {
                                if (found->second.type() == typeid(float)){
                                    arg_value = std::to_string(std::any_cast<float>(found->second));                  
                                }else{
                                    arg_value = std::to_string(std::any_cast<int>(found->second));
                                }

                            }
                        }




                        for (int i = 0; i < arg_lst.size(); i++){
                            this->stack_function_variable[arg_lst[i]] = arg_value_lst[i];
                        }
                        auto func_def_parsed = split(pair.second, ")*+-/( ", true);

                        for (auto &it : func_def_parsed) {
                            auto found = stack_function_variable.find(it);
                            if (found != stack_function_variable.end()) {
                                it = found->second; 
                            }
                        }   

                        this->stack_function_variable.clear();

                        RecursiveDescentParser eval(func_def_parsed);

                        float result = eval.parse();

                        return result;

                    }
                }


                throw std::runtime_error("Variable " + varNode->id->getValue() + " not found");
            }

        } else if (auto stmtNode = dynamic_cast<StatementNode*>(node)) {
            for (auto& codeString : stmtNode->codeStrings) {
                stack_function[this->current_func] = this->current_func_def;
                this->current_func_def = "";
                this->current_func = "";
                this->isFunc = false;
                this->run(codeString);
            }
            return {};
        }

        throw std::runtime_error("Unknown node type");
    }
};

#endif // PARSER_H





/*#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include "../../lexer/include/lexer.h"
#include "../../lexer/include/lexer_builder.h"

class parser{

public:

            else {
                //std::string id_func = varNode->id->getValue().substr(0, varNode->id->getValue().find('('));
                //std::cout << varNode->id->getValue().substr(0, varNode->id->getValue().find('(')) << std::endl;
                //for (auto& pair : this->stack_function) {
                    //if (pair.first.find(id_func) != std::string::npos) {

   

/*
                        if (!this->lock){
                            std::cout << varNode->id->getValue() << std::endl;


                            auto arg_lst = split(pair.first.substr(pair.first.find('(') + 1, pair.first.find(')') - pair.first.find('(') - 1), ",");
                            auto arg_value_lst = split(varNode->id->getValue().substr(varNode->id->getValue().find('(') + 1, varNode->id->getValue().find(')') - varNode->id->getValue().find('(') - 1), ",");
                            
                            for (int i = 0; i < arg_lst.size(); i++){
                                float value = std::stof(arg_value_lst[i]);
                                this->stack_fuction_variable[arg_lst[i]] = value;
                            }

                            this->lock = true;
                        }

                        if (this->lock){
                            for (auto &it: this->stack_fuction_variable){
                                if (it.first == varNode->id->getValue()){
                                    return it.second;
                                }
                            }
                        }

                        if (this->run(pair.second).type() == typeid(float)){
                            float result = std::any_cast<float>(this->run(pair.second)); 
                            return static_cast<float>(result);
                        }
                        if (this->run(pair.second).type() == typeid(int)){
                            int result = std::any_cast<int>(this->run(pair.second));  
                            return static_cast<int>(result);                  
                        }




                    //}
                }
struct ASTNode {
    lexer::TokenType type;
    std::string value;
    ASTNode** children;
    size_t size;
    size_t capacity;
    ASTNode* parent; // Добавим указатель на родительский узел

    ASTNode(lexer::TokenType type, std::string const& val, size_t initial_capacity = 4, ASTNode* parent = nullptr)
        : type(type), value(val), size(0), capacity(initial_capacity), parent(parent) {
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
#endif*/