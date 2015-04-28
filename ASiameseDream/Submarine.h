
#ifndef __Submarine_h_
#define __Submarine_h_

class Submarine
{
protected:
	Ogre::Vector3			position;
	Ogre::Real				yaw;
	Ogre::Real				pitch;
	Ogre::Real				roll;
	Ogre::Real				velocity;
	Ogre::Real				turnVelocity;
	Ogre::Real				camYaw;
	Ogre::SceneManager*		sceneMgr;
	Ogre::Entity*			sub;
	Ogre::SceneNode*		subNode;
	Ogre::SceneNode*		meshNode;
	Ogre::SceneNode*		camNode;
	Ogre::Camera*			camera;
	const Ogre::Real		maxVelocity = 10.0f;
	const Ogre::Real		maxTurnVelocity = 10.0f;

public:
	Submarine(Ogre::SceneManager* sceneMgr);
	Submarine(Ogre::SceneManager* sceneMgr, const Ogre::Vector3& position);
	~Submarine();

	void update(Ogre::Real dt);
	void addRotations(Ogre::Real dYaw, Ogre::Real dPitch, Ogre::Real dRoll);

	void moveFront(bool back = false);
	void turnRight(bool left = false);
	void stopMove();
	void stopTurn();
	void attachCamera(Ogre::Camera* camera);

	void setVelocity(Ogre::Real velocity);
	Ogre::Vector3 getPosition(void) const;
};

#endif