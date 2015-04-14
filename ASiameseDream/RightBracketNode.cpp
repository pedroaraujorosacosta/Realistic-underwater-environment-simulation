#include "RightBracketNode.h"
#include <iostream>

namespace GeneratorNodes
{
	RightBracketNode::RightBracketNode() : tokenString("]")
	{

	}

	std::string RightBracketNode::getName() const
	{
		return tokenString;
	}

	void RightBracketNode::print() const
	{
		std::cout << tokenString;
	}

	void RightBracketNode::accept(StandardOutputVisitor* rv)
	{
		rv->visit(this);
	}

	void RightBracketNode::accept(RendererVisitor* rv)
	{
		rv->visit(this);
	}
}