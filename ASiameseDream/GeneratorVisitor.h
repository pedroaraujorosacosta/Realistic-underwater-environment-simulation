#ifndef _GEN_VIS_H_
#define _GEN_VIS_H_

#include "NodeVisitor.h"

namespace GeneratorNodes
{
	class Node;
	class IdentifierNode;
	class LeftBracketNode;
	class RightBracketNode;
	class PlusNode;
	class MinusNode;
	class TokenSequenceNode;
}

class GeneratorVisitor : public NodeVisitor
{
	std::vector<GeneratorNodes::Node*>& symbols;
	Ogre::int32 age;
public:
	GeneratorVisitor(std::vector<GeneratorNodes::Node*>& symbols, Ogre::int32 age);

	virtual void visit(GeneratorNodes::Node* node);
	virtual void visit(GeneratorNodes::IdentifierNode* node);
	virtual void visit(GeneratorNodes::LeftBracketNode* node);
	virtual void visit(GeneratorNodes::RightBracketNode* node);
	virtual void visit(GeneratorNodes::PlusNode* node);
	virtual void visit(GeneratorNodes::MinusNode* node);
	virtual void visit(GeneratorNodes::ProgramNode* node);
	virtual void visit(GeneratorNodes::RuleNode* node);
	virtual void visit(GeneratorNodes::TokenSequenceNode* node);
};
#endif