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

	void MinusNode::accept(RendererVisitor* rv) const
	{
		
	}

	void MinusNode::accept(StandardOutputVisitor* stdov) const
	{
		std::cout << tokenString;
	}
}