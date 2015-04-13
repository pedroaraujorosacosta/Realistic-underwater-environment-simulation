#include "StandardOutputVisitor.h"
#include "Node.h"

using namespace GeneratorNodes;

void StandardOutputVisitor::visit(Node* node)
{
	node->accept(this);
}