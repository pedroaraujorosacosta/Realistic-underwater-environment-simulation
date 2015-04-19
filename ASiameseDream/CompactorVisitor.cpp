#include "CompactorVisitor.h"
#include "Node.h"
#include "IdentifierNode.h"
#include "LeftBracketNode.h"
#include "RightBracketNode.h"
#include "PlusNode.h"
#include "MinusNode.h"

CompactorVisitor::CompactorVisitor(std::vector<GeneratorNodes::Node*>& symbols, Ogre::int32 numInstances) :
	symbols(symbols), numInstances(numInstances)
{

}

void CompactorVisitor::visit(GeneratorNodes::Node* node)
{
	node->accept(this);
}

void CompactorVisitor::visit(GeneratorNodes::IdentifierNode* node)
{
	GeneratorNodes::IdentifierNode* newNode = new GeneratorNodes::IdentifierNode(*node);
	newNode->setNumInstances(numInstances);
	symbols.push_back(newNode);
}

void CompactorVisitor::visit(GeneratorNodes::LeftBracketNode* node)
{
	GeneratorNodes::LeftBracketNode* newNode = new GeneratorNodes::LeftBracketNode(*node);
	newNode->setNumInstances(numInstances);
	symbols.push_back(newNode);
}

void CompactorVisitor::visit(GeneratorNodes::RightBracketNode* node)
{
	GeneratorNodes::RightBracketNode* newNode = new GeneratorNodes::RightBracketNode(*node);
	newNode->setNumInstances(numInstances);
	symbols.push_back(newNode);
}

void CompactorVisitor::visit(GeneratorNodes::PlusNode* node)
{
	GeneratorNodes::PlusNode* newNode = new GeneratorNodes::PlusNode(*node);
	newNode->setNumInstances(numInstances);
	symbols.push_back(newNode);
}

void CompactorVisitor::visit(GeneratorNodes::MinusNode* node)
{
	GeneratorNodes::MinusNode* newNode = new GeneratorNodes::MinusNode(*node);
	newNode->setNumInstances(numInstances);
	symbols.push_back(newNode);
}

void CompactorVisitor::visit(GeneratorNodes::ProgramNode* node)
{

}

void CompactorVisitor::visit(GeneratorNodes::RuleNode* node)
{

}

void CompactorVisitor::visit(GeneratorNodes::TokenSequenceNode* node)
{

}