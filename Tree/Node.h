#ifndef INTERPRETER_NODE_H
#define INTERPRETER_NODE_H
#include <memory>


class Node
{
protected:
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
        Parameters,
        Instruction,
        OrExpression,
        AndExpression,
        LogicNegationExpression,
        ComparisonExpression,
        AddSubExpression,
        MulDivExpression,
        UnaryExpression,
        SimpleExpression,
        VarAssignmentOrFuncCall,
        IdentifierAssigmentOrCall,
    };
//    virtual Nodes::Object visit(Nodes::TreeVisitor* visitor)
//    {
//
//    }
};


#endif //INTERPRETER_NODE_H
