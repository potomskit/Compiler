#pragma once
#include <memory>
#include <string>
#include <vector>
#include "../lexer/Token.h"
#include "TreeVisitor.h"
#include "Node.h"
#include "Nodes.h"

namespace Nodes {

    Identifier::Identifier(std::vector<std::string> identidiers, Lexer::Token token) : identifiers(identidiers) {
        this->token = ParserToken(token);
    }
    Nodes::Object* Identifier::visit(TreeVisitor * visitor)
    {
        return visitor->visit(this);
    }

    Nodes::Object* Call::visit(TreeVisitor * visitor, Identifier* identifier)
    {
        return visitor->visit(this, identifier);
    }

    Nodes::Object* Assignment::visit(TreeVisitor * visitor, Identifier* identifier)
    {
        return visitor->visit(this, identifier);
    }

    Nodes::Object* Condition::visit(TreeVisitor* visitor)
    {
        return visitor->visit(this);
    }
    Condition::Condition(std::unique_ptr<Nodes::Expression> expression) : expression(std::move(expression)) {}

    FunDefinition::FunDefinition(std::string name, std::unique_ptr<Nodes::ParametersList> parameters, std::unique_ptr<Nodes::StatementBlock> blockNode) :
        name(name), parameters(std::move(parameters)), blockNode(std::move(blockNode)) {}
    Nodes::Object* FunDefinition::visit(TreeVisitor* visitor)
    {
        return visitor->visit(this);
    }
    Nodes::Object* FunDefinition::visit(TreeVisitor* visitor, std::unordered_map<std::string, Object*> variables, std::string variableName)
    {
        return visitor->visit(this, variables, variableName);
    };

    Nodes::Object* ParametersList::visit(TreeVisitor * visitor)
    {
        return visitor->visit(this);
    }
    ParametersList::ParametersList(std::vector<std::unique_ptr<Nodes::Variable>> parameters) : parameters(std::move(parameters)) {}

    Nodes::Object* Arguments::visit(TreeVisitor *visitor) {
        return visitor->visit(this);
    }
    Arguments::Arguments(std::vector<std::unique_ptr<Expression>> arguments):expressions(std::move(arguments)) {}

    Nodes::Object* IfStatement::visit(TreeVisitor* visitor)
    {
        return visitor->visit(this);
    }
    IfStatement::IfStatement(std::unique_ptr<Nodes::Condition> conditionNode, std::unique_ptr<Nodes::StatementBlock> trueBlockNode, std::unique_ptr<Nodes::StatementBlock> elseBlock, std::vector<std::unique_ptr<Nodes::IfStatement>> elifs)
    {
        this->conditionNode = std::move(conditionNode);
        this->blockNode = std::move(trueBlockNode);
        this->elseBlockNode = std::move(elseBlock);
        this->elifBlocksNodes = std::move(elifs);
    }
    IfStatement::IfStatement(std::unique_ptr<Nodes::Condition> conditionNode, std::unique_ptr<Nodes::StatementBlock> trueBlockNode)
    {
        this->conditionNode = std::move(conditionNode);
        this->blockNode = std::move(trueBlockNode);
    }

    Program::Program(std::vector<std::unique_ptr<Nodes::ClassDefinition>> classes, std::unique_ptr<Nodes::FunDefinition> main) : classes(std::move(classes)), main(std::move(main)) {}

    Nodes::Object* Program::visit(TreeVisitor* visitor)
    {
        return visitor->visit(this);
    }

    Nodes::Object* ReturnStatement::visit(TreeVisitor* visitor)
    {
        return visitor->visit(this);
    }
    ReturnStatement::ReturnStatement(std::unique_ptr<Expression> expression) : expression(std::move(expression)) {}

    Nodes::Object* StatementBlock::visit(TreeVisitor* visitor)
    {
        return visitor->visit(this);
    }
    StatementBlock::StatementBlock(std::vector<std::unique_ptr<Instruction>> instructions)
    {
        statements = std::move(instructions);
    }



    Nodes::Object* VarAssigment::visit(TreeVisitor* visitor)
    {
        return visitor->visit(this);
    }
    VarAssigment::VarAssigment(std::unique_ptr<Identifier> identifier, std::unique_ptr<Expression> expression) : identifier(std::move(identifier)), expression(std::move(expression)) {}



    Nodes::Object* Variable::visit(TreeVisitor* visitor)
    {
        visitor->visit(this);
    }
    Variable::Variable(std::unique_ptr<Identifier> identifier) : identifier(std::move(identifier)) {}

    Nodes::Object* WhileStatement::visit(TreeVisitor* visitor)
    {
        return visitor->visit(this);
    }
    WhileStatement::WhileStatement(std::unique_ptr<Condition> condition, std::unique_ptr<StatementBlock> statementBlock) :
        conditionNode(std::move(condition)), blockNode(std::move(statementBlock)) {}

    
    Nodes::Object* Object::visit(TreeVisitor* visitor)
    {
        return visitor->visit(this);
    }

    Nodes::Object* StringLiteral::visit(TreeVisitor* visitor)
    {
        visitor->visit(this);
    }

    Nodes::Object* IntegerLiteral::visit(TreeVisitor * visitor)
    {
        visitor->visit(this);
    }

    Nodes::Object* ClassDefinition::visit(TreeVisitor * visitor)
    {
        visitor->visit(this);
    }
    ClassDefinition::ClassDefinition(std::vector<std::unique_ptr<FunDefinition>> functions, std::vector<std::unique_ptr<VarAssigment>> variables, std::string identifier) :
        identifier(identifier), variables(std::move(variables)), funcDefinitions(std::move(functions)) {}


    Nodes::Object ClassBlock::visit(TreeVisitor * visitor)
	{
	    visitor->visit(this);
	}
	ClassBlock::ClassBlock(std::vector<std::unique_ptr<Node>> variables, std::vector<std::unique_ptr<Node>> functions)
	{
	    this->variables = std::move(variables);
	    this->functions = std::move(functions);
    }

    OrExpression::OrExpression(std::unique_ptr<Expression>& left, std::unique_ptr<Expression>& right)
    {
        this->left = std::move(left);
        this->right = std::move(right);
    }
    Nodes::Object* OrExpression::visit(TreeVisitor* visitor)
    {
        return visitor->visit(this);
    }
   

    AndExpression::AndExpression(std::unique_ptr<Expression>& left, std::unique_ptr<Expression>& right)
    {
        this->left = std::move(left);
        this->right = std::move(right);
    }
    Nodes::Object* AndExpression::visit(TreeVisitor* visitor)
    {
        visitor->visit(this);
    }


    Nodes::Object* LogicNegationExpression::visit(TreeVisitor* visitor)
    {
        visitor->visit(this);
    }
    LogicNegationExpression::LogicNegationExpression(std::unique_ptr<Expression> left) : left(std::move(left)) {}



    Nodes::Object* ComparisonExpression::visit(TreeVisitor* visitor)
    {
        return visitor->visit(this);
    }
    ComparisonExpression::ComparisonExpression(std::unique_ptr<Nodes::Expression> left, std::unique_ptr<Nodes::Expression> right, Lexer::Token token): left(std::move(left)), right(std::move(right)), token(ParserToken(token)) {}



    Nodes::Object* AddSubExpression::visit(TreeVisitor* visitor)
    {
        return visitor->visit(this);
    }
    AddSubExpression::AddSubExpression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right, Lexer::Token token) : left(std::move(left)), right(std::move(right)), token(ParserToken(token)) {};

    Nodes::Object* MulDivExpression::visit(TreeVisitor* visitor)
    {
        visitor->visit(this);
    }
    MulDivExpression::MulDivExpression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right, Lexer::Token token) : left(std::move(left)), right(std::move(right)),token(ParserToken(token)){};

    Nodes::Object* UnaryExpression::visit(TreeVisitor* visitor)
    {
        visitor->visit(this);
    }
    UnaryExpression::UnaryExpression(std::unique_ptr<Expression> left) : left(std::move(left)) {}

    Nodes::Object* VarAssignmentOrFuncCall::visit(TreeVisitor* visitor, Identifier* identifier)
    {
        visitor->visit(this, identifier);
    }

    //::VarAssignmentOrFuncCall(std::unique_ptr<Expression> expression, Lexer::Token token) : expression(std::move(expression)), operatorAssignment(ParserToken(token)) {}
    //VarAssignmentOrFuncCall::VarAssignmentOrFuncCall(std::unique_ptr<Arguments> arguments) : arguments(std::move(arguments)) {}

    IdentifierAssigmentOrCall::IdentifierAssigmentOrCall(std::unique_ptr<Identifier> identifier, std::unique_ptr<VarAssignmentOrFuncCall> varAssignmentOrFunCall) :
        identifier(std::move(identifier)), varAssignmentOrFuncCall(std::move(varAssignmentOrFunCall)) {}

    Nodes::Object* IdentifierAssigmentOrCall::visit(TreeVisitor* visitor)
    {
        return visitor->visit(this);
    }
    PrimaryExpression::PrimaryExpression(Lexer::Token token)
    {
        if (token.type == Lexer::token_type::IntegerLiteral || token.type == Lexer::token_type::StringLiteral)
            intOrStringToken = ParserToken(token);

    }
    PrimaryExpression::PrimaryExpression(std::unique_ptr<Identifier> identifier, Lexer::Token token) : identifier(std::move(identifier)) {
        this->token = ParserToken(token);
    }


    PrimaryExpression::PrimaryExpression(std::unique_ptr<Expression> expression)
    {
        this->expression = std::move(expression);
    }
    PrimaryExpression::PrimaryExpression(std::unique_ptr<IdentifierAssigmentOrCall> funcCall, Lexer::Token token)
    {
        this->funcCall = std::move(funcCall);
        this->token = ParserToken(token);
    }
    Nodes::Object* PrimaryExpression::visit(TreeVisitor* visitor)
    {
        return visitor->visit(this);
    }
    Nodes::Object* Instruction::visit(TreeVisitor *visitor) {
       return visitor->visit(this);
    }
    Nodes::Object* Print::visit(TreeVisitor *visitor, std::vector<Nodes::Object *> expressions) {
        return visitor->visit(this, expressions);
    }
    Nodes::Object* StdLib::visit(TreeVisitor *visitor, std::vector<Nodes::Object *> expressions) {
        return visitor->visit(this, expressions);
    }

    Assignment::Assignment(std::unique_ptr<Expression> expression): expression(std::move(expression))
    {
    }
    Call::Call(std::unique_ptr<Nodes::Arguments> arguments): arguments(std::move(arguments))
    {
    }
    Nodes::Object* Nodes::ContinueStatement::visit(TreeVisitor *visitor) {
        return visitor->visit(this);
    }
    Nodes::Object* Nodes::BreakStatement::visit(TreeVisitor *visitor) {
        return visitor->visit(this);
    }
}
