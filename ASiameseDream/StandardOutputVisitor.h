#ifndef _STDO_VIS_H_
#define _STDO_VIS_H_

#include "NodeVisitor.h"

class StandardOutputVisitor : public NodeVisitor
{
public:
	virtual void visit(GeneratorNodes::Node* node);
};
#endif