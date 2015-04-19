#ifndef _GRAM_COMP_H_
#define _GRAM_COMP_H_

#include <vector>

namespace GeneratorNodes
{
	class Node;
	class IdentifierNode;
	class LeftBracketNode;
	class RightBracketNode;
	class PlusNode;
	class MinusNode;
}

class GrammarCompactor
{
public:
	static void compact(std::vector<GeneratorNodes::Node*>& symbols);
	static void printSymbols(std::vector<GeneratorNodes::Node*>& symbols);
};
#endif