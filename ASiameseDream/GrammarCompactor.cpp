#include "GrammarCompactor.h"
#include "IdentifierNode.h"
#include "LeftBracketNode.h"
#include "RightBracketNode.h"
#include "PlusNode.h"
#include "MinusNode.h"
#include "Node.h"
#include "CompactorVisitor.h"

void GrammarCompactor::compact(std::vector<GeneratorNodes::Node*>& symbols)
{
	std::vector<GeneratorNodes::Node*> aux(symbols);
	
	symbols.clear();
	std::vector<GeneratorNodes::Node*>::const_iterator it = aux.begin();
	Ogre::int32 offset;
	while (it != aux.end())
	{
		offset = 1;
		while (it + offset != aux.end() && (*it)->getName() == (*(it + offset))->getName())
			offset++;

		CompactorVisitor compVisitor(symbols, offset);
		(*it)->accept(&compVisitor);
		it += offset;
	}
}

void GrammarCompactor::printSymbols(std::vector<GeneratorNodes::Node*>& symbols)
{
	for (std::vector<GeneratorNodes::Node*>::const_iterator it = symbols.begin(); it != symbols.end(); it++)
		std::cout << (*it)->getName() << std::endl;
}