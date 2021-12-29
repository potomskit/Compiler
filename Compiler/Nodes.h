#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Token.h"
#include "Node.h"
namespace Nodes {
	class ClassBlock;
	class ClassDefinition;
	class StatementBlock;
	class Variable;
    class Identifier : public Node
    {
    public:
        virtual Type getType()
        {
            return Node::Type::Identifier;
        }
    	void addNameSpace(std::string name_space)
        {
            name_spaces.push_back(name_space);
        }
        std::vector<std::string> name_spaces;
    };
	class Assignable : public Node
    {
    public:
        typedef std::unique_ptr<Assignable> ptr;
    };

    class Call : public Assignable
    {
    public:
        void setIdentifier(std::unique_ptr <Identifier>& identifier)
        {
            this->identifier = identifier.();
        }
    	
        void addArgument(const std::unique_ptr<Assignable>& assignableNode)
        {
            this->arguments.push_back(assignableNode);
        }
    	
        virtual Type getType()
        {
            return Node::Type::Call;
        }
      
        std::unique_ptr<Identifier> identifier;
        std::vector<std::unique_ptr<Assignable>> arguments;
    };

    class Assignment : public Node
    {
    public:
        void setVariable(const std::unique_ptr<Variable>& variable)
        {
            this->variable = variable;
        }
        void setValue(const std::unique_ptr<Assignable>& value)
        {
            this->value = value;
        }

        virtual Type getType()
        {
            return Node::Type::Assignment;
        }

        std::unique_ptr<Variable> variable;
        std::unique_ptr<Assignable> value;
    };

    class Condition : public Node
    {
    public:
        void addOperand(const NodePtr& node)
        {
            this->operands.push_back(node);
        }
        void setOperator(const token_type& operation)
        {
            this->operation = operation;
        }
        void setNegated()
        {
            this->negated = true;
        }
        bool isNegated()
        {
            return this->negated;
        }
        NodePtr& getLeftSide()
        {
            return this->operands.at(0);
        }

        virtual Type getType()
        {
            return Node::Type::Condition;
        }

        bool negated = false;
        token_type operation = token_type::Undefined;
        std::vector<NodePtr> operands;
    };

    class Expression : public Assignable
    {
    public:
        void addOperand(const NodePtr& node)
        {
            this->operands.push_back(node);
        }
        void addOperator(const token_type& operation)
        {
            this->operations.push_back(operation);
        }
        virtual Type getType()
        {
            return Node::Type::Expression;
        }

        std::vector<token_type> operations;
        std::vector<NodePtr> operands;
    };

    class FunDefinition : public Node
    {
    public:
        void setName(const std::string& name)
        {
            this->name = name;
        }
        void setParameters(const std::vector<std::string>& parameters)
        {
            this->parameters = parameters;
        }
        void setBlock(const std::unique_ptr<StatementBlock>& blockNode)
        {
            this->blockNode = blockNode;
        }
        virtual Type getType()
        {
            return Node::Type::FunDefinition;
        }

        std::string name;
        std::vector<std::string> parameters;
        std::unique_ptr<StatementBlock> blockNode;
    };

    class IfStatement : public Node
    {
    public:
        void setCondition(const std::unique_ptr<Condition>& conditionNode)
        {
            this->conditionNode = conditionNode;
        }
        void setTrueBlock(const std::unique_ptr<StatementBlock>& trueBlockNode)
        {
            this->trueBlockNode = trueBlockNode;
        }
        void setFalseBlock(const std::unique_ptr<StatementBlock>& falseBlockNode)
        {
            this->falseBlockNode = falseBlockNode;
        }
    	void addElifBlock(const std::unique_ptr<StatementBlock> elifBlockNode)
        {
            this->elifBlockNodes.push_back(elifBlockNode);
        }
        virtual Type getType()
        {
            return Node::Type::IfStatement;
        }

        std::unique_ptr<Condition> conditionNode;
        std::unique_ptr<StatementBlock> trueBlockNode;
        std::vector<std::unique_ptr<StatementBlock>> elifBlockNodes;
        std::unique_ptr<StatementBlock> falseBlockNode;
    };

    class LoopJump : public Node
    {
    public:
        void setType(const token_type& type)
        {
            if (type == token_type::Break)
            {
                this->isBreak = true;
                return;
            }
            this->isBreak = false;
        }
        virtual Type getType()
        {
            return Node::Type::LoopJump;
        }

        bool isBreak;
    };

    class Program : public Node
    {
    public:
        void addFunction(const std::unique_ptr<FunDefinition>& function)
        {
            this->functions.push_back(function);
        }
        virtual Type getType()
        {
            return Node::Type::Program;
        }
        void addClass(const std::unique_ptr<ClassDefinition>& classPtr)
        {
            this->classes.push_back(classPtr);
        }
        std::vector<std::unique_ptr<FunDefinition>> functions = {};
        std::vector<std::unique_ptr<ClassDefinition>> classes = {};
    };

    class ReturnStatement : public Node
    {
    public:
        void setValue(const std::unique_ptr<Assignable>& assignableNode)
        {
            this->assignableNode = assignableNode;
        }
        virtual Type getType()
        {
            return Node::Type::ReturnStatement;
        }

        std::unique_ptr<Assignable> assignableNode;
    };

    class StatementBlock : public Node
    {
    public:
        void addInstruction(const NodePtr& node)
        {
            this->instructions.push_back(node);
        }
        virtual Type getType()
        {
            return Node::Type::StatementBlock;
        }

        std::vector<NodePtr> instructions;
    };

    class VarDeclaration : public Node
    {
    public:
        void setName(const std::string& name)
        {
            this->name = name;
        }
        void setValue(const std::unique_ptr<Assignable>& assignableNode)
        {
            this->assignableNode = assignableNode;
        }
        virtual Type getType()
        {
            return Node::Type::VarDeclaration;
        }

        std::string name;
        std::unique_ptr<Assignable> assignableNode;
    };

    class Variable : public Assignable
    {
    public:
        void setIdentifier(std::unique_ptr<Identifier> identifier)
        {
            this->identifier = identifier;
        }

        virtual Type getType()
        {
            return Node::Type::Variable;
        }
       
        std::unique_ptr<Identifier> identifier;
    };

    class WhileStatement : public Node
    {
    public:
        void setCondition(const std::unique_ptr<Condition>& conditionNode)
        {
            this->conditionNode = conditionNode;
        }
        void setBlock(const std::unique_ptr<StatementBlock>& blockNode)
        {
            this->blockNode = blockNode;
        }
        virtual Type getType()
        {
            return Node::Type::WhileStatement;
        }

        std::unique_ptr<Condition> conditionNode;
        std::unique_ptr<StatementBlock> blockNode;
    };

	class Literal : public Assignable
	{
        virtual Type getType()
        {
            return Node::Type::Literal;
        }
	};
	
    class StringLiteral : public Literal
    {
        void setValue(const int& value)
        {
            this->value = value;
        }
        virtual Type getType()
        {
            return Node::Type::StringLiteral;
        }
        std::string value;
    };
	
    class IntegerLiteral : public Literal
    {
    public:
        void setValue(const int& value)
        {
            this->value = value;
        }
       
        virtual Type getType()
        {
            return Node::Type::IntegerLiteral;
        }

        
        int value = 0;
    };

	class ClassDefinition: public Node
	{
	public:
        void setName(const std::string& name)
        {
            this->name = name;
        	
        }
        void setBlock(const std::unique_ptr<ClassBlock>& blockNode)
        {
            this->blockNode = blockNode;
        }
        virtual Type getType()
        {
            return Node::Type::ClassDefinition;
        }
        std::string name;
        std::unique_ptr<ClassBlock> blockNode;
	};

	class ClassBlock: public Node
	{
    public:
        void addVariable(const NodePtr& node)
        {
            this->variables.push_back(node);
        }
		void addFunction(const NodePtr& node)
        {
            this->functions.push_back(node);
        }
        virtual Type getType()
        {
            return Node::Type::ClassBlock;
        }

        std::vector<NodePtr> variables;
        std::vector<NodePtr> functions;
	};

}