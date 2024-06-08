#ifndef PARSER_H
#define PARSER_H
#include <any>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
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
public:

    Parser() noexcept = default;

    Parser(Lexer& lexer) {
        this->tokens = lexer.getTokenList();
        this->tokens_size = lexer.getTokenSize();
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
        
    ExpressionNode* parseExpression() {

        auto id = this->match({Token::TokenType::ID});
        // если не переменная, то смотрим на print
        if (id == nullptr || (!id->getValue().compare("print"))) {
            
            auto printNode = this->parsePrint();
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

    ExpressionNode* parsePrint() {
        
        auto opLog = this->match({Token::TokenType::CONSOLE});

        
        if (opLog != nullptr){


            return new UnarOperationNode(opLog, this->parseRightExpression());

        }

        throw std::runtime_error("invalid operator " + std::to_string(this->pos) );
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

            auto it = this->stack_variable.find(varNode->id->getValue());

            if (it != this->stack_variable.end()) {
                return it->second;
            } else if(this->isFunc){
                return varNode->id->getValue();
            } else if(!varNode->id->getValue().compare("\n")){
                return varNode->id->getValue();
            }
            else{
                
                std::string id_func = varNode->id->getValue().substr(0, varNode->id->getValue().find('('));
                auto arg_value_lst = split(varNode->id->getValue().substr(varNode->id->getValue().find('(') + 1, varNode->id->getValue().find(')') - varNode->id->getValue().find('(') - 1), " ,");
                
                auto arg_value_lst2 = split(varNode->id->getValue().substr(varNode->id->getValue().find('(') + 1, varNode->id->getValue().find(')') - varNode->id->getValue().find('(') - 1), "*/-+ ", true);
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