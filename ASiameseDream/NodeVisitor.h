#ifndef _NODE_VISITOR_H_
#define _NODE_VISITOR_H_

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

class NodeVisitor {
public:
	virtual void visit(GeneratorNodes::Node* node) = 0;
	virtual void visit(GeneratorNodes::IdentifierNode* node) = 0;
	virtual void visit(GeneratorNodes::LeftBracketNode* node) = 0;
	virtual void visit(GeneratorNodes::RightBracketNode* node) = 0;
	virtual void visit(GeneratorNodes::PlusNode* node) = 0;
	virtual void visit(GeneratorNodes::MinusNode* node) = 0;
	virtual void visit(GeneratorNodes::ProgramNode* node) = 0;
	virtual void visit(GeneratorNodes::RuleNode* node) = 0;
	virtual void visit(GeneratorNodes::TokenSequenceNode* node) = 0;
};
#endif