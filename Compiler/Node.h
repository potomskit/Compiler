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
    	Identifier,
    };

    virtual Type getType() = 0;
};

typedef std::unique_ptr<Node> NodePtr;

