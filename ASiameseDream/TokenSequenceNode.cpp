#include "TokenSequenceNode.h"
#include <iostream>
#include <sstream>

namespace GeneratorNodes
{
	TokenSequenceNode::TokenSequenceNode(std::vector<Node*>& sequence)
	{
		this->sequence = sequence;
	}

	TokenSequenceNode::~TokenSequenceNode()
	{
		for (std::vector<Node*>::const_iterator it = sequence.end() - 1; it != sequence.begin() - 1; it--)
		{
			sequence.erase(it);
			delete *it;
		}
	}

	std::vector<Node*> TokenSequenceNode::getSequence() const
	{
		return sequence;
	}

	std::string TokenSequenceNode::getName() const
	{
		std::ostringstream oss("", std::ostringstream::ate);
		
		if (!sequence.empty())
		{
			oss << sequence[0]->getName();
			std::vector<Node*>::const_iterator it = sequence.begin() + 1;
			while (it != sequence.end())
			{
				oss << " " << (*it)->getName();
				it++;
			}
		}

		return oss.str();
	}

	void TokenSequenceNode::print() const
	{
		for (std::vector<Node*>::const_iterator it = sequence.begin(); it != sequence.end(); it++)
		{
			(*it)->print();
			std::cout << " ";
		}
	}

	void TokenSequenceNode::accept(StandardOutputVisitor* rv)
	{
		rv->visit(this);
	}

	void TokenSequenceNode::accept(RendererVisitor* rv)
	{
		rv->visit(this);
	}
}