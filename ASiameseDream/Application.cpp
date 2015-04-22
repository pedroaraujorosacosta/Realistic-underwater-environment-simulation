#include "Application.h"
#include "Submarine.h"

// includes for my GrammarParser lib
#include "GrammarParser.h"
#include "GrammarGenerator.h"
#include "GrammarCompactor.h"
#include "ProgramNode.h"
#include "StandardOutputVisitor.h"
#include "RendererVisitor.h"
#include "IdentifierNode.h"

using namespace GeneratorNodes;

//-------------------------------------------------------------------------------------
Application::Application(void) : INIT_AMBIENT_R(0.5f), INIT_AMBIENT_G(0.5f), INIT_AMBIENT_B(0.5f), diffuseRed(0.0f),
	diffuseGreen(0.0f), diffuseBlue(0.0f), specularRed(0.0f), specularGreen(0.0f), specularBlue(0.0f),
	INIT_ANGLE(Ogre::Math::PI / 6), appState(PLANT_EDITOR), texNumber(0), INIT_LIGHT_YAW(180.0f), INIT_LIGHT_PITCH(0.0f)
{
	sub = 0;
	ambientRed = INIT_AMBIENT_R;
	ambientGreen = INIT_AMBIENT_G;
	ambientBlue = INIT_AMBIENT_B;
	angle = INIT_ANGLE; 
	lightDirYaw = INIT_LIGHT_YAW;
	lightDirPitch = INIT_LIGHT_PITCH;
}
//-------------------------------------------------------------------------------------
Application::~Application(void)
{
	delete plantQuad;
}
//-------------------------------------------------------------------------------------
void Application::destroyScene(void)
{
	OGRE_DELETE mTerrainGroup;
	OGRE_DELETE mTerrainGlobals;
}
//-------------------------------------------------------------------------------------
void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img)
{
	img.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	if (flipX)
		img.flipAroundY();
	if (flipY)
		img.flipAroundX();
}
//-------------------------------------------------------------------------------------
void Application::defineTerrain(long x, long y)
{
	Ogre::String filename = mTerrainGroup->generateFilename(x, y);
	if (Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename))
	{
		mTerrainGroup->defineTerrain(x, y);
	}
	else
	{
		Ogre::Image img;
		getTerrainImage(x % 2 != 0, y % 2 != 0, img);
		mTerrainGroup->defineTerrain(x, y, &img);
		mTerrainsImported = true;
	}
}
//-------------------------------------------------------------------------------------
void Application::initBlendMaps(Ogre::Terrain* terrain)
{
	Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
	Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
	Ogre::Real minHeight0 = 70;
	Ogre::Real fadeDist0 = 40;
	Ogre::Real minHeight1 = 70;
	Ogre::Real fadeDist1 = 15;
	float* pBlend0 = blendMap0->getBlendPointer();
	float* pBlend1 = blendMap1->getBlendPointer();
	for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
	{
		for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
		{
			Ogre::Real tx, ty;

			blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
			Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
			Ogre::Real val = (height - minHeight0) / fadeDist0;
			val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
			*pBlend0++ = val;

			val = (height - minHeight1) / fadeDist1;
			val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
			*pBlend1++ = val;
		}
	}
	blendMap0->dirty();
	blendMap1->dirty();
	blendMap0->update();
	blendMap1->update();
}
//-------------------------------------------------------------------------------------
void Application::configureTerrainDefaults(Ogre::Light* light)
{
	// Configure global
	mTerrainGlobals->setMaxPixelError(8);
	// testing composite map
	mTerrainGlobals->setCompositeMapDistance(3000);

	// Important to set these so that the terrain knows what to use for derived (non-realtime) data
	mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
	mTerrainGlobals->setCompositeMapAmbient(mSceneMgr->getAmbientLight());
	mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());

	// Configure default import settings for if we use imported image
	Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
	defaultimp.terrainSize = 513;
	defaultimp.worldSize = 12000.0f;
	defaultimp.inputScale = 600; // due terrain.png is 8 bpp
	defaultimp.minBatchSize = 33;
	defaultimp.maxBatchSize = 65;

	// textures
	defaultimp.layerList.resize(3);
	defaultimp.layerList[0].worldSize = 100;
	defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
	defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
	defaultimp.layerList[1].worldSize = 30;
	defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
	defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
	defaultimp.layerList[2].worldSize = 200;
	defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
	defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
}
//-------------------------------------------------------------------------------------
void Application::createScene(void)
{
	createEditorScene();
}
//-------------------------------------------------------------------------------------
void Application::createEditorScene()
{
	// setup camera properties
	mCamera->setPosition(Ogre::Vector3(1683, 50, 2116));
	mCamera->setNearClipDistance(0.1f);
	mCamera->setFarClipDistance(50000.0f);

	if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
		mCamera->setFarClipDistance(0);   // enable infinite far clip distance if we can

	// setup RTT
	rttTexture =
		Ogre::TextureManager::getSingleton().createManual(
		"RttTex",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::TEX_TYPE_2D,
		mWindow->getWidth(), mWindow->getHeight(),
		0,
		Ogre::PF_R8G8B8,
		Ogre::TU_RENDERTARGET);

	renderTexture = rttTexture->getBuffer()->getRenderTarget();
	renderTexture->addViewport(mCamera);
	renderTexture->getViewport(0)->setClearEveryFrame(true);
	renderTexture->getViewport(0)->setBackgroundColour(Ogre::ColourValue::Black);
	renderTexture->getViewport(0)->setOverlaysEnabled(false);
	renderTexture->setAutoUpdated(false);

	rttMatName = "RttMat" + Ogre::StringConverter::toString(texNumber);
	renderMaterial =
		Ogre::MaterialManager::getSingleton().create(
		rttMatName,
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	renderMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	renderMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("RttTex");

	// Setup lights
	Ogre::Vector3 lightdir(Ogre::Math::Cos(lightDirPitch)*Ogre::Math::Sin(lightDirYaw),
		Ogre::Math::Sin(lightDirPitch), Ogre::Math::Cos(lightDirYaw) * Ogre::Math::Cos(lightDirPitch));
	lightdir.normalise();

	light = mSceneMgr->createLight("tstLight");
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDirection(lightdir);
	light->setDiffuseColour(Ogre::ColourValue::White);
	light->setSpecularColour(Ogre::ColourValue(0.4f, 0.4f, 0.4f));

	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

	Ogre::ColourValue fadeColour(0.0f, 0.0f, 0.0f);
	mWindow->getViewport(0)->setBackgroundColour(fadeColour);

	// create the selector cube node & attach its mesh
	oldSelection = selection = FRACTAL_PLANT;
	selectVelocity = 10.0f;
	isSelectMoving = false;

	// Create the quad where we render our plant
	plantQuad = new Ogre::Rectangle2D(true);
	plantQuad->setCorners(-1.0f, 1.0f, 1.0f, -1.0f);
	plantQuad->setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);
	miniScreenNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	miniScreenNode->attachObject(plantQuad);

	renderTexture->addListener(this);

	// create the plants
	plants[FRACTAL_PLANT].numGenerations = 3;
	plants[FRACTAL_PLANT].plantLocation = Ogre::Vector3(1683, 49.7f, 2115);
	plants[FRACTAL_PLANT].maxHeight = 0.0f;
	createPlant("gram5.txt", plants[FRACTAL_PLANT], FRACTAL_PLANT);
}
//-------------------------------------------------------------------------------------
void Application::createFrameListener(void)
{
	BaseApplication::createFrameListener();

	// Setup GUI for the demo
	mInfoLabel = mTrayMgr->createLabel(OgreBites::TL_TOP, "TInfo", "", 350);
	mTrayMgr->removeWidgetFromTray(mInfoLabel);
	mInfoLabel->hide();
	mTrayMgr->hideLogo();

	// GUI for setting light properties
	mTrayMgr->setTrayWidgetAlignment(OgreBites::TL_LEFT, Ogre::GuiHorizontalAlignment::GHA_CENTER);
	mTrayMgr->createLabel(OgreBites::TL_LEFT, "TLLightDir", "Light Direction", 170.0f);
	mLightYawSlider = mTrayMgr->createLongSlider(OgreBites::TL_LEFT, "TSLightYaw", "Yaw", 50,
		60, 0, 359, 360);
	mLightYawSlider->setValue(INIT_LIGHT_YAW);
	mLightPitchSlider = mTrayMgr->createLongSlider(OgreBites::TL_LEFT, "TSLightPitch", "Pitch", 50,
		50, -90, 90, 181);
	mLightPitchSlider->setValue(INIT_LIGHT_PITCH);

	// GUI for setting ambient properties of tree material
	mTrayMgr->setTrayWidgetAlignment(OgreBites::TL_RIGHT, Ogre::GuiHorizontalAlignment::GHA_CENTER);
	mTrayMgr->createLabel(OgreBites::TL_RIGHT, "TLAmbient", "Ambient", 100.0f);
	mAmbientColorRSlider = mTrayMgr->createLongSlider(OgreBites::TL_RIGHT, "TSAmbientR", "R", 50,
		60, 0, 255, 256);
	mAmbientColorRSlider->setValue(INIT_AMBIENT_R * 255);
	mAmbientColorGSlider = mTrayMgr->createLongSlider(OgreBites::TL_RIGHT, "TSAmbientG", "G", 50,
		60, 0, 255, 256);
	mAmbientColorGSlider->setValue(INIT_AMBIENT_G * 255);
	mAmbientColorBSlider = mTrayMgr->createLongSlider(OgreBites::TL_RIGHT, "TSAmbientB", "B", 50,
		60, 0, 255, 256);
	mAmbientColorBSlider->setValue(INIT_AMBIENT_B * 255);

	mTrayMgr->createSeparator(OgreBites::TL_RIGHT, "TSeparator0", 150.0f);

	// GUI for setting diffuse properties of tree material
	mTrayMgr->createLabel(OgreBites::TL_RIGHT, "TLDiffuse", "Diffuse", 100.0f);
	mDiffuseColorRSlider = mTrayMgr->createLongSlider(OgreBites::TL_RIGHT, "TSDiffuseR", "R", 50,
		60, 0, 255, 256);
	mDiffuseColorGSlider = mTrayMgr->createLongSlider(OgreBites::TL_RIGHT, "TSDiffuseG", "G", 50,
		60, 0, 255, 256);
	mDiffuseColorBSlider = mTrayMgr->createLongSlider(OgreBites::TL_RIGHT, "TSDiffuseB", "B", 50,
		60, 0, 255, 256);

	mTrayMgr->createSeparator(OgreBites::TL_RIGHT, "TSeparator1", 150.0f);

	// GUI for setting specular properties of tree material
	mTrayMgr->createLabel(OgreBites::TL_RIGHT, "TLSpecular", "Specular", 100.0f);
	mSpecularColorRSlider = mTrayMgr->createLongSlider(OgreBites::TL_RIGHT, "TSSpecularR", "R", 50,
		60, 0, 255, 256);
	mSpecularColorGSlider = mTrayMgr->createLongSlider(OgreBites::TL_RIGHT, "TSSpecularG", "G", 50,
		60, 0, 255, 256);
	mSpecularColorBSlider = mTrayMgr->createLongSlider(OgreBites::TL_RIGHT, "TSSpecularB", "B", 50,
		60, 0, 255, 256);

	mTrayMgr->createSeparator(OgreBites::TL_RIGHT, "TSeparator2", 150.0f);

	// GUI for setting angle properties
	mTrayMgr->createLabel(OgreBites::TL_RIGHT, "TLAngle", "Angle", 100.0f);
	mAngleSlider = mTrayMgr->createLongSlider(OgreBites::TL_RIGHT, "TSAngle", "", 50,
		60, 0, 90, 91);
	mAngleSlider->setValue(Ogre::Math::Floor(angle * 180.0f / Ogre::Math::PI), true);

	mTrayMgr->createSeparator(OgreBites::TL_RIGHT, "TSeparator3", 150.0f);

	// GUI for setting generations
	mTrayMgr->createLabel(OgreBites::TL_RIGHT, "TLNumGen", "Generations", 150.0f);
	mNumGenerationsSlider = mTrayMgr->createLongSlider(OgreBites::TL_RIGHT, "TSNumGen", "", 50,
		60, 0, 6, 7);
	mNumGenerationsSlider->setValue(plants[FRACTAL_PLANT].numGenerations);

	mTrayMgr->createSeparator(OgreBites::TL_RIGHT, "TSeparator4", 150.0f);

	// Buttons for Generating the trees and running the demo
	mGenerateButton = mTrayMgr->createButton(OgreBites::TL_RIGHT, "TGenButton", "Generate", 100);
	mRunDemoButton = mTrayMgr->createButton(OgreBites::TL_RIGHT, "TRunButton", "Run Demo", 100);;

}
//-------------------------------------------------------------------------------------
bool Application::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	bool ret = BaseApplication::frameRenderingQueued(evt);

	mInfoLabel->setCaption("L-System to texture demo");

	if (sub) sub->update(evt.timeSinceLastFrame);

	if(appState == PLANT_EDITOR) mTrayMgr->showCursor();
	else mTrayMgr->hideCursor();

	// update camera position - TODO: remove this when project is integrated
	Ogre::Vector3 directionX = mCamera->getOrientation() * Ogre::Vector3::UNIT_X;
	Ogre::Vector3 directionY = - (mCamera->getOrientation() * Ogre::Vector3::UNIT_Z);
	Ogre::Vector3 newPosition = mCamera->getPosition() + directionX * cameraVelocity.x * evt.timeSinceLastFrame + 
		directionY * cameraVelocity.y * evt.timeSinceLastFrame;
	mCamera->setPosition(newPosition);

	// update selector velocity & direction - TODO: remove this when project is integrated
	/*if (selection != oldSelection && isSelectMoving)
	{
		Ogre::Vector3 vectorToSelected = plants[selection].plantLocation - mCamera->getPosition() +
			Ogre::Vector3(0.0f, 0.3f, 1.0f);
		Ogre::Real distanceToSelected = vectorToSelected.length();
		Ogre::Vector3 selectDirection = vectorToSelected /
			(plants[selection].plantLocation - plants[oldSelection].plantLocation).length();
		if (distanceToSelected > 0.001f)
		{
			Ogre::Vector3 newSelectPosition = mCamera->getPosition() + selectVelocity * selectDirection *
				evt.timeSinceLastFrame;
			mCamera->setPosition(newSelectPosition);
		}
		else
			isSelectMoving = false;
	}*/

	return ret;
}

//-------------------------------main func----------------------------------
#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
	int main(int argc, char *argv[])
#endif
	{
		// Create application object
		Application app;

		try {
			app.go();
		}
		catch (Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "An exception has occurred: " <<
				e.getFullDescription().c_str() << std::endl;
#endif
		}

		return 0;
	}

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------

// OIS::MouseListeners
bool Application::mouseMoved(const OIS::MouseEvent& evt)
{
	//sub->addRotations(0.13f * evt.state.X.rel, 0.13f * evt.state.Y.rel, 0.0f);

	if (mTrayMgr->injectMouseMove(evt)) return true;
	
	if (appState == PLANT_DEMO)
	{
		mCamera->yaw(Ogre::Degree(-0.13f * evt.state.X.rel));
		mCamera->pitch(Ogre::Degree(-0.13f * evt.state.Y.rel));
	}

	return true;
}

bool Application::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	if (mTrayMgr->injectMouseDown(evt, id)) return true;
	return true;
}

bool Application::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	if (mTrayMgr->injectMouseUp(evt, id)) return true;
	return true;
}
//---------------------------------------------------------------------------
bool Application::keyPressed(const OIS::KeyEvent &arg)
{
	if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up

	if (arg.key == OIS::KC_F)   // toggle visibility of advanced frame stats
	{
		mTrayMgr->toggleAdvancedFrameStats();
	}
	else if (arg.key == OIS::KC_G)   // toggle visibility of even rarer debugging details
	{
		if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
		{
			mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
			mDetailsPanel->show();
		}
		else
		{
			mTrayMgr->removeWidgetFromTray(mDetailsPanel);
			mDetailsPanel->hide();
		}
	}
	else if (arg.key == OIS::KC_T)   // cycle polygon rendering mode
	{
		Ogre::String newVal;
		Ogre::TextureFilterOptions tfo;
		unsigned int aniso;

		switch (mDetailsPanel->getParamValue(9).asUTF8()[0])
		{
		case 'B':
			newVal = "Trilinear";
			tfo = Ogre::TFO_TRILINEAR;
			aniso = 1;
			break;
		case 'T':
			newVal = "Anisotropic";
			tfo = Ogre::TFO_ANISOTROPIC;
			aniso = 8;
			break;
		case 'A':
			newVal = "None";
			tfo = Ogre::TFO_NONE;
			aniso = 1;
			break;
		default:
			newVal = "Bilinear";
			tfo = Ogre::TFO_BILINEAR;
			aniso = 1;
		}

		Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
		Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
		mDetailsPanel->setParamValue(9, newVal);
	}
	else if (arg.key == OIS::KC_R)   // cycle polygon rendering mode
	{
		Ogre::String newVal;
		Ogre::PolygonMode pm;

		switch (mCamera->getPolygonMode())
		{
		case Ogre::PM_SOLID:
			newVal = "Wireframe";
			pm = Ogre::PM_WIREFRAME;
			break;
		case Ogre::PM_WIREFRAME:
			newVal = "Points";
			pm = Ogre::PM_POINTS;
			break;
		default:
			newVal = "Solid";
			pm = Ogre::PM_SOLID;
		}

		mCamera->setPolygonMode(pm);
		mDetailsPanel->setParamValue(10, newVal);
	}
	else if (arg.key == OIS::KC_F5)   // refresh all textures
	{
		Ogre::TextureManager::getSingleton().reloadAll();
	}
	else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
	{
		mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
	}
	else if (arg.key == OIS::KC_ESCAPE)
	{
		mShutDown = true;
	}
	else if (appState == PLANT_DEMO)
	{
		if (arg.key == OIS::KC_A)
		{
			cameraVelocity.x = -100.0f;
		}
		else if (arg.key == OIS::KC_D)
		{
			cameraVelocity.x = 100.0f;
		}
		else if (arg.key == OIS::KC_W)
		{
			cameraVelocity.y = 100.0f;
		}
		else if (arg.key == OIS::KC_S)
		{
			cameraVelocity.y = -100.0f;
		}
	}
	else if (appState == PLANT_EDITOR)
	{
		if (arg.key == OIS::KC_ADD)
		{
			plants[selection].numGenerations++;
			resetPlant();
		}
		else if (arg.key == OIS::KC_SUBTRACT)
		{
			if (plants[selection].numGenerations > 0) plants[selection].numGenerations--;
			resetPlant();
		}
		else if (arg.key == OIS::KC_LEFT)
		{
			oldSelection = selection;
			selection = ++selection % MAX_PLANTS;
			isSelectMoving = true;
		}
		else if (arg.key == OIS::KC_RIGHT)
		{
			oldSelection = selection;
			if (--selection < 0)
				selection += MAX_PLANTS;
			isSelectMoving = true;
		}
	}

	return true;
}
//---------------------------------------------------------------------------
bool Application::keyReleased(const OIS::KeyEvent &arg)
{
	if (appState == PLANT_DEMO)
	{
		if (arg.key == OIS::KC_A || arg.key == OIS::KC_D)
		{
			if (sub) sub->stopTurn();
			cameraVelocity.x = 0;
		}
		else if (arg.key == OIS::KC_W || arg.key == OIS::KC_S)
		{
			if (sub) sub->stopMove();
			cameraVelocity.y = 0;
		}
	}

	return true;
}
//---------------------------------------------------------------------------

// Sdk Tray listeners
void Application::sliderMoved(OgreBites::Slider* slider)
{
	if (slider == mAmbientColorRSlider)
		ambientRed = slider->getValue() / 255.0f;
	else if (slider == mAmbientColorGSlider)
		ambientGreen = slider->getValue() / 255.0f;
	else if (slider == mAmbientColorBSlider)
		ambientBlue = slider->getValue() / 255.0f;
	else if (slider == mDiffuseColorRSlider)
		diffuseRed = slider->getValue() / 255.0f;
	else if (slider == mDiffuseColorGSlider)
		diffuseGreen = slider->getValue() / 255.0f;
	else if (slider == mDiffuseColorBSlider)
		diffuseBlue = slider->getValue() / 255.0f;
	else if (slider == mSpecularColorRSlider) 
		specularRed = slider->getValue() / 255.0f;
	else if (slider == mSpecularColorGSlider)
		specularGreen = slider->getValue() / 255.0f;
	else if (slider == mSpecularColorBSlider)
		specularBlue = slider->getValue() / 255.0f;
	else if (slider == mAngleSlider)
		angle = mAngleSlider->getValue() * Ogre::Math::PI / 180.0f;
	else if (slider == mNumGenerationsSlider)
		plants[FRACTAL_PLANT].numGenerations = mNumGenerationsSlider->getValue();
	else if (slider == mLightYawSlider)
		lightDirYaw = slider->getValue() * 2 * Ogre::Math::PI / 360.0f;
	else if (slider == mLightPitchSlider)
		lightDirPitch = slider->getValue() * Ogre::Math::PI / 90.0f;
}
void Application::buttonHit(OgreBites::Button* bt)
{
	if (bt == mGenerateButton)
		resetPlant();
	else if (bt == mRunDemoButton)
		editorMode();
}

void Application::resetPlant()
{
	// reset our lighting properties
	// Setup lights
	Ogre::Vector3 lightdir = Ogre::Quaternion(Ogre::Radian(lightDirYaw), Ogre::Vector3(0.0f, 1.0f, 0.0f)) *
		Ogre::Quaternion(Ogre::Radian(lightDirPitch), Ogre::Vector3(1.0f, 0.0f, 0.0f)) * Ogre::Vector3(0.0f, 0.0f, 1.0f);
	/*Ogre::Vector3 lightdir(Ogre::Math::Cos(lightDirPitch)*Ogre::Math::Sin(lightDirYaw),
	Ogre::Math::Sin(lightDirPitch), Ogre::Math::Cos(lightDirYaw) * Ogre::Math::Cos(lightDirPitch));*/
	lightdir.normalise();
	light->setDirection(lightdir);

	plants[FRACTAL_PLANT].maxHeight = 0.0f;
	createPlant("gram5.txt", plants[FRACTAL_PLANT], FRACTAL_PLANT);
}

void Application::createPlant(const std::string& filename, Plant_t& plant, SystemType type)
{
	GrammarParser gp;
	std::vector<std::string> grammarText;
	std::string line;
	std::ifstream ifStream;

	ifStream.open(filename);
	if (ifStream.is_open())
	{
		// read and store the grammar file's lines
		while (getline(ifStream, line))
			grammarText.push_back(line);
		ifStream.close();

		// parse the grammar, get its rules
		TokenInfo grammarRules;
		gp.parseGrammar(grammarText, grammarRules);

		ProgramNode grammar = *(ProgramNode*)grammarRules.tokenNode;
		
		// print grammar
		grammar.accept(new StandardOutputVisitor());

		// Add start symbol
		IdentifierNode* idNode = new IdentifierNode("F");
		//IdentifierNode* idNode2;
		std::vector<Node*> symbols;

		switch (type)
		{
		case FRACTAL_PLANT:
			idNode = new IdentifierNode("X");
			symbols.push_back(idNode);
			break;
		case PYTHAGORAS_TREE:
			idNode = new IdentifierNode("A");
			symbols.push_back(idNode);
			break;
		default:
			idNode = 0;
			break;
		}

		// create a material for our plant
		Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create(Ogre::String("mat").c_str(),
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		mat->setAmbient(ambientRed, ambientGreen, ambientBlue);
		mat->setDiffuse(diffuseRed, diffuseGreen, diffuseBlue, 1.0f);
		mat->setSpecular(specularRed, specularGreen, specularBlue, 1.0f);

		// Renderer visitor - will create our entities according to the grammar nodes it visits
		plant.plantNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		RendererVisitor rendererVis(mSceneMgr, plant.plantNode, plant.plantLocation, angle, mat);

		// Evolve the generations
		for (Ogre::int32 i = 0; i < plant.numGenerations; i++)
		{
			GrammarGenerator::generate(grammar, symbols);
			GrammarGenerator::printSymbols(symbols);
		}

		GrammarCompactor::compact(symbols);
		
		// Semantic processing of resulting parse tree, adding entities to ogre
		for (std::vector<Node*>::iterator it = symbols.begin(); it != symbols.end(); it++)
			(*it)->accept(&rendererVis);

		Ogre::Real h = abs(rendererVis.getMaxHeightReached() - 49.7f);
		mCamera->setPosition(Ogre::Vector3(1683.0f, 49.7f + h / 2, 0.05 + 2115.0f + h / atan(1.0f)));

		renderToTexture();
		mSceneMgr->destroySceneNode(plants[FRACTAL_PLANT].plantNode);

		for (std::vector<Node*>::const_iterator it = symbols.begin(); it != symbols.end(); )
		{
			delete *it;
			it = symbols.erase(it);
		}
	}
	else
	{
		std::cout << "Error: unable to open file: " << filename << std::endl;
		plant.plantNode = 0;
	}
}

void Application::renderToTexture(const Ogre::String& filename)
{
	renderTexture->update();
	renderTexture->writeContentsToFile(filename);

	// Load the image that was rendered
	chromaTexName = loadChromaKeyedTexture(filename, Ogre::ColourValue(0.0f, 0.0f, 0.0f), 
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "ck_", Ogre::TextureMipmap::MIP_DEFAULT, 0.098f);

	rttMatName = "RttMat" + Ogre::StringConverter::toString(texNumber);
	renderMaterial =
		Ogre::MaterialManager::getSingleton().create(
		rttMatName,
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	renderMaterial->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_GREATER_EQUAL, 1);
	renderMaterial->getTechnique(0)->getPass(0)->setCullingMode(Ogre::CULL_NONE);
	renderMaterial->getTechnique(0)->getPass(0)->setManualCullingMode(Ogre::MANUAL_CULL_NONE);
	renderMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	//renderMaterial->getTechnique(0)->getPass(0)->setTextureFiltering(Ogre::TextureFilterOptions::TFO_TRILINEAR);
	//renderMaterial->getTechnique(0)->getPass(0)->setTextureFiltering(Ogre::TextureFilterOptions::TFO_ANISOTROPIC);
	//renderMaterial->getTechnique(0)->getPass(0)->setTextureFiltering(Ogre::TextureFilterOptions::TFO_BILINEAR);
	renderMaterial->getTechnique(0)->getPass(0)->setTextureFiltering(Ogre::TextureFilterOptions::TFO_NONE);
	renderMaterial->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SceneBlendType::SBT_TRANSPARENT_ALPHA);
	//renderMaterial->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_REPLACE);

	// Instead of the original filename of the image, use the one returned by loadChromaKeyedTexture above:
	Ogre::TextureUnitState*  t = renderMaterial->getTechnique(0)->getPass(0)->createTextureUnitState(chromaTexName);
	t->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);

	plantQuad->setMaterial(rttMatName);
}

void Application::preRenderTargetUpdate(const Ogre::RenderTargetEvent& rte)
{
	plantQuad->setVisible(false);
}

void Application::postRenderTargetUpdate(const Ogre::RenderTargetEvent& rte)
{
	plantQuad->setVisible(true);
}

void Application::createDemoScene()
{
	sub = new Submarine(mSceneMgr, Ogre::Vector3(1963, 50, 1660));
	sub->attachCamera(mCamera);

	Ogre::Vector3 lightdir(0.55f, -0.3f, 0.75f);
	lightdir.normalise();

	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDirection(lightdir);
	light->setDiffuseColour(Ogre::ColourValue::White);
	light->setSpecularColour(Ogre::ColourValue(0.4f, 0.4f, 0.4f));

	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2f, 0.2f, 0.2f));

	mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();

	mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(mSceneMgr, Ogre::Terrain::ALIGN_X_Z, 513, 12000.0f);
	mTerrainGroup->setFilenameConvention(Ogre::String("SiameseDreamTerrain"), Ogre::String("dat"));
	mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

	Ogre::ColourValue fadeColour(0.1f, 0.1f, 0.1f);
	mWindow->getViewport(0)->setBackgroundColour(fadeColour);
	mSceneMgr->setFog(Ogre::FOG_LINEAR, fadeColour, 0.0, 100, 6000);

	Ogre::Plane plane;
	plane.d = 10;
	plane.normal = Ogre::Vector3::NEGATIVE_UNIT_Y;

	mSceneMgr->setSkyPlane(true, plane, "Examples/SpaceSkyPlane", 100, 45, true, 0.5, 150, 150);

	configureTerrainDefaults(light);

	for (long x = 0; x <= 0; ++x)
		for (long y = 0; y <= 0; ++y)
			defineTerrain(x, y);

	// sync load since we want everything in place when we start
	mTerrainGroup->loadAllTerrains(true);

	if (mTerrainsImported)
	{
		Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
		while (ti.hasMoreElements())
		{
			Ogre::Terrain* t = ti.getNext()->instance;
			initBlendMaps(t);
		}
	}

	mTerrainGroup->freeTemporaryResources();

	// create a billboard set for far objects
	Ogre::SceneNode* farBBNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("farBillboards", Ogre::Vector3(3000, 70.0f, 1660));
	Ogre::BillboardSet* farBillboardSet = mSceneMgr->createBillboardSet();
	farBillboardSet->setBillboardType(Ogre::BillboardType::BBT_ORIENTED_COMMON);
	farBillboardSet->setCommonDirection(Ogre::Vector3(0.0f, 1.0f, 0.0f));
	farBillboardSet->createBillboard(Ogre::Vector3(0.0f, 0.0f, 0.0f));
	farBillboardSet->createBillboard(Ogre::Vector3(-180.0f, -20.0f, 80.0f));
	farBillboardSet->createBillboard(Ogre::Vector3(120.0f, 20.0f, -180.0f));
	farBillboardSet->createBillboard(Ogre::Vector3(70.0f, 20.0f, -380.0f));
	farBillboardSet->createBillboard(Ogre::Vector3(-70.0f, 50.0f, 380.0f));
	farBillboardSet->setMaterialName(rttMatName);
	farBillboardSet->setDefaultDimensions(50.0f, 50.0f);
	farBBNode->attachObject(farBillboardSet);

	// create a billboard set for mid distance objects
	Ogre::SceneNode* midBBNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("midBillboards", Ogre::Vector3(2500, 70.0f, 1660));
	Ogre::BillboardSet* midBillboardSet = mSceneMgr->createBillboardSet();
	midBillboardSet->setBillboardType(Ogre::BillboardType::BBT_ORIENTED_COMMON);
	midBillboardSet->setCommonDirection(Ogre::Vector3(0.0f, 1.0f, 0.0f));
	midBillboardSet->createBillboard(Ogre::Vector3(0.0f, -40.0f, 40.0f));
	midBillboardSet->createBillboard(Ogre::Vector3(-80.0f, -42.0f, 120.0f));
	midBillboardSet->createBillboard(Ogre::Vector3(120.0f, -20.0f, -180.0f));
	midBillboardSet->createBillboard(Ogre::Vector3(70.0f, -30.0f, -280.0f));
	midBillboardSet->setMaterialName(rttMatName);
	midBillboardSet->setDefaultDimensions(50.0f, 50.0f);
	midBBNode->attachObject(midBillboardSet);
}

void Application::destroyEditorScene()
{
	mSceneMgr->destroySceneNode(miniScreenNode);
}

void Application::editorMode()
{
	appState = PLANT_DEMO;
	destroyEditorScene();
	createDemoScene();
}

Ogre::String Application::loadChromaKeyedTexture(const Ogre::String& filename, const Ogre::ColourValue& keyCol,
	const Ogre::String& resGroup, const Ogre::String& prefix, int numMipmaps, float threshold)
{
	using namespace Ogre;
	using std::fabs;
	Image srcImg;

	loadImage(filename, filename, srcImg);
	//srcImg.load(filename, resGroup);
	uint width = srcImg.getWidth(), height = srcImg.getHeight();

	// Since Ogre 1.6 Shoggoth, the OGRE_ALLOC_T memory macro must be used:
	uchar* pixelData = OGRE_ALLOC_T(uchar, PixelUtil::getMemorySize(width, height, 1, PF_A8R8G8B8), MEMCATEGORY_GENERAL);
	ulong pxDataIndex = 0, pxDataIndexStep = PixelUtil::getNumElemBytes(PF_A8R8G8B8);

	for (uint y = 0; y < height; ++y)
	{
		for (uint x = 0; x < width; ++x)
		{
			ColourValue pixCol = srcImg.getColourAt(x, y, 0);
			ColourValue diffCol = pixCol - keyCol;
			pixCol.a = ((fabs(diffCol.r)<threshold) && (fabs(diffCol.g)<threshold) && (fabs(diffCol.b)<threshold))
				? 0 : 1;
			Ogre::PixelUtil::packColour(pixCol, PF_A8R8G8B8, static_cast<void*> (pixelData + pxDataIndex));
			pxDataIndex += pxDataIndexStep;
		}
	}

	Image chromaKeyedImg;
	chromaKeyedImg.loadDynamicImage(pixelData, width, height, 1, PF_A8R8G8B8, true);
	String resName = prefix + filename + Ogre::StringConverter::toString(++texNumber);
	// You could save the chroma keyed image at this point for caching:
	// chromaKeyedImg.save(resName); 
	TextureManager::getSingleton().loadImage(resName, resGroup, chromaKeyedImg, TEX_TYPE_2D, numMipmaps);
	return resName;
}

bool Application::loadImage(const Ogre::String& texture_name, const Ogre::String& texture_path, Ogre::Image& img)
{
	bool image_loaded = false;
	std::ifstream ifs(texture_path.c_str(), std::ios::binary | std::ios::in);
	if (ifs.is_open())
	{
		Ogre::String tex_ext;
		Ogre::String::size_type index_of_extension = texture_path.find_last_of('.');
		if (index_of_extension != Ogre::String::npos)
		{
			tex_ext = texture_path.substr(index_of_extension + 1);
			Ogre::DataStreamPtr data_stream(new Ogre::FileStreamDataStream(texture_path, &ifs, false));
			//Ogre::Image img;
			img.load(data_stream, tex_ext);
			Ogre::TextureManager::getSingleton().loadImage(texture_name,
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, img, Ogre::TEX_TYPE_2D, 0, 1.0f);
			image_loaded = true;
		}
		ifs.close();
	}
	return image_loaded;
}