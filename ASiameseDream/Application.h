
#ifndef __Application_h_
#define __Application_h_

#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
#include "BaseApplication.h"

class Submarine;

class Application : public BaseApplication
{
private:
	Ogre::TerrainGlobalOptions* mTerrainGlobals;
	Ogre::TerrainGroup* mTerrainGroup;
	OgreBites::Label* mInfoLabel;
	bool mTerrainsImported;

	// Keep our objects here
	Submarine* sub;
	Ogre::Vector2 cameraVelocity; // TODO: remove this when project is integrated
	Ogre::MaterialPtr selectMat;
	Ogre::SceneNode* cubeNode;
	Ogre::Entity *selectorCube;
	enum SystemType {PYTHAGORAS_TREE, KOCH_CURVE, SIERPINSKY_TRI};
	struct Plant_t
	{
		Ogre::SceneNode* plantNode;
		Ogre::Vector3	 plantLocation;
		Ogre::int32		 numGenerations;
	} plants[3];
	Ogre::int32		 selection;

	void defineTerrain(long x, long y);
	void initBlendMaps(Ogre::Terrain* terrain);
	void configureTerrainDefaults(Ogre::Light* light);
public:
	Application(void);
	virtual ~Application(void);

protected:
	virtual void createScene(void);
	virtual void createFrameListener(void);
	virtual void destroyScene(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	void createPlant(const std::string& filename, Plant_t& plant, SystemType type);

	// IO Listeners
	virtual bool mouseMoved(const OIS::MouseEvent& evt);
	virtual bool mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	virtual bool keyPressed(const OIS::KeyEvent &arg);
	virtual bool keyReleased(const OIS::KeyEvent &arg);
};

#endif