#ifndef _MINUS_NODE_H_
#define _MINUS_NODE_H_

#include "Node.h"

namespace GeneratorNodes
{
	class MinusNode : public Node
	{
		const std::string tokenString;
	public:
		MinusNode();

		std::string getName() const;

		virtual void print() const;

		virtual void accept(RendererVisitor* rv) const;
		virtual void accept(StandardOutputVisitor* stdo) const;
	};
}
#endif