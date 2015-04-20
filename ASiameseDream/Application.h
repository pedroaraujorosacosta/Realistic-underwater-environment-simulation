
#ifndef __Application_h_
#define __Application_h_

#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
#include "BaseApplication.h"

#define MAX_PLANTS 2

class Submarine;

class Application : public BaseApplication, Ogre::RenderQueueListener, public Ogre::RenderTargetListener
{
private:
	Ogre::TerrainGlobalOptions* mTerrainGlobals;
	Ogre::TerrainGroup* mTerrainGroup;
	OgreBites::Label* mInfoLabel;
	bool mTerrainsImported;

	// Keep our objects here
	Submarine*		sub;
	OgreBites::Slider* mDiffuseColorRSlider;
	OgreBites::Slider* mDiffuseColorGSlider;
	OgreBites::Slider* mDiffuseColorBSlider;
	OgreBites::Slider* mSpecularColorRSlider;
	OgreBites::Slider* mSpecularColorGSlider;
	OgreBites::Slider* mSpecularColorBSlider;
	OgreBites::Slider* mAngleSlider;
	OgreBites::Slider* mNumGenerationsSlider;
	OgreBites::Button* mGenerateButton;
	OgreBites::Button* mRunDemoButton;
	Ogre::Vector2	cameraVelocity; // TODO: remove this when project is integrated
	Ogre::Real		selectVelocity; // TODO: remove this when project is integrated
	Ogre::int32		selection;		// TODO: remove this when project is integrated
	Ogre::int32		oldSelection;	// TODO: remove this when project is integrated
	bool			isSelectMoving; // TODO: remove this when project is integrated
	enum SystemType { FRACTAL_PLANT, PYTHAGORAS_TREE };
	struct Plant_t
	{
		Ogre::SceneNode* plantNode;
		Ogre::Vector3	 plantLocation;
		Ogre::int32		 numGenerations;
		Ogre::Real		 maxHeight;
	} plants[MAX_PLANTS];
	Ogre::Real	diffuseRed, diffuseGreen, diffuseBlue;
	Ogre::Real	specularRed, specularGreen, specularBlue;
	Ogre::Real  angle;
	const Ogre::Real INIT_ANGLE;
	bool plantChanged;
	Ogre::Rectangle2D* plantQuad;
	Ogre::RenderTexture* renderTexture;
	Ogre::TexturePtr rttTexture;

	void defineTerrain(long x, long y);
	void initBlendMaps(Ogre::Terrain* terrain);
	void configureTerrainDefaults(Ogre::Light* light);
public:
	Application(void);
	virtual ~Application(void);

	virtual void go(void);

	// Sdk Tray listeners
	virtual void sliderMoved(OgreBites::Slider* slider);
	virtual void buttonHit(OgreBites::Button* bt);

	virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation,
		bool& repeatThisInvocation);
	virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation,
		bool& skipThisInvocation);

	virtual void preRenderTargetUpdate(const Ogre::RenderTargetEvent& rte);
	virtual void postRenderTargetUpdate(const Ogre::RenderTargetEvent& rte);
protected:
	virtual void createScene(void);
	virtual void createFrameListener(void);
	virtual void destroyScene(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	void createPlant(const std::string& filename, Plant_t& plant, SystemType type);
	void resetPlant();
	void renderToTexture(const Ogre::String& filename = "treeTexture.png");

	// IO Listeners
	virtual bool mouseMoved(const OIS::MouseEvent& evt);
	virtual bool mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	virtual bool keyPressed(const OIS::KeyEvent &arg);
	virtual bool keyReleased(const OIS::KeyEvent &arg);
};

#endif