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

	class Assignable : public Node
    {
    public:
        typedef std::shared_ptr<Assignable> ptr;
    };

    class Call : public Assignable
    {
    public:
        void setName(const std::string& name)
        {
            this->name = name;
        }
        void addArgument(const std::shared_ptr<Assignable>& assignableNode)
        {
            this->arguments.push_back(assignableNode);
        }
        virtual Type getType()
        {
            return Node::Type::Call;
        }
        void setObjectName(const std:: string objectName)
        {
            this->objectName = objectName;
        }
        std::string objectName;
        std::string name;
        std::vector<std::shared_ptr<Assignable>> arguments;
    };

    class Assignment : public Node
    {
    public:
        void setVariable(const std::shared_ptr<Variable>& variable)
        {
            this->variable = variable;
        }
        void setValue(const std::shared_ptr<Assignable>& value)
        {
            this->value = value;
        }

        virtual Type getType()
        {
            return Node::Type::Assignment;
        }

        std::shared_ptr<Variable> variable;
        std::shared_ptr<Assignable> value;
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
        void setBlock(const std::shared_ptr<StatementBlock>& blockNode)
        {
            this->blockNode = blockNode;
        }
        virtual Type getType()
        {
            return Node::Type::FunDefinition;
        }

        std::string name;
        std::vector<std::string> parameters;
        std::shared_ptr<StatementBlock> blockNode;
    };

    class IfStatement : public Node
    {
    public:
        void setCondition(const std::shared_ptr<Condition>& conditionNode)
        {
            this->conditionNode = conditionNode;
        }
        void setTrueBlock(const std::shared_ptr<StatementBlock>& trueBlockNode)
        {
            this->trueBlockNode = trueBlockNode;
        }
        void setFalseBlock(const std::shared_ptr<StatementBlock>& falseBlockNode)
        {
            this->falseBlockNode = falseBlockNode;
        }
        virtual Type getType()
        {
            return Node::Type::IfStatement;
        }

        std::shared_ptr<Condition> conditionNode;
        std::shared_ptr<StatementBlock> trueBlockNode;
        std::shared_ptr<StatementBlock> falseBlockNode;
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
        void addFunction(const std::shared_ptr<FunDefinition>& function)
        {
            this->functions.push_back(function);
        }
        virtual Type getType()
        {
            return Node::Type::Program;
        }
        void addClass(const std::shared_ptr<ClassDefinition>& classPtr)
        {
            this->classes.push_back(classPtr);
        }
        std::vector<std::shared_ptr<FunDefinition>> functions = {};
        std::vector<std::shared_ptr<ClassDefinition>> classes = {};
    };

    class ReturnStatement : public Node
    {
    public:
        void setValue(const std::shared_ptr<Assignable>& assignableNode)
        {
            this->assignableNode = assignableNode;
        }
        virtual Type getType()
        {
            return Node::Type::ReturnStatement;
        }

        std::shared_ptr<Assignable> assignableNode;
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
        void setValue(const std::shared_ptr<Assignable>& assignableNode)
        {
            this->assignableNode = assignableNode;
        }
        virtual Type getType()
        {
            return Node::Type::VarDeclaration;
        }

        std::string name;
        std::shared_ptr<Assignable> assignableNode;
    };

    class Variable : public Assignable
    {
    public:
        void setName(const std::string& name)
        {
            this->variableName = name;
        }

        virtual Type getType()
        {
            return Node::Type::Variable;
        }
        void setObjectName(const std::string& objectName)
        {
            this->objectName = objectName;
        }
        std::string variableName;
        std::string objectName;
    };

    class WhileStatement : public Node
    {
    public:
        void setCondition(const std::shared_ptr<Condition>& conditionNode)
        {
            this->conditionNode = conditionNode;
        }
        void setBlock(const std::shared_ptr<StatementBlock>& blockNode)
        {
            this->blockNode = blockNode;
        }
        virtual Type getType()
        {
            return Node::Type::WhileStatement;
        }

        std::shared_ptr<Condition> conditionNode;
        std::shared_ptr<StatementBlock> blockNode;
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
        void setBlock(const std::shared_ptr<ClassBlock>& blockNode)
        {
            this->blockNode = blockNode;
        }
        virtual Type getType()
        {
            return Node::Type::ClassDefinition;
        }
        std::string name;
        std::shared_ptr<ClassBlock> blockNode;
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