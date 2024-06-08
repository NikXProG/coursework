#ifndef STATEMENT_NODE_H
#define STATEMENT_NODE_H

#include "expression_node.h"
#include <vector>

class StatementNode : public ExpressionNode {

public:
    std::vector<ExpressionNode*> codeStrings;

public:

    void addNode(ExpressionNode* node) {
        codeStrings.push_back(node);
    }

public:

    virtual ~StatementNode() {
        for (auto node : codeStrings) {
            delete node;
        }
    }

};

#endif // STATEMENT_NODE_H