#ifndef _ID_NODE_H_
#define _ID_NODE_H_

#include "Node.h"

namespace GeneratorNodes
{
	class IdentifierNode : public Node
	{
		std::string name;
	public:
		IdentifierNode(const std::string& tokenString);

		virtual std::string getName() const;

		virtual void print() const;

		virtual void accept(RendererVisitor* rv) const;

		virtual void accept(StandardOutputVisitor* stdo) const;
	};
}
#endif