#include <iostream>
#include "GrammarGenerator.h"
#include "TokenSequenceNode.h"

using namespace GeneratorNodes;

void GrammarGenerator::generate(const ProgramNode& grammar, std::vector<Node*>& symbols)
{
	std::vector<Node*> out;

	size_t i = 0;
	for (std::vector<Node*>::const_iterator it = symbols.begin(); it != symbols.end(); it++)
	{
		const Node* consequent = grammar.getConsequent(*it);
		if (consequent)
		{
			//std::cout << consequent->getName() << std::endl;

			std::vector<Node*> consElements = ((TokenSequenceNode*)consequent)->getSequence();

			for (std::vector<Node*>::const_iterator consIt = consElements.begin(); consIt != consElements.end(); consIt++)
				out.push_back(*consIt);
		}
		else
			out.push_back(*it);
	}

	symbols = out;
}

void GrammarGenerator::printSymbols(std::vector<GeneratorNodes::Node*>& symbols)
{
	for (std::vector<Node*>::const_iterator it = symbols.begin(); it != symbols.end(); it++)
		std::cout << (*it)->getName() << std::endl;
}