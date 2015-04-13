#include "RuleNode.h"
#include <iostream>
#include <sstream>

namespace GeneratorNodes
{
	RuleNode::RuleNode(const Node* antecedent, const Node* consequent) : RULE_STRING(":=")
	{
		this->antecedent = antecedent;
		this->consequent = consequent;
	}

	RuleNode::~RuleNode()
	{
		delete antecedent;
		delete consequent;
	}

	const Node* RuleNode::getConsequent() const
	{
		return consequent;
	}

	const Node* RuleNode::getAntecedent() const
	{
		return antecedent;
	}
	
	std::string RuleNode::getName() const
	{
		std::ostringstream oss("", std::ostringstream::ate);
		oss << antecedent->getName() << " " << RULE_STRING << " " << consequent->getName();
		return oss.str();
	}

	void RuleNode::print() const
	{
		antecedent->print();
		std::cout << " " << RULE_STRING << " ";
		consequent->print();
	}

	void RuleNode::accept(RendererVisitor* rv) const
	{
		
	}

	void RuleNode::accept(StandardOutputVisitor* stdov) const
	{
		antecedent->accept(stdov);
		std::cout << " " << RULE_STRING << " ";
		consequent->accept(stdov);
	}
}