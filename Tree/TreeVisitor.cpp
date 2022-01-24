#pragma once
#include <memory>
#include "Node.h"
#include "Nodes.h"
#include <iostream>
#include "TreeVisitor.h"
#include "../Utils/ErrorHanlder.h"

namespace Nodes
{
    Nodes::Object* TreeVisitor::visit(Program* program)
	{
        this->loadStandardLibrary();
        for(int i=0; i<program->classes.size(); i++)
        {
            program->classes[i]->visit(this);
        }
        auto result = program->main->visit(this);
        return result;
	}

    Nodes::Object* TreeVisitor::visit(ClassDefinition* classDef)
	{
        this->classDefinitions.insert(std::make_pair(classDef->identifier, classDef));
	}
    Nodes::Object* TreeVisitor::visit(ClassBlock* classBlock)
	{

	}
    Nodes::Object* TreeVisitor::visit(WhileStatement* whileStatement)
	{
        auto conditionResult = whileStatement->conditionNode->visit(this);
        while(conditionResult->isTrue)
        {
            auto result = whileStatement->blockNode->visit(this);
            if(result!= nullptr && result->checkIfIsReturn())
            {
                return result;
            }
            if(result!= nullptr && result->checkIfIsBreak())
            {
                return result->ToObject();
            }
            if(result!= nullptr && result->checkIfIsContinue())
            {

            }
            conditionResult = whileStatement->conditionNode->visit(this);
        }
	}
    Nodes::Object* TreeVisitor::visit(Variable* variable)
	{
        auto result = variable->identifier->visit(this);
        return result;
	}
    Nodes::Object* TreeVisitor::visit(VarAssignmentOrFuncCall* varAssignmentOrFuncCall, Identifier* identifier)
	{
        return nullptr;
	}
    Nodes::Object* TreeVisitor::visit(VarAssigment* varAssigment)
	{
        auto result = varAssigment->expression->visit(this);
        return result;
	}
    Nodes::Object* TreeVisitor::visit(UnaryExpression* unaryExpression)
	{
        auto result = unaryExpression->left->visit(this);
        return new Object(-result->value);
	}
    Nodes::Object* TreeVisitor::visit(Nodes::StringLiteral* classBlock)
	{
		std::cout << "porogram visitor";
	}
    Nodes::Object* TreeVisitor::visit(Nodes::StatementBlock* statementBlock)
 	{

        int variablesCount = this->variables.size();
        for(int i=0; i<statementBlock->statements.size(); i++)
        {
            auto result = statementBlock->statements[i]->visit(this);
            if(result!= nullptr && (result->checkIfIsReturn() || result->checkIfIsBreak() || result->checkIfIsContinue()))
                return result;
        }

        return nullptr;
	}
    Nodes::Object* TreeVisitor::visit(Nodes::PrimaryExpression* simpleExpression)
	{
        if(simpleExpression->intOrStringToken.type == ParserTokenType::IntegerLiteral)
        {
            auto result = new Nodes::IntegerLiteral(std::stoi(simpleExpression->intOrStringToken.value));
            return result;
        }
        else if(simpleExpression->identifier != nullptr)
        {
            std::string variableName = simpleExpression->identifier->identifiers[0];
            for(int i=1; i<simpleExpression->identifier->identifiers.size(); i++)
            {
                variableName=variableName + "." +simpleExpression->identifier->identifiers[i];
            }
            if(this->variables.find(variableName) == this->variables.end())
            {
                ErrorHandler::error("No variable named " + variableName + " at line " + std::to_string(simpleExpression->token.line), false);
            }
            auto result = this->variables.at(variableName);
            return result;
        }
        else if(simpleExpression->expression!= nullptr)
        {
            auto result = simpleExpression->expression->visit(this);
            return result;
        }
        else if(simpleExpression->intOrStringToken.type == ParserTokenType::StringLiteral)
        {
            auto result = new Object(simpleExpression->intOrStringToken.value);
            return result;
        }
	}

    Nodes::Object* TreeVisitor::visit(Nodes::ReturnStatement* returnStatement)
	{
        Nodes::RetrunObject* result = new RetrunObject(returnStatement->expression->visit(this));
        return result;
	}

    Nodes::Object* TreeVisitor::visit(Nodes::ParametersList* classBlock)
	{
		ErrorHandler::error("something went wrong");
	}

    Nodes::Object* TreeVisitor::visit(Nodes::Arguments* arguments) {
        ErrorHandler::error("something went wrong");
    }

    Nodes::Object* TreeVisitor::visit(Nodes::OrExpression* orExpression)
	{
        auto left = orExpression->left->visit(this);
        if(left->isTrue)
        {
            return new Object(true);
        }
        auto right = orExpression->right->visit(this);
        return new Object(left->isTrue || right->isTrue);
	}
    Nodes::Object* TreeVisitor::visit(Nodes::MulDivExpression* mulDivExpression)
	{
        auto left = mulDivExpression->left->visit(this);
        auto right = mulDivExpression->right->visit(this);
        if(mulDivExpression->token.type == ParserTokenType::Multiply)
        {
            return new IntegerLiteral(left->value*right->value);
        }
        else if(mulDivExpression->token.type == ParserTokenType::Divide)
        {
            if(right->value==0)
            {
                ErrorHandler::error("\n dividing by zero at line " + std::to_string(mulDivExpression->token.line), false);
            }
            return new IntegerLiteral(left->value/right->value);
        }
        return nullptr;
	}
    Nodes::Object* TreeVisitor::visit(Nodes::LogicNegationExpression* logicNegationExpression)
	{
        auto result = logicNegationExpression->left->visit(this);
        return new Object(!result->isTrue);
	}
    Nodes::Object* TreeVisitor::visit(Nodes::Object* literal)
	{
		ErrorHandler::error("something went wrong");
	}
    Nodes::Object* TreeVisitor::visit(Nodes::IntegerLiteral* classBlock)
	{
        ErrorHandler::error("something went wrong");
	}
    Nodes::Object* TreeVisitor::visit(Nodes::IfStatement* ifStatement)
	{

        auto conditionResult = ifStatement->conditionNode->visit(this);
        if(conditionResult->isTrue)
        {
            auto result = ifStatement->blockNode->visit(this);
            if(result!= nullptr && (result->checkIfIsReturn() || result->checkIfIsBreak() || result->checkIfIsContinue()))
                return result;
            return nullptr;
        }
        for(int i=0; i<ifStatement->elifBlocksNodes.size(); i++)
        {
            auto conditionResult = ifStatement->elifBlocksNodes[i]->conditionNode->visit(this);
            if(conditionResult->isTrue)
            {
                auto result = ifStatement->elifBlocksNodes[i]->blockNode->visit(this);
                if(result!= nullptr && (result->checkIfIsReturn() || result->checkIfIsBreak() || result->checkIfIsContinue()))
                    return result;
                return nullptr;
            }
        }
        if(ifStatement->elseBlockNode != nullptr)
        {
            auto result = ifStatement->elseBlockNode->visit(this);
            if(result!= nullptr && (result->checkIfIsReturn() || result->checkIfIsBreak() || result->checkIfIsContinue()))
                return result;
            return nullptr;
        }
        return nullptr;
	}
    Nodes::Object* TreeVisitor::visit(Nodes::IdentifierAssigmentOrCall* identifierAssigmentOrCall)
	{
       auto identifer = identifierAssigmentOrCall->identifier.get();
       if(identifierAssigmentOrCall->varAssignmentOrFuncCall!= nullptr)
       {
           auto result = identifierAssigmentOrCall->varAssignmentOrFuncCall->visit(this,identifer);
       }
	}
    Nodes::Object* TreeVisitor::visit(Nodes::FunDefinition* funDef)
	{
        Nodes::Object* result;
        if(funDef->name=="main")
        {
            this->main = funDef;
            for(int i=0; i<funDef->blockNode->statements.size(); i++)
            {
                auto instruction = funDef->blockNode->statements[i].get();
                result = instruction->visit(this);
                if(result != nullptr&& result->checkIfIsReturn())
                {
                    return result->ToObject();
                }
            }
        }
        else
        {
            ErrorHandler::error("There is no main function");
        }
        return result;
	}
    Nodes::Object* TreeVisitor::visit(Nodes::FunDefinition* funDef, std::unordered_map<std::string, Object*> variables, std::string variableName)
    {
        auto variables_copy = this->variables;
        this->variables = variables;
        Nodes::Object* result;
        for(int i=0; i<funDef->blockNode->statements.size(); i++)
        {
            auto instruction = funDef->blockNode->statements[i].get();
            result = instruction->visit(this);
            if(result != nullptr &&  result->checkIfIsReturn())
            {
                result = result->ToObject();
                break;
            }
        }

        for(auto variable: this->variables)
        {
            if(variables_copy.at(variableName)->variables.find(variable.first)!=variables_copy.at(variableName)->variables.end() )
                variables_copy.at(variableName)->variables.at(variable.first) = this->variables.at(variable.first);
        }

        this->variables = variables_copy;
        return result;
    }

    Nodes::Object* TreeVisitor::visit(Nodes::Condition* condition)
	{
        auto result = condition->expression->visit(this);
        return result;
	}

    Nodes::Object* TreeVisitor::visit(Nodes::ComparisonExpression* comparisonExpression)
	{
        auto left = comparisonExpression->left->visit(this);
        auto right = comparisonExpression->right->visit(this);
        if(comparisonExpression->token.type == ParserTokenType::Less)
        {
            return new Object(left->value < right->value);
        }
        else if(comparisonExpression->token.type == ParserTokenType::Greater)
        {
            return new Object(left->value > right->value);
        }
        else if(comparisonExpression->token.type == ParserTokenType::Equal)
        {
            return new Object(left->value == right->value);
        }
        else if(comparisonExpression->token.type == ParserTokenType::NotEqual)
        {
            return new Object(left->value != right->value);
        }
        else if(comparisonExpression->token.type == ParserTokenType::GreaterEqual)
        {
            return new Object(left->value >= right->value);
        }
        else if(comparisonExpression->token.type == ParserTokenType::LessEqual)
        {
            return new Object(left->value <= right->value);
        }

        return nullptr;
    }

    Nodes::Object* TreeVisitor::visit(Nodes::Call* funcCall, Nodes::Identifier* identifier)
	{
		Nodes::Object* result;
        std::string id = identifier->identifiers[0];
        if(identifier->identifiers.size() == 1)
        {
            std::vector<Nodes::Object*> arguments;
            if(this->standarLibraryFunctions.find(id)!=this->standarLibraryFunctions.end())
            {
                for(int i=0; i<funcCall->arguments->expressions.size(); i++)
                {
                    arguments.push_back(funcCall->arguments->expressions[i]->visit(this));
                }
                auto result = this->standarLibraryFunctions.at(id)->visit(this, arguments);
                return result;
            }
            else
            {
                if(this->classDefinitions.find(id) == this->classDefinitions.end())
                {
                    ErrorHandler::error("No function or class named " + id + " at line " + std::to_string(identifier->token.line), false);
                }
                if(funcCall->arguments->expressions.size() != 0)
                {
                    ErrorHandler::error("When creating object dont pass arguments " + id + " at line " + std::to_string(identifier->token.line), false);
                }
                auto classDef = this->classDefinitions.at(id);
                std::unordered_map<std::string, Object*> initVariables;
                for(int i=0; i<classDef->variables.size(); i++)
                {
                    initVariables.insert(std::make_pair("self." + classDef->variables[i]->identifier->identifiers[0], classDef->variables[i]->visit(this)));
                }
                result = new Object(classDef->identifier, initVariables);
                return result;
            }
        }
        else
        {
            if(this->variables.find(id)!=this->variables.end())
            {
                auto className = this->variables.at(id)->className;
                for(int i=0; i<this->classDefinitions.at(className)->funcDefinitions.size(); i++)
                {
                    if(classDefinitions.at(className)->funcDefinitions[i]->name == identifier->identifiers[1])
                    {
                        auto variables = this->variables.at(id)->variables;
                        if(funcCall->arguments->expressions.size() != this->classDefinitions.at(className)->funcDefinitions[i]->parameters->parameters.size())
                        {
                            ErrorHandler::error("bad number of arguments: expected: " + std::to_string(this->classDefinitions.at(className)->funcDefinitions[i]->parameters->parameters.size()) +
                            " but given " + std::to_string(funcCall->arguments->expressions.size() )  + " at line " +std::to_string(identifier->token.line));
                        }
                        for(int j=0; j<funcCall->arguments->expressions.size(); j++)
                        {
                            variables.insert(std::make_pair(this->classDefinitions.at(className)->funcDefinitions[i]->parameters->parameters[j]->identifier->identifiers[0],
                                                            funcCall->arguments->expressions[j]->visit(this)));
                        }

                        auto funcResult = classDefinitions.at(className)->funcDefinitions[i]->visit(this, variables, id);
                        return funcResult;
                    }
                }
            }
        }
	}

    Nodes::Object* TreeVisitor::visit(Nodes::Assignment* assignment, Nodes::Identifier* identifier)
	{
        std::string id = identifier->identifiers[0];
        auto result = assignment->expression->visit(this);
        for(int i=1; i<identifier->identifiers.size(); i++)
        {
            id = id + "." + identifier->identifiers[i];
        }
        if(this->variables.find(id) != this->variables.end())
        {
            this->variables.at(id)=result;
        }
        else
        {
            this->variables.insert(std::make_pair(id, result));
        }
        return nullptr;
	}

    Nodes::Object* TreeVisitor::visit(Nodes::AndExpression* andExpression)
	{
        auto left = andExpression->left->visit(this);
        if(!left->isTrue)
            return new Object(false);
        auto right = andExpression->right->visit(this);
        return new Object(left->isTrue && right->isTrue);
	}

    Nodes::Object* TreeVisitor::visit(Nodes::AddSubExpression* addSubExpression)
	{
        auto left = addSubExpression->left->visit(this);
        auto right = addSubExpression->right->visit(this);
        if(addSubExpression->token.type == ParserTokenType::Plus)
        {
            return new IntegerLiteral(left->value + right->value);
        }
        else if(addSubExpression->token.type == ParserTokenType::Minus)
        {
            return new IntegerLiteral(left->value - right->value);
        }
        return nullptr;
	}

    Nodes::Object* TreeVisitor::visit(Nodes::Identifier* identifier)
    {
        if(this->variables.find(identifier->identifiers[0])!=this->variables.end())
        {
            return this->variables.at(identifier->identifiers[0]);
        }
        else
        {
            return nullptr;
        }
    }
    Nodes::Object* TreeVisitor::visit(Nodes::Assignable* assignable)
    {
        ErrorHandler::error("something went wrong visit assigable");
    }
    Nodes::Object* TreeVisitor::visit(Nodes::Instruction* instruction)
    {
        std::cout<<"something went wrong - visit Instruction"<<std::endl;
    }
    Nodes::Object* TreeVisitor::visit(Nodes::Print*, std::vector<Nodes::Object*> expressions)
    {
        if(expressions.size()!=1)
        {
            ErrorHandler::error("print function requires one argument", false);
            return nullptr;
        }
        //auto result = expressions[0]->visit(this);
        if(expressions[0]->className!="")
        {
            ErrorHandler::error("cannot print class object", false);
            return nullptr;
        };
        if(expressions[0]->stringLiteral != "")
        {
            std::cout<<expressions[0]->stringLiteral;
            return nullptr;
        }
        std::cout<<expressions[0]->value;
        return nullptr;
    }
    Nodes::Object* TreeVisitor::visit(Nodes::StdLib *stdlib, std::vector<Nodes::Object *> expressions)
    {
        ErrorHandler::error("Something went wrong", false);
    }
    void TreeVisitor::loadStandardLibrary() {
        auto print = new Print();
        this->standarLibraryFunctions.insert(std::make_pair(print->name, print));
    }

    Nodes::Object* TreeVisitor::visit(BreakStatement *program)
    {
        return new BreakStatementLiteral();
    }
    Nodes::Object* TreeVisitor::visit(ContinueStatement *program)
    {
        return new ContinueStatementLiteral();
    }
}
