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

	void IdentifierNode::accept(StandardOutputVisitor* rv)
	{
		rv->visit(this);
	}
	
	void IdentifierNode::accept(RendererVisitor* rv)
	{
		rv->visit(this);
	}
}