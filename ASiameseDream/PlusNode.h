#ifndef _PLUS_NODE_H_
#define _PLUS_NODE_H_

#include "Node.h"

namespace GeneratorNodes
{
	class PlusNode : public Node
	{
		const std::string tokenString;
	public:
		PlusNode();

		std::string getName() const;

		virtual void print() const;

		virtual void accept(RendererVisitor* rv) const;
		virtual void accept(StandardOutputVisitor* stdo) const;
	};
}
#endif