#pragma once
#include "Nodes.h"
#include "Token.h"
#include "tracer.h"

class lexer;

class Parser
{
public:
    Parser(lexer& lexer);

    std::shared_ptr<Nodes::Program> parse();

private:
    Tracer tracer = Tracer(true);

    lexer& lexer;
    token previousToken;

    bool isAcceptable(const token& token, const std::initializer_list<token_type>& acceptable) const;
    token accept(const std::initializer_list<token_type>& acceptable);
    bool peek(const std::initializer_list<token_type>& acceptable);
    token getPeeked();
    void peekFail();
    const std::string makeErrorMarker(const unsigned int& pos);

    bool hasBufferedToken() const;
    void resetPreviousToken();

    std::shared_ptr<Nodes::FunDefinition> parseFunction();
    std::shared_ptr<Nodes::ClassDefinition> parseClass();

    std::vector<std::string> parseParameters();
    std::shared_ptr<Nodes::StatementBlock> parseStatementBlock();
    std::shared_ptr<Nodes::ClassBlock> parseClassBlock();
	
    std::shared_ptr<Nodes::IfStatement> parseIfStatement();
    std::shared_ptr<Nodes::WhileStatement> parseWhileStatement();
    std::shared_ptr<Nodes::ReturnStatement> parseReturnStatement();
    std::shared_ptr<Nodes::VarDeclaration> parseInitStatement();
    NodePtr parseAssignmentOrFunCall();
    std::shared_ptr<Nodes::LoopJump> parseLoopJump();

    std::shared_ptr<Nodes::Assignable> parseAssignable();
    std::shared_ptr<Nodes::Call> parseFunCall(const std::string& identifier1);
    std::shared_ptr<Nodes::Variable> parseVariable(const token& firstToken = token(token_type::Undefined));
    std::shared_ptr<Nodes::IntegerLiteral> parseLiteral();
    double parseNumberLiteral();
    
    std::shared_ptr<Nodes::Expression> parseExpression(const token& firstToken = token(token_type::Undefined));
    std::shared_ptr<Nodes::Expression> parseMultiplicativeExpression(const token& firstToken = token(token_type::Undefined));
    NodePtr parsePrimaryExpression(const token& firstToken = token(token_type::Undefined));

    std::shared_ptr<Nodes::Condition> parseCondition();
    std::shared_ptr<Nodes::Condition> parseAndCondition();
    std::shared_ptr<Nodes::Condition> parseEqualityCondition();
    std::shared_ptr<Nodes::Condition> parseRelationalCondition();
    NodePtr parsePrimaryCondition();
};