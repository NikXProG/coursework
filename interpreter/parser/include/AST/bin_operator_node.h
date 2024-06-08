#ifndef BIN_OPERATOR_NODE_H
#define BIN_OPERATOR_NODE_H

#include "expression_node.h"
#include "../../../lexer/include/token.h"

class BinOperatorNode : public ExpressionNode {


public:

    Token* op;
    ExpressionNode* leftNode;
    ExpressionNode* rightNode;

    BinOperatorNode(Token* op, ExpressionNode* leftNode, ExpressionNode* rightNode)
        : op(op), leftNode(leftNode), rightNode(rightNode) {}
};

#endif // BIN_OPERATOR_NODE_H