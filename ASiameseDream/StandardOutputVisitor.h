#ifndef _STDO_VIS_H_
#define _STDO_VIS_H_

#include "NodeVisitor.h"

namespace GeneratorNodes
{
	class Node;
	class IdentifierNode;
	class LeftBracketNode;
	class RightBracketNode;
	class PlusNode;
	class MinusNode;
	class ProgramNode;
	class RuleNode;
	class TokenSequenceNode;
}

class StandardOutputVisitor : public NodeVisitor
{
public:
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