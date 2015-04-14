#ifndef _RULE_NODE_H_
#define _RULE_NODE_H_

#include <vector>
#include "Node.h"

namespace GeneratorNodes
{
	class RuleNode : public Node
	{
		const std::string RULE_STRING;
		Node* antecedent;
		Node* consequent;
	public:
		RuleNode(Node* antecedent, Node* consequent);
		~RuleNode();

		Node* getConsequent() const;
		Node* getAntecedent() const;
		std::string getRuleString() const;
		std::string getName() const;

		virtual void print() const;

		virtual void accept(StandardOutputVisitor* rv);
		virtual void accept(RendererVisitor* rv);
	};
}
#endif