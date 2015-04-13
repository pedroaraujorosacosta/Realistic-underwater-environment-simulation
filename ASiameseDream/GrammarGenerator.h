#ifndef _GRAM_GEN_H_
#define _GRAM_GEN_H_

#include <vector>
#include "ProgramNode.h"

class GrammarGenerator
{
public:
	static void generate(const GeneratorNodes::ProgramNode& grammar, 
		std::vector<const GeneratorNodes::Node*>& symbols);
	static void printSymbols(std::vector<const GeneratorNodes::Node*>& symbols);
};
#endif