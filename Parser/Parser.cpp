#include <iostream>

#include "../lexer/Lexer.h"
#include "../Utils/ErrorHanlder.h"
#include "../Tree/Nodes.h"
#include "../Utils/ErrorHanlder.h"

class Token;


class Parser
{
public:
	Parser(Lexer::Lexer& lexer): lexer(lexer){
	
		this -> current_token = lexer.next_token();
	}
    bool isError = false;
	std::unique_ptr<Nodes::Program> parse()
	{
		std::vector<std::unique_ptr<Nodes::ClassDefinition>> classes;
		auto classDef = std::move(parseClassDef());
		while(classDef != nullptr)
		{
			classes.push_back(std::move(classDef));
            classDef = std::move(parseClassDef());
		}
		auto fun = std::move(parseFunctionDef());
		return std::move(std::make_unique<Nodes::Program>(std::move(classes), std::move(fun)));
	}
private:
	Lexer::Lexer& lexer;
    Lexer::Token current_token;

	const std::string makeErrorMarker(const unsigned int& pos)
	{
	    return std::string(pos, ' ').append("^");
	}

	Lexer::Token accept(Lexer::token_type expected_token_type)
	{
		if (current_token.type != expected_token_type)
		{
            isError = true;
			ErrorHandler::error(
				                std::string("Unexpected token: ")
				                        .append(getTokenTypeName(current_token.type))
				                        .append(" (Line: ")
				                        .append(std::to_string(current_token.line))
				                        .append(", Pos: ")
				                        .append(std::to_string(current_token.position))
				                        .append(")")
				                        .append("\n")
                                        .append("expected: ")
                                        .append(getTokenTypeName(expected_token_type))
				                        , false
				        ); 
			return expected_token_type;
		}
		else
		{
			auto prevToken = current_token;
			this->current_token = lexer.next_token();
			return prevToken;
		}

	}

	std::unique_ptr<Nodes::FunDefinition> parseFunctionDef()
	{

		if (current_token.type != Lexer::token_type::Function)
			return nullptr;
		accept(Lexer::token_type::Function);
		auto identifier = accept(Lexer::token_type::Identifier);
		auto parametersList = parseParametersList();
		auto blockStatement = parseStatementBlock();
		std::unique_ptr<Nodes::FunDefinition> function = std::make_unique<Nodes::FunDefinition>(identifier.value, std::move(parametersList), std::move(blockStatement));

		return std::move(function);
	}

	std::unique_ptr<Nodes::ClassDefinition> parseClassDef()
	{
		if (current_token.type != Lexer::token_type::Class)
			return nullptr;
		accept(Lexer::token_type::Class);
		auto identifier = accept(Lexer::token_type::Identifier);
		accept(Lexer::token_type::LeftBracket);
		std::vector<std::unique_ptr<Nodes::VarAssigment>> variables;
		std::unique_ptr<Nodes::VarAssigment> variable = parseVarAssigmenet();
		while(variable != nullptr)
		{
			variables.push_back(std::move(variable));
			variable = parseVarAssigmenet();
		}
		std::unique_ptr<Nodes::FunDefinition> funcDef = parseFunctionDef();
		std::vector<std::unique_ptr<Nodes::FunDefinition>> funcDefinitions;
		while(funcDef != nullptr)
		{
			funcDefinitions.push_back(std::move(funcDef));
			funcDef = parseFunctionDef();
		}
		accept(Lexer::token_type::RightBracket);
		return std::make_unique<Nodes::ClassDefinition>(std::move(funcDefinitions), std::move(variables), identifier.value);
	}

	std::unique_ptr<Nodes::VarAssigment> parseVarAssigmenet()
	{
		if (current_token.type != Lexer::token_type::Identifier)
			return nullptr;
		auto identifier = parseIdentifier();
		accept(Lexer::token_type::Assignment);
		auto simpleExpression = parsePrimaryExpression();
		accept(Lexer::token_type::Semicolon);
		return std::move(std::make_unique<Nodes::VarAssigment>(std::move(identifier), std::move(simpleExpression)));
	}

	std::unique_ptr<Nodes::ParametersList> parseParametersList()
	{
		accept(Lexer::token_type::LeftParen);
		if(current_token.type == Lexer::token_type::RightParen)
		{
			accept(Lexer::token_type::RightParen);
			return std::move(std::make_unique<Nodes::ParametersList>());
		}
		std::vector<std::unique_ptr<Nodes::Variable>> parameters;
		
		auto variable = this->parseVariable();
		while(current_token.type == Lexer::token_type::Comma)
		{
			accept(Lexer::Comma);
			parameters.push_back(std::move(variable));
			variable = std::move(parseVariable());
		}
		parameters.push_back(std::move(variable));
		accept(Lexer::token_type::RightParen);
		return std::move(std::make_unique<Nodes::ParametersList>(std::move(parameters)));
    }

    std::unique_ptr<Nodes::Arguments> parseArguments()
    {
        accept(Lexer::token_type::LeftParen);
        if(current_token.type == Lexer::token_type::RightParen)
        {
            accept(Lexer::token_type::RightParen);
            return std::move(std::make_unique<Nodes::Arguments>());
        }
        std::vector<std::unique_ptr<Nodes::Expression>> arguments;

        auto argument = this->parseExpression();
        while(current_token.type == Lexer::token_type::Comma)
        {
            accept(Lexer::Comma);
            arguments.push_back(std::move(argument));
            argument = std::move(parseExpression());
        }
        arguments.push_back(std::move(argument));
        accept(Lexer::token_type::RightParen);
        return std::move(std::make_unique<Nodes::Arguments>(std::move(arguments)));
    };

	std::unique_ptr<Nodes::Variable> parseVariable()
	{

		if (current_token.type != Lexer::Identifier)
			return nullptr;
		auto identifier = parseIdentifier();
		std::unique_ptr<Nodes::Variable> variable = std::make_unique<Nodes::Variable>(std::move(identifier));
		return std::move(variable);
	}

	std::unique_ptr<Nodes::StatementBlock> parseStatementBlock()
	{
		accept(Lexer::token_type::LeftBracket);
		std::vector<std::unique_ptr<Nodes::Instruction>> instructions;
		std::unique_ptr<Nodes::Instruction> instruction = std::move(parseInstruction());
		while(instruction != nullptr)
		{
			instructions.push_back(std::move(instruction));
			instruction = parseInstruction();
		}
		accept(Lexer::token_type::RightBracket);

		return std::move(std::make_unique<Nodes::StatementBlock>(std::move(instructions)));
	}

	std::unique_ptr<Nodes::Instruction> parseInstruction()
	{
		std::unique_ptr<Nodes::Instruction> instruction;
		if ((instruction = parseIfStatement()) != nullptr
			|| (instruction = parseWhileStatement()) != nullptr
			|| (instruction = parseReturnStatement()) != nullptr
			|| (instruction = parseIdentiferAssigmentOrCall()) != nullptr
            || (instruction = parseBreakStatement())!= nullptr
            || (instruction = parseContinueStatement())!= nullptr)
			return std::move(instruction);
		return nullptr;
	}
    std::unique_ptr<Nodes::BreakStatement> parseBreakStatement()
    {
        if(current_token.type == Lexer::token_type::Break)
        {
            accept(Lexer::token_type::Break);
            accept(Lexer::token_type::Semicolon);
            return std::move(std::make_unique<Nodes::BreakStatement>());
        }
        return nullptr;
    }

    std::unique_ptr<Nodes::ContinueStatement> parseContinueStatement()
    {
        if(current_token.type == Lexer::token_type::Continue)
        {
            accept(Lexer::token_type::Continue);
            accept(Lexer::token_type::Semicolon);
            return std::move(std::make_unique<Nodes::ContinueStatement>());
        }
        return nullptr;
    }

	std::unique_ptr<Nodes::IdentifierAssigmentOrCall> parseIdentiferAssigmentOrCall()
	{
		if (current_token.type != Lexer::token_type::Identifier)
			return nullptr;
		auto identifier = parseIdentifier();
		std::unique_ptr<Nodes::VarAssignmentOrFuncCall> varAssigmentOrCall = parseVarAssignmenetOrFuncCall();
		accept(Lexer::token_type::Semicolon);
		return std::move(std::make_unique<Nodes::IdentifierAssigmentOrCall>(std::move(identifier), std::move(varAssigmentOrCall)));
	}

	std::unique_ptr<Nodes::ReturnStatement> parseReturnStatement()
	{
		if (current_token.type != Lexer::token_type::Return)
		{
			return nullptr;
		}
		accept(Lexer::token_type::Return);
		auto expression = parseExpression();
		accept(Lexer::token_type::Semicolon);
		return std::move(std::make_unique<Nodes::ReturnStatement>(std::move(expression)));
	}

	std::unique_ptr<Nodes::WhileStatement> parseWhileStatement()
	{
		if(current_token.type != Lexer::token_type::While)
		{
			return nullptr;
		}
		accept(Lexer::token_type::While);
		std::unique_ptr<Nodes::Condition> condition = parseCondition();
		if(condition == nullptr)
		{
			ErrorHandler::error("something went wrong, condition is nullptr");
		}
		std::unique_ptr<Nodes::StatementBlock> statementBlock = parseStatementBlock();
		if(statementBlock == nullptr)
		{
            ErrorHandler::error("something went wrong, statement block is nullptr");
		}
		return std::move(std::make_unique<Nodes::WhileStatement>(std::move(condition), std::move(statementBlock)));
	}

	std::unique_ptr<Nodes::IfStatement> parseIfStatement()
	{
	    if(current_token.type != Lexer::token_type::If)
	        return nullptr;
	    accept({Lexer::token_type::If});
	    auto condition = std::move(parseCondition());
	    if(condition==nullptr)
	        std::cout<<"error";
	    auto block = parseStatementBlock();
	    if(block== nullptr)
	        std::cout<<"error";
	    std::vector<std::unique_ptr<Nodes::IfStatement>> elifBlocks;
		auto elifBlock = std::move(parseElifBlock());
		while (elifBlock != nullptr)
		{
			elifBlocks.push_back(std::move(elifBlock));
			elifBlock = std::move(parseElifBlock());
		}
	    return std::move(std::make_unique<Nodes::IfStatement>(std::move(condition), std::move(block), std::move(parseElse()), std::move(elifBlocks)));
	}

	std::unique_ptr<Nodes::StatementBlock> parseElse()
	{
		if (current_token.type != Lexer::token_type::Else)
			return nullptr;
		accept({ Lexer::token_type::Else });

		return std::move(parseStatementBlock());
	}

	std::unique_ptr<Nodes::IfStatement> parseElifBlock()
	{
		if (current_token.type != Lexer::token_type::Elif)
			return nullptr;
		accept({ Lexer::token_type::Elif });
		auto condition = parseCondition();
		if (condition == nullptr)
			std::cout << "error";
		auto block = parseStatementBlock();
		return std::move(std::make_unique<Nodes::IfStatement>(std::move(condition), std::move(block)));
	}

	std::unique_ptr<Nodes::Condition> parseCondition()
	{
		accept(Lexer::token_type::LeftParen);
		std::unique_ptr<Nodes::Expression> expression = std::move(parseExpression());
		std::unique_ptr<Nodes::Condition> condition = std::make_unique<Nodes::Condition>(std::move(expression));
		accept(Lexer::token_type::RightParen);
		return std::move(condition);
	}

	std::unique_ptr<Nodes::Expression> parseExpression()
	{
		return std::move(parseOrExpression());
	}

	std::unique_ptr<Nodes::Expression> parseOrExpression()
	{
		std::unique_ptr<Nodes::Expression> left = std::move(parseAndExpression());
		if (left == nullptr)
			return nullptr;
		while (current_token.type == Lexer::token_type::Or)
		{
            Lexer::Token operatorToken = accept(Lexer::token_type::Or);
			std::unique_ptr<Nodes::Expression> right = parseAndExpression();
			if (right == nullptr)
			{
				std::cout << "error";
			}
			left = std::make_unique<Nodes::OrExpression>(left, right);
		}

		return std::move(left);
	}

	std::unique_ptr<Nodes::Expression> parseAndExpression()
	{
		std::unique_ptr<Nodes::Expression> left = std::move(parseLogicNegationExpression());
		if (left == nullptr)
			return nullptr;
		while (current_token.type == Lexer::token_type::And)
		{
            Lexer::Token operatorToken = accept(Lexer::token_type::And);
			std::unique_ptr<Nodes::Expression> right = parseLogicNegationExpression();
			if (right == nullptr)
			{
				std::cout << "error";
			}
			left = std::make_unique<Nodes::AndExpression>(left, right);
		}

		return std::move(left);
	}

	std::unique_ptr<Nodes::Expression> parseLogicNegationExpression()
	{
		if(current_token.type == Lexer::token_type::Negation)
		{
			accept(Lexer::token_type::Negation);
			return std::move(std::make_unique<Nodes::LogicNegationExpression>(std::move(parseComparisonExpression())));
		}
		
		return std::move(parseComparisonExpression());
	}

	std::unique_ptr<Nodes::Expression> parseComparisonExpression()
	{
		std::unique_ptr<Nodes::Expression> left = parseAddSubExpression();
		if(left == nullptr)
		{
			return nullptr;
		}
		if(current_token.type == Lexer::token_type::Equal || current_token.type == Lexer::token_type::NotEqual || current_token.type == Lexer::token_type::Greater ||
			current_token.type == Lexer::token_type::GreaterEqual || current_token.type == Lexer::token_type::Less || current_token.type == Lexer::token_type::LessEqual)
		{
            Lexer::Token operatorToken = accept(current_token.type);
			std::unique_ptr<Nodes::Expression> right = parseAddSubExpression();
			if(right == nullptr)
			{
                ErrorHandler::error("something went wrong, right is nullptr at line" + std::to_string(operatorToken.line) );
			}
			left = std::make_unique<Nodes::ComparisonExpression>(std::move(left), std::move(right), operatorToken);
		}
		return std::move(left);
	}

	std::unique_ptr<Nodes::Expression> parseAddSubExpression()
	{
		std::unique_ptr<Nodes::Expression> left = parseMulDivExpression();
		if (left == nullptr)
		{
			return nullptr;
		}
		if (current_token.type == Lexer::token_type::Plus || current_token.type == Lexer::token_type::Minus)
		{
            Lexer::Token operatorToken = accept(current_token.type);
			std::unique_ptr<Nodes::Expression> right = parseMulDivExpression();
			if (right == nullptr)
			{
                ErrorHandler::error("something went wrong, right is nullptr at" + std::to_string(operatorToken.line));
			}
			left = std::make_unique<Nodes::AddSubExpression>(std::move(left), std::move(right), operatorToken);
		}
		return std::move(left);
	}

	std::unique_ptr<Nodes::Expression> parseMulDivExpression()
	{
		std::unique_ptr<Nodes::Expression> left = parseUnaryExpression();
		if (left == nullptr)
		{
			return nullptr;
		}
		if (current_token.type == Lexer::token_type::Multiply || current_token.type == Lexer::token_type::Divide)
		{
			Lexer::Token operatorToken = accept(current_token.type);
			std::unique_ptr<Nodes::Expression> right = parseUnaryExpression();
			if (right == nullptr)
			{
                ErrorHandler::error("something went wrong, right is nullptr at line" + std::to_string(operatorToken.line));
			}
			left = std::make_unique<Nodes::MulDivExpression>(std::move(left), std::move(right), operatorToken);
		}
		return std::move(left);
	}

	std::unique_ptr<Nodes::Expression> parseUnaryExpression()
	{
		if(current_token.type == Lexer::token_type::Minus)
		{
            Lexer::Token operatorToken = accept(Lexer::token_type::Minus);
			return std::move(std::make_unique<Nodes::UnaryExpression>(std::move(parsePrimaryExpression())));
		}
		return std::move(parsePrimaryExpression());
	}

	std::unique_ptr<Nodes::Expression> parsePrimaryExpression()
	{
		if(current_token.type == Lexer::token_type::Identifier)
		{
            auto token = current_token;
			auto identifier = parseIdentifier();
            auto varAssignmentOrFunCall = parseVarAssignmenetOrFuncCall();
            if(varAssignmentOrFunCall!= nullptr)
            {
                return std::move(std::make_unique<Nodes::IdentifierAssigmentOrCall>(std::move(identifier), std::move(varAssignmentOrFunCall)));
            }
            auto result = std::make_unique<Nodes::PrimaryExpression>(std::move(identifier), token);
            return std::move(result);
		}
        if(current_token.type == Lexer::token_type::StringLiteral)
        {
            Lexer::Token token = accept(Lexer::token_type::StringLiteral);
            return std::move(std::make_unique<Nodes::PrimaryExpression>(token));
        }
		if(current_token.type == Lexer::token_type::IntegerLiteral)
		{
            Lexer::Token token = accept(Lexer::token_type::IntegerLiteral);
			return std::move(std::make_unique<Nodes::PrimaryExpression>(token));
		}
        if(current_token.type == Lexer::token_type::LeftParen)
        {
            accept(Lexer::token_type::LeftParen);
            auto expression = parseExpression();
            accept(Lexer::token_type::RightParen);
            return expression;
        }
		return nullptr;
	}

    std::unique_ptr<Nodes::Call> parseCall()
    {
        if(current_token.type == Lexer::token_type::LeftParen)
        {
            return std::move(std::make_unique<Nodes::Call>(parseArguments()));
        }
        return nullptr;
    }

    std::unique_ptr<Nodes::Assignment> parseAssignment()
    {
        if(current_token.type != Lexer::token_type::Assignment)
        {
            return nullptr;
        }
        accept(Lexer::token_type::Assignment);
        auto result = parseExpression();
        return std::move(std::make_unique<Nodes::Assignment>(std::move(result)));
    }

	std::unique_ptr<Nodes::VarAssignmentOrFuncCall> parseVarAssignmenetOrFuncCall()
	{
        std::unique_ptr<Nodes::VarAssignmentOrFuncCall> varAssignmentOrFuncCall;

        varAssignmentOrFuncCall = std::move(parseCall());
        if(varAssignmentOrFuncCall != nullptr)
            return std::move(varAssignmentOrFuncCall);
        varAssignmentOrFuncCall = std::move(parseAssignment());
        if(varAssignmentOrFuncCall!= nullptr)
            return std::move(varAssignmentOrFuncCall);
        return nullptr;
	}

	std::unique_ptr<Nodes::Identifier> parseIdentifier() //todo
	{
		if(current_token.type != Lexer::token_type::Identifier)
		{
			return nullptr;
		}
		std::vector<std::string> identifiers;
        int i=0;
        auto token = current_token;
		while(current_token.type == Lexer::token_type::Identifier)
		{
            if(i>2)
                break;
			identifiers.push_back(current_token.value);
			accept(Lexer::token_type::Identifier);
			if (current_token.type == Lexer::token_type::Dot)
			{
				accept(Lexer::token_type::Dot);
			}
            i++;
		}
		return std::move(std::make_unique<Nodes::Identifier>(identifiers,token));
	}
};