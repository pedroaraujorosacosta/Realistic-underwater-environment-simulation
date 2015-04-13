#ifndef _PROGRAM_NODE_H_
#define _PROGRAM_NODE_H_

#include <vector>
#include "Node.h"

namespace GeneratorNodes
{
	class ProgramNode : public Node
	{
		std::vector<Node*> sequence;
	public:
		ProgramNode(std::vector<Node*>& sequence);
		~ProgramNode();

		std::vector<Node*> getSequence() const;
		const Node* getConsequent(const Node* antecedent) const;
		std::string getName() const;

		virtual void print() const;

		virtual void accept(RendererVisitor* rv) const;
		virtual void accept(StandardOutputVisitor* stdo) const;
	};
}
#endif