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

	void PlusNode::accept(RendererVisitor* rv) const
	{
		
	}

	void PlusNode::accept(StandardOutputVisitor* stdov) const
	{
		std::cout << tokenString;
	}
}