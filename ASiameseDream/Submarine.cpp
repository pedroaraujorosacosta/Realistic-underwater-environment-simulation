#include "Submarine.h"
#include "TextRenderer.h"

Submarine::Submarine(Ogre::SceneManager* sceneMgr)
{
	//ogText->setText("hello world");
	//ogText->setPos(0, 0);
	//TextRenderer::getSingleton().addTextBox("txtGreeting", "Hello", 10, 10, 100, 20, Ogre::ColourValue::Green);
	Submarine(sceneMgr, Ogre::Vector3::ZERO);
}

Submarine::Submarine(Ogre::SceneManager* sceneMgr, const Ogre::Vector3& position) : yaw(0.0f),
	pitch(0.0f), roll(0.0f), velocity(0.0f), turnVelocity(0.0f), camera(NULL)
{
	this->sceneMgr = sceneMgr;
	this->position = position;
	sub = sceneMgr->createEntity("Sub", "Sub.mesh");
	subNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
	subNode->setPosition(Ogre::Vector3(position));
	meshNode = subNode->createChildSceneNode();
	meshNode->attachObject(sub);
	meshNode->setScale(0.01f, 0.01f, 0.01f);
	meshNode->setPosition(0.0f, 0.0f, 0.0f);
	camNode = subNode->createChildSceneNode();
	camNode->setPosition(30.0f, 10.0f, 0.0f);
	camNode->setOrientation(Ogre::Quaternion(Ogre::Radian(-Ogre::Math::PI/2), Ogre::Vector3::UNIT_Y));

	
}

Submarine::~Submarine()
{
}

void Submarine::update(Ogre::Real dt)
{
	// camera rotation
	Ogre::Real camYaw = Ogre::Math::lerp<Ogre::Real, Ogre::Real>(yaw, yaw + 0.13f * turnVelocity * dt, 0.8f);
	//camNode->setOrientation(Ogre::Quaternion(Ogre::Radian(-camYaw - Ogre::Math::PI / 2), Ogre::Vector3::UNIT_Y));
	if (camera)
	{
		Ogre::Real currentCamYaw = camera->getOrientation().getYaw().valueRadians();
		Ogre::Real camYaw = Ogre::Math::lerp<Ogre::Real, Ogre::Real>(currentCamYaw, yaw, 0.8f);
		//camera->setOrientation(Ogre::Quaternion(Ogre::Radian(camYaw - Ogre::Math::PI / 2), Ogre::Vector3::UNIT_Y));
		Ogre::Vector3 lookAtVec = Ogre::Quaternion(Ogre::Radian(0), Ogre::Vector3::UNIT_Y) * Ogre::Vector3::UNIT_X;
		camera->lookAt(subNode->getPosition() + lookAtVec);
		//camNode->setPosition(Ogre::Quaternion(Ogre::Radian(- Ogre::Math::PI / 2), Ogre::Vector3::UNIT_Y) * Ogre::Vector3::UNIT_X);
	}

	// rotate
	yaw += 0.13f * turnVelocity * dt;
	Ogre::Quaternion qYaw(Ogre::Radian(yaw), Ogre::Vector3::UNIT_Y);
	Ogre::Quaternion qPitch(Ogre::Radian(pitch), Ogre::Vector3::UNIT_Z);
	Ogre::Quaternion qRoll(Ogre::Radian(roll), Ogre::Vector3::UNIT_X);
	meshNode->setOrientation(qYaw * qPitch * qRoll);

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
	this->camera = camera;
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