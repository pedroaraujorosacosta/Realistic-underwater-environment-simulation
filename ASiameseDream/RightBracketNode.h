#ifndef _RIGHT_NODE_H_
#define _RIGHT_NODE_H_

#include "Node.h"

namespace GeneratorNodes
{
	class RightBracketNode : public Node
	{
		const std::string tokenString;
	public:
		RightBracketNode();

		std::string getName() const;

		virtual void print() const;

		virtual void accept(StandardOutputVisitor* rv);
		virtual void accept(RendererVisitor* rv);
	};
}
#endif