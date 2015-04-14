#include "RendererVisitor.h"
#include "AllNodes.h"
#include "MovableText.h"

using namespace GeneratorNodes;

RendererVisitor::RendererVisitor(Ogre::SceneManager* sceneMgr, Ogre::SceneNode* sceneNode, std::string name,
	Ogre::Real angle)
{
	RendererVisitor(sceneMgr, sceneNode, Ogre::Vector3(0.0f, 0.0f, 0.0f), name, angle);
}

RendererVisitor::RendererVisitor(Ogre::SceneManager* sceneMgr, Ogre::SceneNode* sceneNode, 
	const Ogre::Vector3& position, std::string name, Ogre::Real angle) : sceneMgr(sceneMgr), sceneNode(sceneNode),
	position(position), angle(angle)
{
	sceneNode->setPosition(position);
	sceneNode->setScale(0.01f, 0.01f, 0.01f);

	Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create(Ogre::String("mat").c_str(),
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	mat->setAmbient(1.0f, 1.0f, 1.0f);
	mat->setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	mat->setSpecular(0.0f, 0.0f, 0.0f, 1.0f);

	Ogre::MovableText* msg = new Ogre::MovableText("TXT_001", name, "BlueHighway-8", 8.0f);
	msg->setTextAlignment(Ogre::MovableText::H_CENTER, Ogre::MovableText::V_BELOW); 
	
	sceneNode->attachObject(msg);
}

void RendererVisitor::visit(Node* node)
{
	node->accept(this);
}

void RendererVisitor::visit(IdentifierNode* node)
{
	Ogre::Entity *barrel, *barrel2;

	if (node->getName() == "A")
	{
		// line segment ending in a leaf
		barrel = sceneMgr->createEntity("Barrel.mesh");
		barrel->setMaterial(mat);

		sceneNode = sceneNode->createChildSceneNode(Ogre::Vector3(0.0f, 6.5f, 0.0f));
		sceneNode->attachObject(barrel);
	}
	else if (node->getName() == "B")
	{
		// line segment with length A * 2
		barrel = sceneMgr->createEntity("Barrel.mesh");
		barrel->setMaterial(mat);
		barrel2 = sceneMgr->createEntity("Barrel.mesh");
		barrel2->setMaterial(mat);

		sceneNode = sceneNode->createChildSceneNode(Ogre::Vector3(0.0f, 2*6.5f, 0.0f));
		sceneNode->createChildSceneNode(Ogre::Vector3(0.0f, -6.5f, 0.0f))->attachObject(barrel2);
		sceneNode->attachObject(barrel);
	}
	else if (node->getName() == "F")
	{
		barrel = sceneMgr->createEntity("Barrel.mesh");
		barrel->setMaterial(mat);

		sceneNode = sceneNode->createChildSceneNode(Ogre::Vector3(0.0f, 6.5f, 0.0f));
		sceneNode->attachObject(barrel);
	}
	else if (node->getName() == "C")
	{
		// line segment as long as A. But it's rule might mean different behaviour
		barrel = sceneMgr->createEntity("Barrel.mesh");
		barrel->setMaterial(mat);

		sceneNode = sceneNode->createChildSceneNode(Ogre::Vector3(0.0f, 6.5f, 0.0f));
		sceneNode->attachObject(barrel);
	}
}

void RendererVisitor::visit(LeftBracketNode* node)
{
	sceneNodes.push(sceneNode);
	sceneNode = sceneNode->createChildSceneNode(Ogre::Vector3(0.0f, 0.0f, 0.0f));
	sceneNode->rotate(Ogre::Quaternion(Ogre::Radian(angle), Ogre::Vector3(0.0f, 0.0f, 1.0f)));
}

void RendererVisitor::visit(RightBracketNode* node)
{
	sceneNode = sceneNodes.top();
	sceneNodes.pop();
	sceneNode = sceneNode->createChildSceneNode(Ogre::Vector3(0.0f, 0.0f, 0.0f));
	sceneNode->rotate(Ogre::Quaternion(Ogre::Radian(-angle), Ogre::Vector3(0.0f, 0.0f, 1.0f)));
}

void RendererVisitor::visit(PlusNode* node)
{
	sceneNode = sceneNode->createChildSceneNode(Ogre::Vector3(0.0f, 6.5f, 0.0f));
	sceneNode->rotate(Ogre::Quaternion(Ogre::Radian(angle), Ogre::Vector3(0.0f, 0.0f, 1.0f)));
}

void RendererVisitor::visit(MinusNode* node)
{
	sceneNode = sceneNode->createChildSceneNode(Ogre::Vector3(0.0f, 6.5f, 0.0f));
	sceneNode->rotate(Ogre::Quaternion(Ogre::Radian(-angle), Ogre::Vector3(0.0f, 0.0f, 1.0f)));
}

void RendererVisitor::visit(ProgramNode* node)
{
	
}

void RendererVisitor::visit(RuleNode* node)
{
	
}

void RendererVisitor::visit(TokenSequenceNode* node)
{
	
}