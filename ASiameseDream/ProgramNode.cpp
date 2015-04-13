#include "ProgramNode.h"
#include "RuleNode.h"
#include <iostream>
#include <sstream>

namespace GeneratorNodes
{
	ProgramNode::ProgramNode(std::vector<Node*>& sequence)
	{
		this->sequence = sequence;
	}

	ProgramNode::~ProgramNode()
	{
		if (sequence.size())
		{
			for (std::vector<Node*>::const_iterator it = sequence.end() - 1; it != sequence.begin(); it--)
				delete *it;
			
			sequence.clear();
		}
	}

	std::vector<Node*> ProgramNode::getSequence() const
	{
		return sequence;
	}

	const Node* ProgramNode::getConsequent(const Node* antecedent) const
	{
		for (std::vector<Node*>::const_iterator it = sequence.begin(); it != sequence.end(); it++)
		{
			RuleNode* rule = (RuleNode*)(*it);
			const Node* ruleAnt = rule->getAntecedent();
			const Node* ruleCons = rule->getConsequent();
			
			if (ruleAnt->getName() == antecedent->getName())
			{
				return ruleCons;
			}
		}

		return 0;
	}

	std::string ProgramNode::getName() const
	{
		std::ostringstream oss("", std::ostringstream::ate);

		if (!sequence.empty())
		{
			oss << sequence[0]->getName();
			std::vector<Node*>::const_iterator it = sequence.begin() + 1;
			while (it != sequence.end())
			{
				oss << std::endl << (*it)->getName();
				it++;
			}
		}

		return oss.str();
	}

	void ProgramNode::print() const
	{
		for (std::vector<Node*>::const_iterator it = sequence.begin(); it != sequence.end(); it++)
		{
			(*it)->print();
			std::cout << std::endl;
		}
	}

	void ProgramNode::accept(RendererVisitor* rv) const
	{
		
	}

	void ProgramNode::accept(StandardOutputVisitor* stdov) const
	{
		for (std::vector<Node*>::const_iterator it = sequence.begin(); it != sequence.end(); it++)
		{
			(*it)->accept(stdov);
			std::cout << std::endl;
		}
	}
}