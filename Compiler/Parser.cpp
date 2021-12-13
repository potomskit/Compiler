#pragma once
#include "Parser.h"
#include <iostream>
#include <limits>
#include "ErrorHandler.h"
#include "Lexer.h"

using Parser = Parser;
using Lexer = lexer;

Parser::Parser(Lexer& lexer) :lexer(lexer)
{}

std::shared_ptr<Nodes::Program> Parser::parse()
{
    this->tracer.reset();

    this->resetPreviousToken();

    token token;

    std::shared_ptr<Nodes::Program> syntaxTree = std::make_shared<Nodes::Program>();
    std::shared_ptr<Nodes::ClassDefinition> last_class;
    this->tracer.enter("Starting parser tracing...");
	
    while (this->peek({token_type::Class}))
    {
        syntaxTree->addClass(this->parseClass());
    }
    syntaxTree->addFunction(this->parseFunction());
	
    this->tracer.leave("Trace ended...");
    this->accept({ token_type::EndOfFile });
    return syntaxTree;
}

bool Parser::isAcceptable(const token& token, const std::initializer_list<token_type>& acceptable) const
{
    for (auto& it : acceptable)
    {
        if (token.type == it)
        {
            return true;
        }
    }

    return false;
}

token Parser::accept(const std::initializer_list<token_type>& acceptable)
{
    token token;

    if (this->hasBufferedToken())
    {
        token = this->previousToken;
        this->resetPreviousToken();
    }
    else
    {
        token = this->lexer.next_token();
    }

    if (this->isAcceptable(token, acceptable))
    {
        return token;
    }
    else
    {
        ErrorHandler::error(
            std::string("Unexpected token: ")
            .append(getTokenTypeName(token.type))
            .append(" (Line: ")
            .append(std::to_string(token.line))
            .append(", Pos: ")
            .append(std::to_string(token.position))
            .append(")")
            .append("\n")
            //.append(this->lexer.getLine(token.line_start))
            .append("\n")
            .append(this->makeErrorMarker(token.position))
        );

        return token;
    }
}

bool Parser::peek(const std::initializer_list<token_type>& acceptable)
{
    if (!this->hasBufferedToken())
    {
        this->previousToken = this->lexer.next_token();
    }

    return this->isAcceptable(this->previousToken, acceptable);
}

token Parser::getPeeked()
{
    if (!this->hasBufferedToken())
    {
        ErrorHandler::debugFatal(std::string("Nothing peeked"));
    }

    return this->previousToken;
}

void Parser::peekFail()
{
    token& token = this->previousToken;

    ErrorHandler::error(
        std::string("Unexpected token: ")
        .append(getTokenTypeName(token.type))
        .append(" (Line: ")
        .append(std::to_string(token.line))
        .append(", Pos: ")
        .append(std::to_string(token.position))
        .append(")")
        .append("\n")
        .append(this->makeErrorMarker(token.position))
    );
}

const std::string Parser::makeErrorMarker(const unsigned int& pos)
{
    return std::string(pos, ' ').append("^");
}

bool Parser::hasBufferedToken() const
{
    return this->previousToken.type != token_type::Undefined;
}

void Parser::resetPreviousToken()
{
    this->previousToken.type = token_type::Undefined;
    this->previousToken.value = "";
    this->previousToken.line = 0;
    this->previousToken.position = 0;
}

std::shared_ptr<Nodes::FunDefinition> Parser::parseFunction()
{
    std::shared_ptr<Nodes::FunDefinition> node = std::make_shared<Nodes::FunDefinition>();

    this->tracer.enter("Parsing function");

    auto tempToken = this->accept({ token_type::Function, token_type::EndOfFile });
    if (tempToken.type == token_type::EndOfFile)
    {
        this->tracer.leave("EOF");
        return nullptr;
    }
     
    tempToken = this->accept({ token_type::Identifier });
    node->setName(tempToken.value);
    node->setParameters(this->parseParameters());
    node->setBlock(this->parseStatementBlock());

    this->tracer.leave();
    return node;
}

std::shared_ptr<Nodes::ClassDefinition> Parser::parseClass()
{
    std::shared_ptr<Nodes::ClassDefinition> node = std::make_shared<Nodes::ClassDefinition>();
    this->tracer.enter("Parsing class");
    auto tempToken = this->accept({ token_type::Class, token_type::EndOfFile });
	if(tempToken.type == token_type::EndOfFile)
	{
        this->tracer.leave("EOF");
        return nullptr;
	}
    tempToken = this->accept({ token_type::Identifier });
    node->setName(tempToken.value);
    node->setBlock(this->parseClassBlock());
    this->tracer.leave();
    return node;
}

std::vector<std::string> Parser::parseParameters()
{
    std::vector<std::string> parametersNames;

    this->tracer.enter("Parsing parameters");

    token tempToken;

    this->accept({ token_type::LeftParen });

    tempToken = this->accept({ token_type::RightParen, token_type::Identifier });
    if (tempToken.type != token_type::RightParen)
    {
        parametersNames.push_back(tempToken.value);

        while (true)
        {
            tempToken = this->accept({ token_type::RightParen, token_type::Comma });
            if (tempToken.type == token_type::RightParen)
            {
                break;
            }
            tempToken = this->accept({ token_type::Identifier });

            parametersNames.push_back(tempToken.value);
        }
    }

    this->tracer.leave();
    return parametersNames;
}

std::shared_ptr<Nodes::ClassBlock> Parser::parseClassBlock()
{
    std::shared_ptr<Nodes::ClassBlock> node = std::make_shared<Nodes::ClassBlock>();

    this->tracer.enter("Parsing statement block");

    this->accept({ token_type::LeftBracket });

    token tempToken;

    while (true)
    {
        if (!this->peek({
            token_type::Identifier,
            token_type::Function,
            }))
        {
            break;
        }

        tempToken = this->getPeeked();

        switch (tempToken.type)
        {
        case token_type::Identifier:
            node->addVariable(this->parseInitStatement());
            break;
        case token_type::Function:
            node->addFunction(this->parseFunction());
            break;
        default:
            break;
        }
    }

    this->accept({ token_type::RightBracket });

    this->tracer.leave();
    return node;
}

std::shared_ptr<Nodes::StatementBlock> Parser::parseStatementBlock()
{
    std::shared_ptr<Nodes::StatementBlock> node = std::make_shared<Nodes::StatementBlock>();

    this->tracer.enter("Parsing statement block");

    this->accept({ token_type::LeftBracket });

    token tempToken;

    while (true)
    {
        if (!this->peek({
            token_type::If,
            token_type::While,
            token_type::Return,
            token_type::LeftBracket,
            token_type::Identifier,
            token_type::Continue,
            token_type::Break
            }))
        {
            break;
        }

        tempToken = this->getPeeked();

        switch (tempToken.type)
        {
        case token_type::If:
            node->addInstruction(this->parseIfStatement());
            break;
        case token_type::While:
            node->addInstruction(this->parseWhileStatement());
            break;
        case token_type::Return:
            node->addInstruction(this->parseReturnStatement());
            break;
        case token_type::LeftBracket:
            node->addInstruction(this->parseStatementBlock());
            break;
        case token_type::Identifier:
            node->addInstruction(this->parseAssignmentOrFunCall());
            break;
        case token_type::Continue:
        case token_type::Break:
            node->addInstruction(this->parseLoopJump());
            break;
        default:
            break;
        }
    }

    this->accept({ token_type::RightBracket });

    this->tracer.leave();
    return node;
}

std::shared_ptr<Nodes::IfStatement> Parser::parseIfStatement()
{
    std::shared_ptr<Nodes::IfStatement> node = std::make_shared<Nodes::IfStatement>();

    this->tracer.enter("Parsing if statement");

    this->accept({ token_type::If });
    this->accept({ token_type::LeftParen });

    node->setCondition(this->parseCondition());

    this->accept({ token_type::RightParen });

    node->setTrueBlock(this->parseStatementBlock());

    if (this->peek({ token_type::Else }))
    {
        this->accept({ token_type::Else });

        node->setFalseBlock(this->parseStatementBlock());
    }

    this->tracer.leave();
    return node;
}

std::shared_ptr<Nodes::WhileStatement> Parser::parseWhileStatement()
{
    std::shared_ptr<Nodes::WhileStatement> node = std::make_shared<Nodes::WhileStatement>();

    this->tracer.enter("Parsing while statement");

    this->accept({ token_type::While });
    this->accept({ token_type::LeftParen });

    node->setCondition(this->parseCondition());

    this->accept({ token_type::RightParen });

    node->setBlock(this->parseStatementBlock());

    this->tracer.leave();
    return node;
}

std::shared_ptr<Nodes::ReturnStatement> Parser::parseReturnStatement()
{
    std::shared_ptr<Nodes::ReturnStatement> node = std::make_shared<Nodes::ReturnStatement>();

    this->tracer.enter("Parsing return statement");

    this->accept({ token_type::Return });

    node->setValue(this->parseAssignable());

    this->accept({ token_type::Semicolon });

    this->tracer.leave();
    return node;
}

std::shared_ptr<Nodes::VarDeclaration> Parser::parseInitStatement()
{
    std::shared_ptr<Nodes::VarDeclaration> node = std::make_shared<Nodes::VarDeclaration>();

    this->tracer.enter("Parsing init statement");


    auto nameToken = this->accept({ token_type::Identifier });
    node->setName(nameToken.value);

   
    this->accept({ token_type::Assignment });

    node->setValue(this->parseAssignable());
    

    this->accept({ token_type::Semicolon });

    this->tracer.leave();
    return node;
}

NodePtr Parser::parseAssignmentOrFunCall()
{
    NodePtr node;

    this->tracer.enter("Parsing assignment or function call");

    auto tempToken1 = this->accept({ token_type::Identifier });
    token tempToken2;
	if(this->peek({token_type::Dot}))
	{
        this->accept({ token_type::Dot });
        tempToken2 = this->accept({ token_type::Identifier });
	}
    
    node = this->parseFunCall(tempToken1.value + "." + tempToken2.value);
    
    if (!node)
    {
        std::shared_ptr<Nodes::Assignment> assignmentNode = std::make_shared<Nodes::Assignment>();
        tempToken1.value = tempToken1.value + "." + tempToken2.value;
        assignmentNode->setVariable(this->parseVariable(tempToken1));

        this->accept({ token_type::Assignment });

        assignmentNode->setValue(this->parseAssignable());

        node = assignmentNode;
    }

    this->accept({ token_type::Semicolon });

    this->tracer.leave();
    return node;
}

std::shared_ptr<Nodes::LoopJump> Parser::parseLoopJump()
{
    std::shared_ptr<Nodes::LoopJump> node = std::make_shared<Nodes::LoopJump>();

    this->tracer.enter("Parsing loop jump");

    auto token = this->accept({ token_type::Continue, token_type::Break });
    node->setType(token.type);

    this->accept({ token_type::Semicolon });

    this->tracer.leave();
    return node;
}

std::shared_ptr<Nodes::Assignable> Parser:: parseAssignable()
{
    std::shared_ptr<Nodes::Assignable> node;
    token tempIdentifier2;;
    this->tracer.enter("Parsing assignable");
    if (this->peek({ token_type::Identifier }))
    {
        auto tempIdentifier1 = this->accept({ token_type::Identifier });
        if (this->peek({ token_type::Dot }))
        {
            this->accept({ token_type::Dot });
            tempIdentifier2 = this->accept({ token_type::Identifier });
        }
		node = this->parseFunCall(tempIdentifier1.value + "." + tempIdentifier2.value);
        
        if (!node)
        {
            tempIdentifier1.value = tempIdentifier1.value + "." + tempIdentifier2.value;
			node = this->parseExpression(tempIdentifier1);
        }
    }
    else
    {
        node = this->parseExpression();
    }

    this->tracer.leave();
    return node;
}

std::shared_ptr<Nodes::Call> Parser::parseFunCall(const std::string& identifier)
{
    std::shared_ptr<Nodes::Call> node = std::make_shared<Nodes::Call>();
    token identifier2;
    this->tracer.enter("Parsing function call");
    /*if(this->peek({token_type::Dot}))
    {
        this->accept({ token_type::Dot });
        identifier2 = this->accept({ token_type::Identifier });
    }*/
	
    if (!this->peek({ token_type::LeftParen }))
    {
        this->tracer.leave("  - not a function call");
        return nullptr;
    }
    
    int dotIndex = identifier.find('.');
    std::string objcetName = identifier.substr(0, dotIndex);
    std::string name = identifier.substr(dotIndex + 1);
    if (name == "" || nullptr)
    {
        node->setName(objcetName);
    }
    else
    {
        node->setName(name);
        node->setObjectName(objcetName);
    }

    this->accept({ token_type::LeftParen });

    if (this->peek({ token_type::RightParen }))
    {
        this->accept({ token_type::RightParen });

        this->tracer.leave("  + function call");
        return node;
    }

    while (true)
    {
        node->addArgument(this->parseAssignable());

        if (this->peek({ token_type::RightParen }))
        {
            this->accept({ token_type::RightParen });
            break;
        }
        if (this->peek({ token_type::Comma }))
        {
            this->accept({ token_type::Comma });
            continue;
        }
    	if(this->peek({token_type::StringLiteral}))
    	{
            this->accept({ token_type::StringLiteral });
    		break;
    	}

        this->peekFail();
    }

    this->tracer.leave("  + function call");
    return node;
}

std::shared_ptr<Nodes::Variable> Parser::parseVariable(const token& identifierToken)
{
    std::shared_ptr<Nodes::Variable> node = std::make_shared<Nodes::Variable>();

    this->tracer.enter("Parsing variable");
    token tempToken = identifierToken;
    if (identifierToken.type != token_type::Identifier)
    {
        tempToken = this->accept({ token_type::Identifier });
        if (this->peek({ token_type::Dot }))
        {
            this->accept({ token_type::Dot });
            auto secondtempToken = this->accept({ token_type::Identifier });
            node->setName(secondtempToken.value);
            node->setObjectName(tempToken.value);

        }
        node->setName(tempToken.value);
    	
    }
    else
    {
        
        int dotIndex = identifierToken.value.find('.');
        std::string objcetName = identifierToken.value.substr(0, dotIndex);
        std::string name = identifierToken.value.substr(dotIndex + 1);
        if (name == "" || nullptr)
        {
            node->setName(objcetName);
        }
        else
        {
            node->setName(name);
            node->setObjectName(objcetName);
        }
    }
	
   /* if (this->peek({token_type::Dot}))
    {
        this->accept({ token_type::Dot });
        auto secondtempToken = this->accept({ token_type::Identifier });
        node->setName(secondtempToken.value);
        node->setObjectName(tempToken.value);
    	
    }*/
  
    this->tracer.leave();
    return node;
}

std::shared_ptr<Nodes::IntegerLiteral> Parser::parseLiteral()
{
    std::shared_ptr<Nodes::IntegerLiteral> node = std::make_shared<Nodes::IntegerLiteral>();

    this->tracer.enter("Parsing literal");
    
    node->setValue(this->parseNumberLiteral());
    

    this->tracer.leave();
    return node;
}

double Parser::parseNumberLiteral()
{
    double value;
    bool negative = false;

    this->tracer.enter("Parsing number literal");

    if (this->peek({ token_type::Minus }))
    {
        this->accept({ token_type::Minus });

        negative = true;
    }

    auto tempToken = this->accept({ token_type::IntegerLiteral });
    
    value = std::stod(tempToken.value);
    

    if (negative)
    {
        value *= -1;
    }

    this->tracer.leave();
    return value;
}

std::shared_ptr<Nodes::Expression> Parser::parseExpression(const token& firstToken)
{
    std::shared_ptr<Nodes::Expression> node = std::make_shared<Nodes::Expression>();

    this->tracer.enter("Parsing expression");

    node->addOperand(this->parseMultiplicativeExpression(firstToken));

    while (this->peek({ token_type::Plus, token_type::Minus }))
    {
        auto tempToken = this->accept({ token_type::Plus, token_type::Minus });
        node->addOperator(tempToken.type);
        node->addOperand(this->parseMultiplicativeExpression());
    }

    this->tracer.leave();
    return node;
}


std::shared_ptr<Nodes::Expression> Parser::parseMultiplicativeExpression(const token& firstToken)
{
    std::shared_ptr<Nodes::Expression> node = std::make_shared<Nodes::Expression>();

    this->tracer.enter("Parsing multiplicative expression");

    node->addOperand(this->parsePrimaryExpression(firstToken));

    while (this->peek({ token_type::Multiply, token_type::Divide }))
    {
        auto tempToken = this->accept({ token_type::Multiply, token_type::Divide});
        node->addOperator(tempToken.type);

        node->addOperand(this->parsePrimaryExpression());
    }

    this->tracer.leave();
    return node;
}

NodePtr Parser::parsePrimaryExpression(const token& firstToken)
{
    this->tracer.enter("Parsing primary expression");
    this->tracer.info(std::string("First token type = ").append(getTokenTypeName(firstToken.type)));

    if (firstToken.type != token_type::Undefined)
    {
        auto node = this->parseVariable(firstToken);

        this->tracer.leave();
        return node;
    }

    if (this->peek({ token_type::LeftParen}))
    {
        this->accept({ token_type::LeftParen});
        auto node = this->parseExpression();
        this->accept({ token_type::RightParen });

        this->tracer.leave();
        return node;
    }

    if (this->peek({ token_type::Identifier }))
    {
        auto node = this->parseVariable();

        this->tracer.leave();
        return node;
    }

    auto node = this->parseLiteral();

    this->tracer.leave();
    return node;
}

std::shared_ptr<Nodes::Condition> Parser::parseCondition()
{
    std::shared_ptr<Nodes::Condition> node = std::make_shared<Nodes::Condition>();

    this->tracer.enter("Parsing condition");

    node->addOperand(this->parseAndCondition());

    while (this->peek({ token_type::Or }))
    {
        this->accept({ token_type::Or });
        node->setOperator(token_type::Or);

        node->addOperand(this->parseAndCondition());
    }

    this->tracer.leave();
    return node;
}

std::shared_ptr<Nodes::Condition> Parser::parseAndCondition()
{
    std::shared_ptr<Nodes::Condition> node = std::make_shared<Nodes::Condition>();

    this->tracer.enter("Parsing and condition");

    node->addOperand(this->parseEqualityCondition());

    while (this->peek({ token_type::And }))
    {
        this->accept({ token_type::And });
        node->setOperator(token_type::And);

        node->addOperand(this->parseEqualityCondition());
    }

    this->tracer.leave();
    return node;
}

std::shared_ptr<Nodes::Condition> Parser::parseEqualityCondition()
{
    std::shared_ptr<Nodes::Condition> node = std::make_shared<Nodes::Condition>();

    this->tracer.enter("Parsing equality condition");

    node->addOperand(this->parseRelationalCondition());

    if (this->peek({ token_type::Equal, token_type::Equal }))
    {
        auto tempToken = this->accept({ token_type::Equal, token_type::NotEqual });
        node->setOperator(tempToken.type);

        node->addOperand(this->parseRelationalCondition());
    }

    this->tracer.leave();
    return node;
}

std::shared_ptr<Nodes::Condition> Parser::parseRelationalCondition()
{
    std::shared_ptr<Nodes::Condition> node = std::make_shared<Nodes::Condition>();

    this->tracer.enter("Parsing relational condition");

    node->addOperand(this->parsePrimaryCondition());

    if (this->peek({ token_type::Less, token_type::Greater, token_type::LessEqual, token_type::GreaterEqual }))
    {
        auto tempToken = this->accept({ token_type::Less, token_type::Greater, token_type::LessEqual, token_type::GreaterEqual });
        node->setOperator(tempToken.type);

        node->addOperand(this->parsePrimaryCondition());
    }

    this->tracer.leave();
    return node;
}

NodePtr Parser::parsePrimaryCondition()
{
    std::shared_ptr<Nodes::Condition> node = std::make_shared<Nodes::Condition>();

    this->tracer.enter("Parsing primary condition");

    if (this->peek({ token_type::Negation }))
    {
        this->accept({ token_type::Negation });

        node->setNegated();
    }

    if (this->peek({ token_type::LeftParen }))
    {
        this->accept({ token_type::LeftParen });
        node->addOperand(this->parseCondition());
        this->accept({ token_type::RightParen });
    }
    else
    {
        if (this->peek({ token_type::Identifier }))
        {
            node->addOperand(this->parseVariable());
        }
        else
        {
            node->addOperand(this->parseLiteral());
        }
    }

    if (!node->isNegated())
    {
        this->tracer.leave();
        return node->getLeftSide();
    }

    this->tracer.leave();
    return node;
}