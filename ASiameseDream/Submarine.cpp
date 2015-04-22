#include "Submarine.h"

Submarine::Submarine(Ogre::SceneManager* sceneMgr)
{
	Submarine(sceneMgr, Ogre::Vector3::ZERO);
}

Submarine::Submarine(Ogre::SceneManager* sceneMgr, const Ogre::Vector3& position) : yaw(0.0f),
	pitch(0.0f), roll(0.0f), velocity(0.0f), turnVelocity(0.0f)
{
	this->sceneMgr = sceneMgr;
	this->position = position;
	sub = sceneMgr->createEntity("Sub", "Sub.mesh");
	subNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
	subNode->setPosition(Ogre::Vector3(position));
	Ogre::SceneNode* meshNode = subNode->createChildSceneNode();
	meshNode->attachObject(sub);
	meshNode->setScale(0.01f, 0.01f, 0.01f);
	meshNode->setPosition(0.0f, 0.0f, 0.0f);
	camNode = subNode->createChildSceneNode();
	camNode->setPosition(-30.0f, 10.0f, 0.0f);
	camNode->setOrientation(Ogre::Quaternion(Ogre::Radian(-Ogre::Math::PI/2), Ogre::Vector3::UNIT_Y));
}

Submarine::~Submarine()
{

}

void Submarine::update(Ogre::Real dt)
{
	// rotate
	yaw += 0.13f * turnVelocity * dt;
	Ogre::Quaternion qYaw(Ogre::Angle(yaw), Ogre::Vector3::UNIT_Y);
	Ogre::Quaternion qPitch(Ogre::Angle(pitch), Ogre::Vector3::UNIT_Z);
	Ogre::Quaternion qRoll(Ogre::Angle(roll), Ogre::Vector3::UNIT_X);
	subNode->setOrientation(qYaw * qPitch * qRoll);

	// translate
	Ogre::Vector3 direction = subNode->getOrientation() * Ogre::Vector3::UNIT_X;
	Ogre::Vector3 newPosition = subNode->getPosition() + velocity * direction * dt;
	subNode->setPosition(newPosition);
}

void Submarine::addRotations(Ogre::Real dYaw, Ogre::Real dPitch, Ogre::Real dRoll)
{
	yaw += dYaw, pitch += dPitch, roll += dRoll;
}

void Submarine::attachCamera(Ogre::Camera* camera)
{
	camera->setPosition(0.0f, 0.0f, 0.0f);
	camera->setOrientation(subNode->getOrientation());
	camNode->attachObject(camera);
}

void Submarine::moveFront(bool back)
{
	velocity = (!back) ? maxVelocity : -maxVelocity;
}

void Submarine::turnRight(bool left)
{
	turnVelocity = (!left) ? -maxTurnVelocity : maxTurnVelocity;
}

void Submarine::stopMove()
{
	velocity = 0.0f;
}

void Submarine::stopTurn()
{
	turnVelocity = 0.0f;
}

// setters
void Submarine::setVelocity(Ogre::Real velocity)
{
	this->velocity = velocity;
}

// getters
Ogre::Vector3 Submarine::getPosition(void) const
{
	return position;
}