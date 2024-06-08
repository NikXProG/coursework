#ifndef UNAR_OPERATOR_NODE_H
#define UNAR_OPERATOR_NODE_H

#include "expression_node.h"
#include "../../../lexer/include/token.h"

class UnarOperationNode : public ExpressionNode {

public:
    Token* op;
    ExpressionNode* operand;
    UnarOperationNode(Token* op, ExpressionNode* operand)
    : ExpressionNode(), op(op), operand(operand) {}
    virtual ~UnarOperationNode() {
        delete operand;
    }
};

#endif // UNAR_OPERATOR_NODE_H