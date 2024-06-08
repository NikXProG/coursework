#ifndef VARIABLE_NODE_H
#define VARIABLE_NODE_H

#include "expression_node.h"
#include "../../../lexer/include/token.h"

class VariableNode : public ExpressionNode {

public:
    Token* id;
    VariableNode(Token* id) : ExpressionNode(), id(id) {}

};

#endif // VARIABLE_NODE_H