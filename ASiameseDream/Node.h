#ifndef _NODE_H_
#define _NODE_H_

#include <string>
#include "NodeVisitor.h"
#include "RendererVisitor.h"
#include "StandardOutputVisitor.h"

namespace GeneratorNodes
{
	class Node
	{
	public:
		Node() {}

		virtual std::string getName() const { return ""; }
		virtual void print() const = 0;

		virtual void accept(RendererVisitor* rv) const = 0;
		virtual void accept(StandardOutputVisitor* stdov) const = 0;
	};
}
#endif