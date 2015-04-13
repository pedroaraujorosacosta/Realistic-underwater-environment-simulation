#ifndef _SEQ_NODE_H_
#define _SEQ_NODE_H_

#include <vector>
#include "Node.h"

namespace GeneratorNodes
{
	class TokenSequenceNode : public Node
	{
		std::vector<Node*> sequence;
	public:
		TokenSequenceNode(std::vector<Node*>& sequence);
		~TokenSequenceNode();

		std::vector<Node*> getSequence() const;
		std::string getName() const;

		virtual void print() const;

		virtual void accept(RendererVisitor* rv) const;
		virtual void accept(StandardOutputVisitor* stdo) const;
	};
}
#endif