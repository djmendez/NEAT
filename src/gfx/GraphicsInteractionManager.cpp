/*
 * GraphicsInteractionManager.cpp
 *
 *  Created on: Dec 20, 2011
 *      Author: sushil
 ----------------------------------------------------------------------------
Filename:    GraphicsInteractionManager.cpp originally  named BaseApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/

#include <OgreMeshManager.h>
#include <OgreEntity.h>
#include <OgreSceneNode.h>

//#include <engine.h>
#include <GraphicsInteractionManager.h>
#include <uiMgr.h>
#include <unitWeapons.h>
#include <map>
#include <snd/SoundMgr.h>
#include <enums.h>
#include <levelMgr.h>


using namespace OgreGFX;

//-------------------------------------------------------------------------------------
GraphicsInteractionManager::GraphicsInteractionManager(FastEcslent::Engine* eng):
	mRoot(0),
	mSceneMgr(0),
	mWindow(0),
	mCamera(0),
	mResourcesCfg(Ogre::StringUtil::BLANK),
	mPluginsCfg(Ogre::StringUtil::BLANK),
	mCursorWasVisible(false),
	mShutDown(false),
    mSceneReady(false)
{
	this->engine = eng;
}

//-------------------------------------------------------------------------------------
GraphicsInteractionManager::~GraphicsInteractionManager(void)
{
    //if (mTrayMgr) delete mTrayMgr;
    //if (mCameraMan) delete mCameraMan;

    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    delete mRoot;
}

//-------------------------------------------------------------------------------------
bool GraphicsInteractionManager::configure(void)
{
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
	if (!(mRoot->restoreConfig() || mRoot->showConfigDialog())){
		return false;
	}
	mWindow = mRoot->initialise(true, "FastEcslent::GraphicsInteractionManager's Render Window");
	return true;

}
//-------------------------------------------------------------------------------------
void GraphicsInteractionManager::chooseSceneManager(void)
{
    // Get the SceneManager, in this case a generic one
	//mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager");
	Ogre::OverlaySystem *ovSys = new Ogre::OverlaySystem();
	mSceneMgr->addRenderQueueListener(ovSys);
}
//-------------------------------------------------------------------------------------
void GraphicsInteractionManager::createCamera(void)
{
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

    // Position it at 500 in Z direction
    //mCamera->setPosition(Ogre::Vector3(100,500,100));
    // Look back along -Z
    //mCamera->lookAt(Ogre::Vector3(0,0,0));
    mCamera->setNearClipDistance(5);

    //mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}
//-------------------------------------------------------------------------------------
//void GraphicsInteractionManager::createFrameListener(void)
void GraphicsInteractionManager::createUIMgr(void)
{
	uiMgr = new OgreGFX::UIMgr(this);
        mRoot->addFrameListener(uiMgr);
}

void GraphicsInteractionManager::createSoundMgr(void)
{
	this->soundMgr = new OgreSND::SoundMgr(this);
	mRoot->addFrameListener(this->soundMgr);
        this->soundMgr->initialize();
}

void GraphicsInteractionManager::createWidgetMgr(void)
{

	widgetMgr = new OgreGFX::WidgetMgr(this);
	mRoot->addFrameListener(widgetMgr);
}

void GraphicsInteractionManager::createParticleSystem(void)
{
	explosionParticleSystem = mSceneMgr->createParticleSystem("explode", "explosionTemplate");
	explosionParticleSystem->setSpeedFactor(50);
}


/*void GraphicsInteractionManager::createInputSystem(void)
{
	inputSystem = new FastEcslent::InputSystem(this, uiMgr->keyboard, uiMgr->mouse);
	mRoot->addFrameListener(inputSystem);
}*/


void GraphicsInteractionManager::loadGameGFX(void) {
	setupGame();
	mRoot->addFrameListener(this);
}

void GraphicsInteractionManager::createFrameListener(void) {
	mRoot->addFrameListener(this);
}

//-------------------------------------------------------------------------------------
void GraphicsInteractionManager::destroyScene(void)
{
}
//-------------------------------------------------------------------------------------
void GraphicsInteractionManager::createViewports(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);

    vp->setVisibilityMask(OgreGFX::PLAYER_CAMERA_MASK);
    //vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}
//-------------------------------------------------------------------------------------
void GraphicsInteractionManager::setupResources(void)
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())  {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i){
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
}
//-------------------------------------------------------------------------------------
void GraphicsInteractionManager::createResourceListener(void)
{

}
//-------------------------------------------------------------------------------------
void GraphicsInteractionManager::loadResources(void)
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//-------------------------------------------------------------------------------------
void GraphicsInteractionManager::go(void)
{
#ifdef _DEBUG
    mResourcesCfg = "config/resources_d.cfg";
    mPluginsCfg = "config/plugins_d.cfg";
#else
    mResourcesCfg = "config/resources.cfg";
    mPluginsCfg = "config/plugins.cfg";
#endif

    if (!setup())
        return;

    mRoot->startRendering();

    // clean up
    destroyScene();
}
//-------------------------------------------------------------------------------------
void GraphicsInteractionManager::initGFXManagers(void) {
	uiMgr->initialize();
	widgetMgr->initialize();
        
	mRoot->addFrameListener(uiMgr);
}

bool GraphicsInteractionManager::setup(void)
{
    mRoot = new Ogre::Root(mPluginsCfg);
    setupResources();
    bool carryOn = configure();
    if (!carryOn) return false;
    chooseSceneManager();
    createCamera();
    createViewports();

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    // Create any resource listeners (for loading screens)
    createResourceListener();
    // Load resources
    loadResources();
    createUIMgr();
    
    createSoundMgr();
    createParticleSystem();

    if(engine->options.enableInteraction){
		//create Lobby Manager
		createLobbyMgr();
		//Load Lobby GUI
		loadLobbyGFX();
    }else{
    	loadGameGFX();
    	engine->gameState = FastEcslent::GAME;
    }

    return true;
};

bool GraphicsInteractionManager::setupGame(void)
{
	createScene();
    createWidgetMgr();
    
    createFrameListener();

    initGFXManagers();

    return true;
};
//-------------------------------------------------------------------------------------

void GraphicsInteractionManager::handleBornEnts(){
	if (this->engine->entityMgr->nEnts > this->nGFXNodes) {
		for (int i = nGFXNodes; i < this->engine->entityMgr->nEnts; i++){
				makeNode(this->engine->entityMgr->ents[i]);
		}
		this->nGFXNodes = engine->entityMgr->nEnts;
	}
}


void GraphicsInteractionManager::handleDevelopingEnts(){

}


bool GraphicsInteractionManager::frameRenderingQueued(const Ogre::FrameEvent& evt){
    if(mWindow->isClosed())
        return false;
    mShutDown = uiMgr->shutDown;
    if(mShutDown){
    	if(uiMgr) {
    		uiMgr->kill();
    		delete uiMgr;
    		boost::mutex::scoped_lock scoped_lock(engine->quitLock);
    		engine->quit = true;
    	}
        return false;
    }

    handleDevelopingEnts(); // create nodes
    handleBornEnts();       // move   nodes
    copySimState(); //handleLivingEnts

    return true;
}

//Adjust mouse clipping area
void GraphicsInteractionManager::windowResized(Ogre::RenderWindow* rw){
	   //check input
	   unsigned int width, height, depth;
	   int left, top;
	   rw->getMetrics(width, height, depth, left, top);

	   const OIS::MouseState &ms = uiMgr->mouse->getMouseState();
	   ms.width = width;
	   ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void GraphicsInteractionManager::windowClosed(Ogre::RenderWindow* rw){
    //Only close for window that created OIS (the main window in these demos)
    if( rw == mWindow ) {
    	if(uiMgr) {
    		uiMgr->kill();
    	}
    }
}



void GraphicsInteractionManager::makeInitialEntityNodes(){
	this->nGFXNodes = engine->entityMgr->nEnts;
	std::cout << "GFX: Number of ents: " << nGFXNodes << std::endl;
	for (int i = 0; i < nGFXNodes; i++){
		makeNode(engine->entityMgr->ents[i]);
	}
}


void GraphicsInteractionManager::makeNode(FastEcslent::Entity *feEnt){
	std::cout << "Name: " << feEnt->uiname << std::endl;
	Ogre::Entity* ent = mSceneMgr->createEntity(feEnt->uiname, feEnt->meshName);

	if(feEnt->entityId.side == FastEcslent::RED && feEnt->entityClass == FastEcslent::SURFACE){
		ent->setMaterialName("color2/SOLID/TEX/ecsl.bmp");
	}else if(feEnt->entityId.side == FastEcslent::BLUE &&  feEnt->entityClass == FastEcslent::SURFACE){
		ent->setMaterialName("RED/SOLID/TEX/ecslDark.bmp/VertCol");
	}

	ent->setVisibilityFlags(OgreGFX::ENT_VISIBILITY_FLAG);
//		if (engine->entityMgr->ents[i]->entityClass == FastEcslent::STATIC){
//			ent->setVisibilityFlags(OgreGFX::STATIC_VISIBILITY_FLAG);
//		}
	std::cout << "GFX creating: " << feEnt->uiname << std::endl;

	Ogre::SceneNode* sceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(feEnt->pos);
	if(feEnt->meshName == "drone.mesh")
	{
		sceneNode->setScale(0.3, 0.3, 0.3);
	}

	std::stringstream ss;
	ss << feEnt->uiname << "_mini" ;
	Ogre::Entity* minimapEnt = mSceneMgr->createEntity(ss.str(), "cube.mesh" );
	minimapEnt->setVisibilityFlags(OgreGFX::MINIMAP_VISIBILITY_FLAG);
	Ogre::SceneNode* minimapNode = sceneNode->createChildSceneNode();
	minimapNode->attachObject(minimapEnt);
	minimapNode->setScale(2.0, 2.0, 2.0);
	int id = feEnt->entityId.id;
	gfxNodes[id].node = sceneNode;
	gfxNodes[id].node->attachObject(ent);
	gfxNodes[id].node->resetOrientation();
	gfxNodes[id].node->yaw(Ogre::Radian(feEnt->heading));
	gfxNodes[id].id = id;
	gfxNodes[id].selectable = feEnt->selectable;
        gfxNodes[id].entType = feEnt->entityType;
	sceneNodeToEntIdMap[sceneNode] = id;


}


void GraphicsInteractionManager::createScene(){

	//this->mShutDown = false;
	Ogre::Entity*         mOceanSurfaceEnt;
	makeInitialEntityNodes();

	//mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.8, 0.8, 0.8));
	Ogre::Light* l1 = mSceneMgr->createLight("LightOne");
	l1->setPosition(20, 80, 50);

	//mSceneMgr->setSkyBox(true, "SkyBox", 1000);
	mSceneMgr->setSkyDome(true, "Examples/CloudySky", 10, 8);
	//mSceneMgr->setSkyBox(true, "Examples/CloudyNoonSkyBox", 50000, false);


    // Define a plane mesh that will be used for the ocean surface
	//Ogre::Plane oceanSurface;
    FastEcslent::Level level = engine->levelMgr->getCurrentLevel();
    
    oceanSurface.normal = Ogre::Vector3::UNIT_Y;
    oceanSurface.d = 0;
    Ogre::MeshManager::getSingleton().createPlane("OceanSurface",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        oceanSurface,
        level.width * 512, level.height * 512, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);

    mOceanSurfaceEnt = mSceneMgr->createEntity( "OceanSurface", "OceanSurface" );
    Ogre::SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject(mOceanSurfaceEnt);
    node->setPosition(FastEcslent::LevelMgr::getWorldOrigin());
    mOceanSurfaceEnt->setCastShadows(false);
    mOceanSurfaceEnt->setMaterialName("OceanHLSL_GLSL");

    //mOceanSurfaceEnt->setMaterialName("OceanCg");
    //mOceanSurfaceEnt->setMaterialName("Ocean2_Cg");
    
    mSceneReady = true;
}

void GraphicsInteractionManager::explode(Ogre::SceneNode *node){
	explosionParticleSystem->getEmitter(0)->setEnabled(true);
	node->detachObject(explosionParticleSystem);
	node->attachObject(explosionParticleSystem);
}

void GraphicsInteractionManager::copySimState(){

	for(int i = 0; i < nGFXNodes; i++){
		//std::cout << engine->entityMgr->ents[i]->pos.x << ", " << engine->entityMgr->ents[0]->pos.y << ", " <<  engine->entityMgr->ents[0]->pos.z << std::endl;
		gfxNodes[i].node->setPosition(engine->entityMgr->ents[i]->pos);//.x, engine->entityMgr->ents[0]->pos.y, engine->entityMgr->ents[0]->pos.z);
		gfxNodes[i].node->resetOrientation();
		gfxNodes[i].node->yaw(Ogre::Radian(engine->entityMgr->ents[i]->heading));
                gfxNodes[i].selectable = engine->entityMgr->ents[i]->selectable;

		//Check the entity state, if the entity is being attacked, show the explosion on the entity and reset the flag.
		FastEcslent::Weapon *uw = dynamic_cast<FastEcslent::Weapon *>(engine->entityMgr->ents[i]->getAspect(FastEcslent::WEAPON));
		if(uw->isBeingAttacked()){
			uw->resetBeingAttack();
			this->explode(gfxNodes[i].node);
                        this->soundMgr->playExplosionSound(&(gfxNodes[i]));
		}
	}
}

void GraphicsInteractionManager::createLobbyMgr()
{
	lobbyMgr = new LobbyMgr(engine, this, uiMgr->mouse, uiMgr->keyboard);
}

void GraphicsInteractionManager::loadLobbyGFX(void) {
	mRoot->addFrameListener(lobbyMgr);
}
