#ifndef EXPRESSION_NODE_H
#define EXPRESSION_NODE_H

#include <iostream>

class ExpressionNode {
    // Add relevant members and methods here
public:


    ExpressionNode() = default;
    virtual ~ExpressionNode() = default;// Virtual destructor for safe polymorphic deletion
};

#endif // EXPRESSION_NODE_H