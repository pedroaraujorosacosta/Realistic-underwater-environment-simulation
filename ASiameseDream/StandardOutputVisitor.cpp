#include <iostream>
#include "StandardOutputVisitor.h"
#include "AllNodes.h"

using namespace GeneratorNodes;

void StandardOutputVisitor::visit(Node* node)
{
	node->accept(this);
}

void StandardOutputVisitor::visit(IdentifierNode* node)
{
	std::cout << node->getName();
}

void StandardOutputVisitor::visit(LeftBracketNode* node)
{
	std::cout << node->getName();
}

void StandardOutputVisitor::visit(RightBracketNode* node)
{
	std::cout << node->getName();
}

void StandardOutputVisitor::visit(PlusNode* node)
{
	std::cout << node->getName();
}

void StandardOutputVisitor::visit(MinusNode* node)
{
	std::cout << node->getName();
}

void StandardOutputVisitor::visit(ProgramNode* node)
{
	std::vector<Node*> sequence = node->getSequence();
	for (std::vector<Node*>::const_iterator it = sequence.begin(); it != sequence.end(); it++)
	{
		(*it)->accept(this);
		std::cout << std::endl;
	}
}

void StandardOutputVisitor::visit(RuleNode* node)
{
	node->getAntecedent()->accept(this);
	std::cout << " " << node->getRuleString() << " ";
	node->getConsequent()->accept(this);
}

void StandardOutputVisitor::visit(TokenSequenceNode* node)
{
	std::vector<Node*> sequence = node->getSequence();
	for (std::vector<Node*>::const_iterator it = sequence.begin(); it != sequence.end(); it++)
	{
		(*it)->accept(this);
		std::cout << " ";
	}
}