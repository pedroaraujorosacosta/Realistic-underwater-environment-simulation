#include "IdentifierNode.h"
#include <iostream>

namespace GeneratorNodes
{
	IdentifierNode::IdentifierNode(const std::string& tokenString)
	{
		name = tokenString;
	}

	std::string IdentifierNode::getName() const
	{
		return name;
	}

	void IdentifierNode::print() const
	{
		std::cout << name;
	}

	void IdentifierNode::accept(RendererVisitor* rv) const
	{
		
	}

	void IdentifierNode::accept(StandardOutputVisitor* stdov) const
	{
		std::cout << name;
	}
}