#pragma once
#include <memory>
#include <string>
#include <vector>
#include "../lexer/Token.h"
#include "TreeVisitor.h"
#include "Node.h"
#include "../Parser/ParserToken.h"

namespace Nodes {
	class Expression;
	class ParametersList;
	class ClassBlock;
    class ClassDefinition;
    class StatementBlock;
    class Variable;
    class TreeVisitor;
    class Object;
    class Arguments;
    class RetrunObject;
    class Identifier : public Node
    {
    public:
        Identifier(std::vector<std::string> identidiers, Lexer::Token token);
        ParserToken token;
        std::vector<std::string> identifiers;
        virtual Nodes::Object* visit(TreeVisitor* visitor);
    };

    class Instruction : public Node
    {
    public:
        virtual Nodes::Object* visit(TreeVisitor* visitor);
    };

    class Condition : public Node
    {
    public:
        virtual Nodes::Object* visit(TreeVisitor* visitor);
       
        std::unique_ptr<Expression> expression;
        Condition(std::unique_ptr<Expression> expression);
    };

    class Expression : public Node
    {
    public:
        virtual Nodes::Object* visit(TreeVisitor* visitor){
            return nullptr;
        }
    };

    class FunDefinition : public Node
    {
    public:
        ParserToken token;
        FunDefinition(std::string name, std::unique_ptr<ParametersList> parameters, std::unique_ptr<StatementBlock> blockNode);
        virtual Nodes::Object* visit(TreeVisitor* visitor);
        virtual Nodes::Object* visit(TreeVisitor* visitor, std::unordered_map<std::string, Nodes::Object*> variables, std::string variableName);
        std::string name;
        std::unique_ptr<ParametersList> parameters;
        std::unique_ptr<StatementBlock> blockNode;
    };

    class ParametersList : public Node
    {
    public:
        virtual Nodes::Object* visit(TreeVisitor* visitor);
        ParametersList(std::vector<std::unique_ptr<Variable>> parameters);

        ParametersList() = default;
    
        std::vector<std::unique_ptr<Variable>> parameters;
    };

    class Arguments: public Node{
    public:
        virtual Nodes::Object* visit(TreeVisitor* visitor);
        Arguments(std::vector<std::unique_ptr<Expression>> arguments);
        Arguments() = default;
        std::vector<std::unique_ptr<Expression>> expressions;
    };

    class IfStatement : public Instruction
    {
    public:
        virtual Nodes::Object* visit(TreeVisitor* visitor);
        IfStatement(std::unique_ptr<Condition> conditionNode, std::unique_ptr<StatementBlock> trueBlockNode, std::unique_ptr<StatementBlock> elseBlock, std::vector<std::unique_ptr<IfStatement>> elifs);
        IfStatement(std::unique_ptr<Condition> conditionNode, std::unique_ptr<StatementBlock> trueBlockNode);

        std::unique_ptr<Condition> conditionNode;
        std::unique_ptr<StatementBlock> blockNode;
        std::vector<std::unique_ptr<IfStatement>> elifBlocksNodes;
        std::unique_ptr<StatementBlock> elseBlockNode;
    };

    class Program : public Node
    {
    public:
        Program(std::vector<std::unique_ptr<ClassDefinition>> classes, std::unique_ptr<FunDefinition> main);

        virtual Nodes::Object* visit(TreeVisitor* visitor);

        std::unique_ptr<FunDefinition> main;
        std::vector<std::unique_ptr<ClassDefinition>> classes;
    };

    class ReturnStatement : public Instruction
    {
    public:
        virtual Nodes::Object* visit(TreeVisitor* visitor);

        ReturnStatement(std::unique_ptr<Expression> expression);
        std::unique_ptr<Expression> expression;
    };

    class BreakStatement: public Instruction
    {
        virtual Nodes::Object* visit(TreeVisitor* visitor);
    };

    class ContinueStatement: public Instruction{
        virtual Nodes::Object* visit(TreeVisitor* visitor);
    };

    class StatementBlock : public Node
    {
    public:
        virtual Nodes::Object* visit(TreeVisitor* visitor);
        StatementBlock(std::vector<std::unique_ptr<Instruction>> instructions);
        std::vector<std::unique_ptr<Instruction>> statements;
    };

    class VarAssigment : public Node
    {
    public:
        virtual Nodes::Object* visit(TreeVisitor* visitor);
        VarAssigment(std::unique_ptr<Identifier> identifier, std::unique_ptr<Expression> expression);
        std::unique_ptr<Identifier> identifier;
        std::unique_ptr<Expression> expression;
    };

    class Variable : public Node
    {
    public:

        virtual Nodes::Object* visit(TreeVisitor* visitor);
        Variable(std::unique_ptr<Identifier> identifier);

        std::unique_ptr<Identifier> identifier;
    };

    class WhileStatement : public Instruction
    {
    public:
        virtual Nodes::Object* visit(TreeVisitor* visitor);

        WhileStatement(std::unique_ptr<Condition> condition, std::unique_ptr<StatementBlock> statementBlock);

        std::unique_ptr<Condition> conditionNode;
        std::unique_ptr<StatementBlock> blockNode;
    };

    class Object : public Node
    {
    public:
        bool isTrue;
        std::string className;
        std::string stringLiteral = "";
        std::unordered_map<std::string, Nodes::Object* > variables;
        int value;
        virtual Nodes::Object* visit(TreeVisitor* visitor);
        Object()=default;
        Object(int value): value(value){}
        Object(bool isTrue): isTrue(isTrue){}
        Object(std::string className, std::unordered_map<std::string, Nodes::Object*> variables):
        className(className), variables(variables) {}
        Object(std::string stringLiteral): stringLiteral(stringLiteral){}
        Object(bool isTrue, int value, std::string stringLiteral, std::string className, std::unordered_map<std::string, Nodes::Object*> variables):
                className(className), variables(variables), isTrue(isTrue), value(value), stringLiteral(stringLiteral) {}
        bool virtual checkIfIsReturn(){ return false;}
        bool virtual checkIfIsBreak(){return false;}
        bool virtual checkIfIsContinue(){return false;}
        virtual Object* ToObject()
        {
            return this;
        }
    };

    class RetrunObject: public Object
    {
    public:
        bool checkIfIsReturn() override {return true;}
        RetrunObject(Object* literal)
        {
            isTrue = literal->isTrue;
            className = literal->className;
            value = literal->value;
            stringLiteral = literal->stringLiteral;
            variables = literal->variables;
        }
        Object * ToObject() override{
            return new Object(isTrue, value, stringLiteral, className, variables);
        }
    };

    class LoopJumpObject: public Object
    {
    public:
        virtual bool checkIfIsBreak() override {return false;}
        virtual bool checkIfIsContinue() override {return false;}
        virtual Object * ToObject() override{
            return nullptr;
        }
        LoopJumpObject() = default;
    };

    class BreakStatementLiteral: public LoopJumpObject
    {
        bool checkIfIsBreak() override{return true;}
    };

    class ContinueStatementLiteral: public LoopJumpObject
    {
        bool checkIfIsContinue() override{return true;}
    };

    class StringLiteral : public Object
    {
        virtual Nodes::Object* visit(TreeVisitor* visitor);
        std::string value;
    };

    class IntegerLiteral : public Object
    {
    public:
        virtual Nodes::Object* visit(TreeVisitor* visitor);
        int value = 0;
        IntegerLiteral(int value): Object(value){ this->value = value;}
    };

    class ClassDefinition: public Node
    {
    public:
        virtual Nodes::Object* visit(TreeVisitor* visitor);
        ClassDefinition(std::vector<std::unique_ptr<FunDefinition>> functions, std::vector<std::unique_ptr<VarAssigment>> variables, std::string identifier);

        std::string identifier;
        std::vector<std::unique_ptr<FunDefinition>> funcDefinitions;
        std::vector<std::unique_ptr<VarAssigment>> variables;
    };

    class ClassBlock: public Node
    {
    public:
        virtual Nodes::Object visit(TreeVisitor* visitor);
        
        ClassBlock(std::vector<std::unique_ptr<Node>> variables, std::vector<std::unique_ptr<Node>> functions);

        std::vector<std::unique_ptr<Node>> variables;
        std::vector<std::unique_ptr<Node>> functions;
    };

    class OrExpression: public Expression
    {
    public:
        OrExpression(std::unique_ptr<Expression>& left, std::unique_ptr<Expression>& right);
        virtual Nodes::Object* visit(TreeVisitor* visitor);
        std::unique_ptr<Expression> left;
        std::unique_ptr<Expression> right;
    };

    class AndExpression : public Expression
    {
    public:
        AndExpression(std::unique_ptr<Expression>& left, std::unique_ptr<Expression>& right);
        virtual Nodes::Object* visit(TreeVisitor* visitor);
        std::unique_ptr<Expression> left;
        std::unique_ptr<Expression> right;
    };

    class LogicNegationExpression: public Expression
    {
    public:
        virtual Nodes::Object* visit(TreeVisitor* visitor);
        LogicNegationExpression(std::unique_ptr<Expression> left);
        std::unique_ptr<Expression> left;
    };

    class ComparisonExpression: public Expression
    {
    public:
        virtual Nodes::Object* visit(TreeVisitor* visitor);
        ComparisonExpression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right, Lexer::Token token);
        std::unique_ptr<Expression> left;
        std::unique_ptr<Expression> right;
        ParserToken token;
    };

    class AddSubExpression: public Expression
    {
    public:
        virtual Nodes::Object* visit(TreeVisitor* visitor);
        AddSubExpression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right, Lexer::Token token);
        std::unique_ptr<Expression> left;
        std::unique_ptr<Expression> right;
        ParserToken token;
    };

    class MulDivExpression : public Expression
    {
    public:
        virtual Nodes::Object* visit(TreeVisitor* visitor);
        MulDivExpression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right, Lexer::Token token);
        std::unique_ptr<Expression> left;
        std::unique_ptr<Expression> right;
        ParserToken token;
    };

    class UnaryExpression: public Expression
    {
    public:
        virtual Nodes::Object* visit(TreeVisitor* visitor);
        UnaryExpression(std::unique_ptr<Expression> left);
        std::unique_ptr<Expression> left;
    };
    class VarAssignmentOrFuncCall: public Expression, Instruction
    {
    public:
        VarAssignmentOrFuncCall()=default;
        virtual Nodes::Object* visit(TreeVisitor* visitor, Identifier* identifier);
        VarAssignmentOrFuncCall(std::unique_ptr<Expression> expression, Lexer::Token token);
        VarAssignmentOrFuncCall(std::unique_ptr<Arguments> arguments);
    };

    class IdentifierAssigmentOrCall: public Instruction, public Expression
    {
    public:
        std::unique_ptr<Identifier> identifier;
        IdentifierAssigmentOrCall()=default;
        std::unique_ptr<VarAssignmentOrFuncCall> varAssignmentOrFuncCall;
        IdentifierAssigmentOrCall(std::unique_ptr<Identifier> identifier, std::unique_ptr<VarAssignmentOrFuncCall> varAssignmentOrFunCall);
        virtual Nodes::Object* visit(TreeVisitor* visitor);
    };



    class PrimaryExpression: public Expression
    {
    public:
        ParserToken intOrStringToken;
        std::unique_ptr<Identifier> identifier;
        std::unique_ptr<Expression> expression;
        std::unique_ptr<IdentifierAssigmentOrCall> funcCall;
        ParserToken token;
        PrimaryExpression(Lexer::Token token);
        PrimaryExpression(std::unique_ptr<Nodes::Identifier> identifier, Lexer::Token token);

        PrimaryExpression(std::unique_ptr<Expression> expression);

        PrimaryExpression(std::unique_ptr<IdentifierAssigmentOrCall> funcCall, Lexer::Token token);
        virtual Nodes::Object* visit(TreeVisitor* visitor);
    };

    class Assignable: public Node{
        virtual Nodes::Object* Visit(TreeVisitor( visitor));
    };

    class Call : public VarAssignmentOrFuncCall
    {
    public:
        virtual Nodes::Object* visit(TreeVisitor* visitor, Identifier* identifier);
        Call(std::unique_ptr<Nodes::Arguments> arguments);
        std::unique_ptr<Arguments> arguments;
    };

    class Assignment : public VarAssignmentOrFuncCall
    {
    public:
        virtual Nodes::Object* visit(TreeVisitor* visitor, Identifier* identifier);
        Assignment(std::unique_ptr<Expression> expression);
        std::unique_ptr<Expression> expression;
    };

    class StdLib {
    public:
        virtual Nodes::Object* visit(TreeVisitor *visitor, std::vector<Nodes::Object *> expressions);
    };

    class Print : public StdLib {
    public:
        const std::string name = "print";
        virtual Nodes::Object* visit(TreeVisitor *visitor, std::vector<Nodes::Object *> expressions);
    };

}
