#include "RuleNode.h"
#include <iostream>
#include <sstream>

namespace GeneratorNodes
{
	RuleNode::RuleNode(Node* antecedent, Node* consequent) : RULE_STRING(":=")
	{
		this->antecedent = antecedent;
		this->consequent = consequent;
	}

	RuleNode::~RuleNode()
	{
		delete antecedent;
		delete consequent;
	}

	Node* RuleNode::getConsequent() const
	{
		return consequent;
	}

	Node* RuleNode::getAntecedent() const
	{
		return antecedent;
	}
	
	std::string RuleNode::getName() const
	{
		std::ostringstream oss("", std::ostringstream::ate);
		oss << antecedent->getName() << " " << RULE_STRING << " " << consequent->getName();
		return oss.str();
	}

	std::string RuleNode::getRuleString() const
	{
		return RULE_STRING;
	}

	void RuleNode::print() const
	{
		antecedent->print();
		std::cout << " " << RULE_STRING << " ";
		consequent->print();
	}

	void RuleNode::accept(StandardOutputVisitor* rv)
	{
		rv->visit(this);
	}

	void RuleNode::accept(RendererVisitor* rv)
	{
		rv->visit(this);
	}
}