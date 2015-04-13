#ifndef _NODE_VISITOR_H_
#define _NODE_VISITOR_H_

namespace GeneratorNodes
{
	class Node;
}

class NodeVisitor {
public:
	virtual void visit(GeneratorNodes::Node* node) = 0;
};
#endif