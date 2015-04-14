#include "PlusNode.h"
#include <iostream>

namespace GeneratorNodes
{
	PlusNode::PlusNode() : tokenString("+")
	{

	}

	std::string PlusNode::getName() const
	{
		return tokenString;
	}

	void PlusNode::print() const
	{
		std::cout << tokenString;
	}

	void PlusNode::accept(StandardOutputVisitor* rv)
	{
		rv->visit(this);
	}

	void PlusNode::accept(RendererVisitor* rv)
	{
		rv->visit(this);
	}
}