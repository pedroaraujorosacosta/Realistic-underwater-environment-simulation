#ifndef _REND_VIS_H_
#define _REND_VIS_H_

#include "NodeVisitor.h"

class RendererVisitor : public NodeVisitor
{
public:
	virtual void visit(GeneratorNodes::Node* node);
};
#endif