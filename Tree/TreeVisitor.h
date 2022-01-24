#pragma once
#include "Nodes.h"
#include "../interpreter/StandardLib.h"

namespace Nodes {
	class AddSubExpression;
	class AndExpression;
	class Assignment;
	class Call;
	class ComparisonExpression;
	class Condition;
	class FunDefinition;
	class IdentifierAssigmentOrCall;
	class IfStatement;
	class IntegerLiteral;
	class Program;
	class Object;
	class ClassDefinition;
	class ClassBlock;
	class LogicNegationExpression;
	class WhileStatement;
	class MulDivExpression;
	class Variable;
	class OrExpression;
	class ParametersList;
	class ReturnStatement;
	class PrimaryExpression;
	class StringLiteral;
	class StatementBlock;
	class UnaryExpression;
	class VarAssigment;
	class VarAssignmentOrFuncCall;
    class Identifier;
    class Assignable;
    class Expression;
    class Instruction;
    class Arguments;
    class Print;
    class StdLib;
    class BreakStatement;
    class ContinueStatement;
//    class Object{
//
//    };
//    class ClassObject: public Object
//    {
//        std::string objectIdentifer;
//        Nodes::FunDefinition* functions;
//        std::vector<Nodes::Object> variables;
//    };
//
//    class IntObject: public Object{
//        int value;
//    };

	class TreeVisitor
	{
    private:
        std::unordered_map<std::string, Nodes::ClassDefinition*> classDefinitions;
        FunDefinition* main;
        std::unordered_map<std::string, Nodes::StdLib*> standarLibraryFunctions;
        std::unordered_map<std::string, Nodes::Object*> variables;
        void loadStandardLibrary();
	public:
        Nodes::Object* visit(Program* program);
        Nodes::Object* visit(ClassDefinition* classDef);
        Nodes::Object* visit(ClassBlock* classBlock);
        Nodes::Object* visit(WhileStatement* whileStatement);
        Nodes::Object* visit(Variable* classBlock);
        Nodes::Object* visit(VarAssignmentOrFuncCall* classBlock, Identifier* identifier);
        Nodes::Object* visit(VarAssigment* classBlock);
        Nodes::Object* visit(UnaryExpression* classBlock);
        Nodes::Object* visit(Nodes::StringLiteral* classBlock);
        Nodes::Object* visit(Nodes::StatementBlock* classBlock);
        Nodes::Object* visit(Nodes::PrimaryExpression* classBlock);
        Nodes::Object* visit(Nodes::ReturnStatement* classBlock);
        Nodes::Object* visit(Nodes::ParametersList* classBlock);
        Nodes::Object* visit(Nodes::OrExpression* classBlock);
        Nodes::Object* visit(Nodes::MulDivExpression* classBlock);
        Nodes::Object* visit(Nodes::LogicNegationExpression* classBlock);
        Nodes::Object* visit(Nodes::Object* classBlock);
        Nodes::Object* visit(Nodes::IntegerLiteral* classBlock);
        Nodes::Object* visit(Nodes::IfStatement* classBlock);
        Nodes::Object* visit(Nodes::IdentifierAssigmentOrCall* classBlock);
        Nodes::Object* visit(Nodes::FunDefinition* classBlock);
        Nodes::Object* visit(Nodes::Condition* classBlock);
        Nodes::Object* visit(Nodes::ComparisonExpression* classBlock);
        Nodes::Object* visit(Nodes::Call* classBlock, Nodes::Identifier* identifier);
        Nodes::Object* visit(Nodes::Assignment* classBlock, Nodes::Identifier* identifier);
        Nodes::Object* visit(Nodes::AndExpression* andExpression);
        Nodes::Object* visit(Nodes::AddSubExpression* addSubExpression);
        Nodes::Object* visit(Nodes::Identifier* identifier);
        Nodes::Object* visit(Nodes::Assignable* assignable);
        Nodes::Object* visit(Nodes::Instruction* instruction);
        Nodes::Object* visit(Nodes::FunDefinition* funDef, std::unordered_map<std::string, Object*> variables, std::string variableName);
        Nodes::Object* visit(Nodes::Arguments* arguments);
        Nodes::Object* visit(Nodes::Print* print, std::vector<Nodes::Object*> expressions);
        Nodes::Object* visit(Nodes::StdLib* stdlib, std::vector<Nodes::Object*> expressions);
        Nodes::Object* visit(Nodes::BreakStatement* breakStatement);
        Nodes::Object* visit(Nodes::ContinueStatement* continueStatement);
	};
}