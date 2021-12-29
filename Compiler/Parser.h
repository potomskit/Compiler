#pragma once
#include "Nodes.h"
#include "Token.h"
#include "tracer.h"

class lexer;

//Unique Pointer
class Parser
{
public:
    Parser(lexer& lexer);

    std::unique_ptr<Nodes::Program> parse();

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

    std::unique_ptr<Nodes::FunDefinition> parseFunction();
    std::unique_ptr<Nodes::ClassDefinition> parseClass();

    std::vector<std::string> parseParameters();
    std::unique_ptr<Nodes::StatementBlock> parseStatementBlock();
    std::unique_ptr<Nodes::ClassBlock> parseClassBlock();
	
    std::unique_ptr<Nodes::IfStatement> parseIfStatement();
    std::unique_ptr<Nodes::WhileStatement> parseWhileStatement();
    std::unique_ptr<Nodes::ReturnStatement> parseReturnStatement();
    std::unique_ptr<Nodes::VarDeclaration> parseInitStatement();
    NodePtr parseAssignmentOrFunCall();
    std::unique_ptr<Nodes::LoopJump> parseLoopJump();

    std::unique_ptr<Nodes::Identifier> parseIdentifier();
    std::unique_ptr<Nodes::Assignable> parseAssignable();
    std::unique_ptr<Nodes::Call> parseFunCall(const std::unique_ptr<Nodes::Identifier>& identifier);
    std::unique_ptr<Nodes::Variable> parseVariable(const std::unique_ptr<Nodes::Identifier>& identifier);
    std::unique_ptr<Nodes::IntegerLiteral> parseLiteral();
    int parseNumberLiteral();
    
    std::unique_ptr<Nodes::Expression> parseExpression(const std::unique_ptr<Nodes::Identifier>& identifier = nullptr);
    std::unique_ptr<Nodes::Expression> parseMultiplicativeExpression(const std::unique_ptr<Nodes::Identifier>& identifier = nullptr);
    NodePtr parsePrimaryExpression(const std::unique_ptr<Nodes::Identifier>& identifier = nullptr);

    std::unique_ptr<Nodes::Condition> parseCondition();
    std::unique_ptr<Nodes::Condition> parseAndCondition();
    std::unique_ptr<Nodes::Condition> parseEqualityCondition();
    std::unique_ptr<Nodes::Condition> parseRelationalCondition();
    NodePtr parsePrimaryCondition();
};