#include "LeftBracketNode.h"
#include <iostream>

namespace GeneratorNodes
{
	LeftBracketNode::LeftBracketNode() : tokenString("[")
	{

	}

	std::string LeftBracketNode::getName() const
	{
		return tokenString;
	}

	void LeftBracketNode::print() const
	{
		std::cout << tokenString;
	}

	void LeftBracketNode::accept(StandardOutputVisitor* rv)
	{
		rv->visit(this);
	}

	void LeftBracketNode::accept(RendererVisitor* rv)
	{
		rv->visit(this);
	}
}