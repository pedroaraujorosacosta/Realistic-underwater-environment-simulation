#include "MinusNode.h"
#include <iostream>

namespace GeneratorNodes
{
	MinusNode::MinusNode() : tokenString("-")
	{

	}

	std::string MinusNode::getName() const
	{
		return tokenString;
	}

	void MinusNode::print() const
	{
		std::cout << tokenString;
	}

	void MinusNode::accept(StandardOutputVisitor* rv)
	{
		rv->visit(this);
	}

	void MinusNode::accept(RendererVisitor* rv)
	{
		rv->visit(this);
	}
}