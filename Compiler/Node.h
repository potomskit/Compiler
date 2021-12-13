#pragma once
#include <memory>

class Node
{
protected:
    std::weak_ptr<Node> parent;

public:
    enum class Type {
        Assignment,
        Call,
        Condition,
        Expression,
        FunDefinition,
        IfStatement,
        LoopJump,
        Literal,
        Program,
        ReturnStatement,
        StatementBlock,
        VarDeclaration,
        Variable,
        WhileStatement,
    	ClassDefinition,
        ClassBlock,
        StringLiteral,
    	IntegerLiteral,
    };

    virtual Type getType() = 0;
};

typedef std::shared_ptr<Node> NodePtr;

