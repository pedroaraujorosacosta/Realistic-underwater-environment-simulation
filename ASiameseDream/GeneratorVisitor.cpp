#include "GeneratorVisitor.h"
#include "Node.h"
#include "IdentifierNode.h"
#include "LeftBracketNode.h"
#include "RightBracketNode.h"
#include "PlusNode.h"
#include "MinusNode.h"
#include "TokenSequenceNode.h"

GeneratorVisitor::GeneratorVisitor(std::vector<GeneratorNodes::Node*>& symbols, Ogre::int32 age) :
	symbols(symbols), age(age)
{

}

void GeneratorVisitor::visit(GeneratorNodes::Node* node)
{
	node->accept(this);
}

void GeneratorVisitor::visit(GeneratorNodes::IdentifierNode* node)
{
	GeneratorNodes::IdentifierNode* newNode = new GeneratorNodes::IdentifierNode(*node);
	newNode->setAge(age);
	symbols.push_back(newNode);
}

void GeneratorVisitor::visit(GeneratorNodes::LeftBracketNode* node)
{
	GeneratorNodes::LeftBracketNode* newNode = new GeneratorNodes::LeftBracketNode(*node);
	newNode->setAge(age);
	symbols.push_back(newNode);
}

void GeneratorVisitor::visit(GeneratorNodes::RightBracketNode* node)
{
	GeneratorNodes::RightBracketNode* newNode = new GeneratorNodes::RightBracketNode(*node);
	newNode->setAge(age);
	symbols.push_back(newNode);
}

void GeneratorVisitor::visit(GeneratorNodes::PlusNode* node)
{
	GeneratorNodes::PlusNode* newNode = new GeneratorNodes::PlusNode(*node);
	newNode->setAge(age);
	symbols.push_back(newNode);
}

void GeneratorVisitor::visit(GeneratorNodes::MinusNode* node)
{
	GeneratorNodes::MinusNode* newNode = new GeneratorNodes::MinusNode(*node);
	newNode->setAge(age);
	symbols.push_back(newNode);
}

void GeneratorVisitor::visit(GeneratorNodes::ProgramNode* node)
{

}

void GeneratorVisitor::visit(GeneratorNodes::RuleNode* node)
{

}

void GeneratorVisitor::visit(GeneratorNodes::TokenSequenceNode* node)
{
	std::vector<GeneratorNodes::Node*> sequence = node->getSequence();
	for (std::vector<GeneratorNodes::Node*>::iterator it = sequence.begin(); it != sequence.end(); it++)
		visit(*it);
}