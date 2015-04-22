
#ifndef __Application_h_
#define __Application_h_

#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
#include "BaseApplication.h"

#define MAX_PLANTS 1

class Submarine;

class Application : public BaseApplication, public Ogre::RenderTargetListener
{
private:
	Ogre::TerrainGlobalOptions* mTerrainGlobals;
	Ogre::TerrainGroup* mTerrainGroup;
	OgreBites::Label* mInfoLabel;
	bool mTerrainsImported;

	//******************************************************************/
	//* Keep our objects here ******************************************/
	//******************************************************************/

	// Scene variables
	Submarine*			sub;
	Ogre::Rectangle2D*	plantQuad;
	Ogre::SceneNode*	miniScreenNode;
	Ogre::Vector2		cameraVelocity; // TODO: remove this when project is integrated
	Ogre::Real			selectVelocity; // TODO: remove this when project is integrated
	Ogre::int32			selection;		// TODO: remove this when project is integrated
	Ogre::int32			oldSelection;	// TODO: remove this when project is integrated
	bool				isSelectMoving; // TODO: remove this when project is integrated
	enum SystemType { FRACTAL_PLANT, PYTHAGORAS_TREE };
	struct Plant_t
	{
		Ogre::SceneNode* plantNode;
		Ogre::Vector3	 plantLocation;
		Ogre::int32		 numGenerations;
		Ogre::Real		 maxHeight;
	} plants[MAX_PLANTS];

	// UI related vars
	OgreBites::Slider*	mDiffuseColorRSlider;
	OgreBites::Slider*	mDiffuseColorGSlider;
	OgreBites::Slider*	mDiffuseColorBSlider;
	OgreBites::Slider*	mSpecularColorRSlider;
	OgreBites::Slider*	mSpecularColorGSlider;
	OgreBites::Slider*	mSpecularColorBSlider;
	OgreBites::Slider*	mAngleSlider;
	OgreBites::Slider*	mNumGenerationsSlider;
	OgreBites::Button*	mGenerateButton;
	OgreBites::Button*	mRunDemoButton;
	Ogre::Real	diffuseRed, diffuseGreen, diffuseBlue;
	Ogre::Real	specularRed, specularGreen, specularBlue;
	Ogre::Real  angle;
	const Ogre::Real INIT_ANGLE;

	// Render to texture vars
	Ogre::RenderTexture* renderTexture;
	Ogre::TexturePtr rttTexture;
	Ogre::String chromaTexName;
	Ogre::int32 texNumber;
	Ogre::String rttMatName;
	Ogre::MaterialPtr renderMaterial;

	// Application state variables
	enum AppState { PLANT_EDITOR, PLANT_DEMO} appState;
	Ogre::Light* light;

	//******************************************************************/
	//******************************************************************/

	void defineTerrain(long x, long y);
	void initBlendMaps(Ogre::Terrain* terrain);
	void configureTerrainDefaults(Ogre::Light* light);
	Ogre::String loadChromaKeyedTexture(const Ogre::String& filename,
		const Ogre::ColourValue& keyCol = Ogre::ColourValue::Black,
		const Ogre::String& resGroup = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		const Ogre::String& prefix = "ck_",
		int numMipmaps = Ogre::MIP_DEFAULT,
		float threshold = 0.003f);
public:
	Application(void);
	virtual ~Application(void);

	// Sdk Tray listeners
	virtual void sliderMoved(OgreBites::Slider* slider);
	virtual void buttonHit(OgreBites::Button* bt);

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
	bool loadImage(const Ogre::String& texture_name, const Ogre::String& texture_path, Ogre::Image& img);

	void createEditorScene();
	void destroyEditorScene();
	void createDemoScene();

	void editorMode();

	// IO Listeners
	virtual bool mouseMoved(const OIS::MouseEvent& evt);
	virtual bool mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	virtual bool keyPressed(const OIS::KeyEvent &arg);
	virtual bool keyReleased(const OIS::KeyEvent &arg);
};

#endif