#ifndef _LEFT_NODE_H_
#define _LEFT_NODE_H_

#include "Node.h"

namespace GeneratorNodes 
{
	class LeftBracketNode : public Node
	{
		const std::string tokenString;
	public:
		LeftBracketNode();

		std::string getName() const;

		virtual void print() const;

		virtual void accept(RendererVisitor* rv) const;
		virtual void accept(StandardOutputVisitor* stdo) const;
	};
}
#endif