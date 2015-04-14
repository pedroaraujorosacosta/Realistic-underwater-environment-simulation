#include "Application.h"
#include "Submarine.h"

// includes for my GrammarParser lib
#include "GrammarParser.h"
#include "GrammarGenerator.h"
#include "ProgramNode.h"
#include "StandardOutputVisitor.h"
#include "RendererVisitor.h"
#include "IdentifierNode.h"

using namespace GeneratorNodes;

//-------------------------------------------------------------------------------------
Application::Application(void)
{
	sub = 0;
}
//-------------------------------------------------------------------------------------
Application::~Application(void)
{
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
	//mCamera->setPosition(Ogre::Vector3(1683, 50, 2116));
	//mCamera->lookAt(Ogre::Vector3(1963, 50, 1660));

	mCamera->setNearClipDistance(0.1f);
	mCamera->setFarClipDistance(50000.0f);

	if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
		mCamera->setFarClipDistance(0);   // enable infinite far clip distance if we can

	/*Ogre::Entity* sub = mSceneMgr->createEntity("Sub", "Sub.mesh");
	Ogre::SceneNode* subNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	subNode->attachObject(sub);
	subNode->setPosition(Ogre::Vector3(1963, 50, 1660));
	subNode->setScale(Ogre::Vector3(0.05f, 0.05f, 0.05f));*/
	
	/***Uncomment the following for the submarine***/
	/*sub = new Submarine(mSceneMgr, Ogre::Vector3(1963, 50, 1660));
	sub->attachCamera(mCamera);*/

	// create the plants
	plants[PYTHAGORAS_TREE].numGenerations = 3;
	plants[PYTHAGORAS_TREE].plantLocation = Ogre::Vector3(1683, 49.7f, 2115);
	plants[PYTHAGORAS_TREE].plantNode = createPlant("gram1.txt", plants[PYTHAGORAS_TREE].numGenerations,
		plants[PYTHAGORAS_TREE].plantLocation, PYTHAGORAS_TREE);

	plants[KOCH_CURVE].numGenerations = 3;
	plants[KOCH_CURVE].plantLocation = Ogre::Vector3(1684, 49.7f, 2113);
	plants[KOCH_CURVE].plantNode = createPlant("gram2.txt", plants[KOCH_CURVE].numGenerations,
		plants[KOCH_CURVE].plantLocation, KOCH_CURVE);

	plants[SIERPINSKY_TRI].numGenerations = 3;
	plants[SIERPINSKY_TRI].plantLocation = Ogre::Vector3(1681, 49.7f, 2114);
	plants[SIERPINSKY_TRI].plantNode = createPlant("gram3.txt", plants[SIERPINSKY_TRI].numGenerations,
		plants[SIERPINSKY_TRI].plantLocation, SIERPINSKY_TRI);

	// create material for the selector cube
	selectMat = Ogre::MaterialManager::getSingleton().create(Ogre::String("selectMat").c_str(),
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	selectMat->setAmbient(1.0f, 0.0f, 0.0f);
	selectMat->setDiffuse(1.0f, 0.0f, 0.0f, 1.0f);
	selectMat->setSpecular(0.2f, 0.0f, 0.0f, 1.0f);

	// create the selector cube mesh & set its material
	selectorCube = mSceneMgr->createEntity("cube.mesh");
	selectorCube->setMaterial(selectMat);

	// create the selector cube node & attach its mesh
	selection = PYTHAGORAS_TREE;
	//cubeNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(1683, 49.7f, 2115));
	cubeNode = plants[PYTHAGORAS_TREE].plantNode->createChildSceneNode(Ogre::Vector3(0.0f, -12.0f, 0.0f));
	cubeNode->attachObject(selectorCube);
	cubeNode->setScale(0.2f, 0.06f, 0.1f);

	mCamera->setPosition(Ogre::Vector3(1683, 50, 2116));

	Ogre::Vector3 lightdir(0.55f, -0.3f, 0.75f);
	lightdir.normalise();

	Ogre::Light* light = mSceneMgr->createLight("tstLight");
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
}
//-------------------------------------------------------------------------------------
void Application::createFrameListener(void)
{
	BaseApplication::createFrameListener();

	mInfoLabel = mTrayMgr->createLabel(OgreBites::TL_TOP, "TInfo", "", 350);
}
//-------------------------------------------------------------------------------------
bool Application::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	bool ret = BaseApplication::frameRenderingQueued(evt);

	if (mTerrainGroup->isDerivedDataUpdateInProgress())
	{
		mTrayMgr->moveWidgetToTray(mInfoLabel, OgreBites::TL_TOP, 0);
		mInfoLabel->show();
		if (mTerrainsImported)
		{
			mInfoLabel->setCaption("Building terrain, please wait...");
		}
		else
		{
			mInfoLabel->setCaption("Updating textures, patience...");
		}
	}
	else
	{
		mTrayMgr->removeWidgetFromTray(mInfoLabel);
		mInfoLabel->hide();
		if (mTerrainsImported)
		{
			mTerrainGroup->saveAllTerrains(true);
			mTerrainsImported = false;
		}
	}

	if (sub) sub->update(evt.timeSinceLastFrame);

	// update camera position - TODO: remove this when project is integrated
	Ogre::Vector3 directionX = mCamera->getOrientation() * Ogre::Vector3::UNIT_X;
	Ogre::Vector3 directionY = - (mCamera->getOrientation() * Ogre::Vector3::UNIT_Z);
	Ogre::Vector3 newPosition = mCamera->getPosition() + directionX * cameraVelocity.x * evt.timeSinceLastFrame + 
		directionY * cameraVelocity.y * evt.timeSinceLastFrame;
	mCamera->setPosition(newPosition);

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
	mCamera->yaw(Ogre::Degree(-0.13f * evt.state.X.rel));
	mCamera->pitch(Ogre::Degree(-0.13f * evt.state.Y.rel));
	return true;
}

bool Application::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	return true;
}

bool Application::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
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
	else if (arg.key == OIS::KC_A)
	{
		//if (sub) sub->turnRight(true);
		cameraVelocity.x = -1.0f;
	}
	else if (arg.key == OIS::KC_D)
	{
		//if (sub) sub->turnRight();
		cameraVelocity.x = 1.0f;
	}
	else if (arg.key == OIS::KC_W)
	{
		//if (sub) sub->moveFront();
		cameraVelocity.y = 1.0f;
	}
	else if (arg.key == OIS::KC_S)
	{
		//if (sub) sub->moveFront(true);
		cameraVelocity.y = -1.0f;
	}
	else if (arg.key == OIS::KC_ADD)
	{
		cubeNode->detachAllObjects();
		mSceneMgr->destroySceneNode(cubeNode);
		plants[selection].numGenerations++;
		mSceneMgr->destroySceneNode(plants[PYTHAGORAS_TREE].plantNode);
		mSceneMgr->destroySceneNode(plants[KOCH_CURVE].plantNode);
		mSceneMgr->destroySceneNode(plants[SIERPINSKY_TRI].plantNode);
		plants[PYTHAGORAS_TREE].plantNode = createPlant("gram1.txt", plants[PYTHAGORAS_TREE].numGenerations,
			plants[PYTHAGORAS_TREE].plantLocation, PYTHAGORAS_TREE);
		plants[KOCH_CURVE].plantNode = createPlant("gram2.txt", plants[KOCH_CURVE].numGenerations,
			plants[KOCH_CURVE].plantLocation, KOCH_CURVE);
		plants[SIERPINSKY_TRI].plantNode = createPlant("gram3.txt", plants[SIERPINSKY_TRI].numGenerations,
			plants[SIERPINSKY_TRI].plantLocation, SIERPINSKY_TRI);
		cubeNode = plants[selection].plantNode->createChildSceneNode(Ogre::Vector3(0.0f, -12.0f, 0.0f));
		cubeNode->attachObject(selectorCube);
		cubeNode->setScale(0.2f, 0.06f, 0.1f);
	}
	else if (arg.key == OIS::KC_SUBTRACT)
	{
		cubeNode->detachAllObjects();
		mSceneMgr->destroySceneNode(cubeNode);
		if (plants[selection].numGenerations > 0) plants[selection].numGenerations--;
		mSceneMgr->destroySceneNode(plants[PYTHAGORAS_TREE].plantNode);
		mSceneMgr->destroySceneNode(plants[KOCH_CURVE].plantNode);
		mSceneMgr->destroySceneNode(plants[SIERPINSKY_TRI].plantNode);
		plants[PYTHAGORAS_TREE].plantNode = createPlant("gram1.txt", plants[PYTHAGORAS_TREE].numGenerations,
			plants[PYTHAGORAS_TREE].plantLocation, PYTHAGORAS_TREE);
		plants[KOCH_CURVE].plantNode = createPlant("gram2.txt", plants[KOCH_CURVE].numGenerations,
			plants[KOCH_CURVE].plantLocation, KOCH_CURVE);
		plants[SIERPINSKY_TRI].plantNode = createPlant("gram3.txt", plants[SIERPINSKY_TRI].numGenerations,
			plants[SIERPINSKY_TRI].plantLocation, SIERPINSKY_TRI);
		cubeNode = plants[selection].plantNode->createChildSceneNode(Ogre::Vector3(0.0f, -12.0f, 0.0f));
		cubeNode->attachObject(selectorCube);
		cubeNode->setScale(0.2f, 0.06f, 0.1f);
	}
	else if (arg.key == OIS::KC_LEFT)
	{
		selection = ++selection % 3;

		cubeNode->detachAllObjects();

		mSceneMgr->destroySceneNode(cubeNode);
		cubeNode = plants[selection].plantNode->createChildSceneNode(Ogre::Vector3(0.0f, -12.0f, 0.0f));
		cubeNode->attachObject(selectorCube);
		cubeNode->setScale(0.2f, 0.06f, 0.1f);
	}
	else if (arg.key == OIS::KC_RIGHT)
	{
		if (--selection < 0)
			selection += 3;

		cubeNode->detachAllObjects();

		mSceneMgr->destroySceneNode(cubeNode);
		cubeNode = plants[selection].plantNode->createChildSceneNode(Ogre::Vector3(0.0f, -12.0f, 0.0f));
		cubeNode->attachObject(selectorCube);
		cubeNode->setScale(0.2f, 0.06f, 0.1f);
	}

	return true;
}
//---------------------------------------------------------------------------
bool Application::keyReleased(const OIS::KeyEvent &arg)
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

	return true;
}

Ogre::SceneNode* Application::createPlant(const std::string& filename, unsigned int numGenerations,
	Ogre::Vector3 position, SystemType type)
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
		std::vector<Node*> symbols;
		std::string name;
		Ogre::Real angle;
		switch (type)
		{
		case PYTHAGORAS_TREE:
			idNode = new IdentifierNode("A");
			name = "PYTHAGORAS TREE";
			angle = Ogre::Math::PI / 4;
			break;
		case KOCH_CURVE:
			idNode = new IdentifierNode("F");
			name = "KOCH CURVE";
			angle = Ogre::Math::PI / 2;
			break;
		case SIERPINSKY_TRI:
			idNode = new IdentifierNode("A");
			name = "SIERPINSKY TRIANGLE";
			angle = Ogre::Math::PI / 3;
			break;
		default:
			idNode = 0;
			break;
		}
		if (idNode) symbols.push_back(idNode);

		// Renderer visitor - will create our entities according to the grammar nodes it visits
		Ogre::SceneNode* sceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		RendererVisitor rendererVis(mSceneMgr, sceneNode, position, name, angle);

		// Evolve the generations
		for (unsigned int i = 0; i < numGenerations; i++)
		{
			GrammarGenerator::generate(grammar, symbols);
			GrammarGenerator::printSymbols(symbols);
		}

		// Semantic processing of resulting parse tree, adding entities to ogre
		for (std::vector<Node*>::iterator it = symbols.begin(); it != symbols.end(); it++)
			(*it)->accept(&rendererVis);

		return sceneNode;
	}
	else
	{
		std::cout << "Error: unable to open file: " << filename << std::endl;
		return 0;
	}
}