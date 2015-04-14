#ifndef _REND_VIS_H_
#define _REND_VIS_H_

#include <stack>
#include "NodeVisitor.h"

namespace GeneratorNodes
{
	class Node;
	class IdentifierNode;
	class LeftBracketNode;
	class RightBracketNode;
	class PlusNode;
	class MinusNode;
	class ProgramNode;
	class RuleNode;
	class TokenSequenceNode;
}

class RendererVisitor : public NodeVisitor
{
	Ogre::SceneNode*				sceneNode;
	Ogre::SceneManager*				sceneMgr;
	Ogre::Vector3					position;
	Ogre::Real						angle;
	Ogre::MaterialPtr				mat;
	std::stack<Ogre::SceneNode*>	sceneNodes;

public:
	RendererVisitor(Ogre::SceneManager* sceneMgr, Ogre::SceneNode* sceneNode, std::string name, Ogre::Real angle);
	RendererVisitor(Ogre::SceneManager* sceneMgr, Ogre::SceneNode* sceneNode, const Ogre::Vector3& position,
		std::string name, Ogre::Real angle);

	virtual void visit(GeneratorNodes::Node* node);
	virtual void visit(GeneratorNodes::IdentifierNode* node);
	virtual void visit(GeneratorNodes::LeftBracketNode* node);
	virtual void visit(GeneratorNodes::RightBracketNode* node);
	virtual void visit(GeneratorNodes::PlusNode* node);
	virtual void visit(GeneratorNodes::MinusNode* node);
	virtual void visit(GeneratorNodes::ProgramNode* node);
	virtual void visit(GeneratorNodes::RuleNode* node);
	virtual void visit(GeneratorNodes::TokenSequenceNode* node);
};
#endif