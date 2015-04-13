#include "RendererVisitor.h"
#include "Node.h"

using namespace GeneratorNodes;

void RendererVisitor::visit(Node* node)
{
	node->accept(this);
}