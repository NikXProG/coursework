#ifndef NUMBER_NODE_H
#define NUMBER_NODE_H

#include "expression_node.h"
#include "../../../lexer/include/token.h"

class NumberNode : public ExpressionNode {



public:
    Token* number;
    NumberNode(Token* number) : ExpressionNode() {
        this->number = number;
    }
};

#endif // NUMBER_NODE_H