/*
 * GraphicsInteractionManager.h
 *
 *  Created on: Dec 20, 2011
 *      Author: sushil
 */


#ifndef GRAPHICSINTERACTIONMANAGER_H_
#define GRAPHICSINTERACTIONMANAGER_H_


#include <map>

#include <OgreCamera.h>

#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreSceneNode.h>

#include <OgreFrameListener.h>
#include <OgreWindowEventUtilities.h>

#include <gfxNode.h>
#include <const.h>
#include <uiMgr.h>
#include <widgetMgr.h>
#include <gfxConst.h>

#include <lobbyMgr.h>
#include <OgreParticleSystem.h>
#include <OgreParticleEmitter.h>

extern const int FastEcslent::MaxEnts;
namespace OgreSND { // If you are in a different namespace you need to be forward declared here
	class SoundMgr;
}

namespace OgreGFX {

	class Engine;
        class WidgetMgr;
	class GraphicsInteractionManager : public Ogre::FrameListener, public Ogre::WindowEventListener {
	public:
		GraphicsInteractionManager(FastEcslent::Engine* eng);
		~GraphicsInteractionManager(void);

		void go(void);
		FastEcslent::Engine* engine;
		UIMgr*  uiMgr;
		WidgetMgr* widgetMgr;
		LobbyMgr *lobbyMgr;
                OgreSND::SoundMgr* soundMgr;

		Ogre::Root *mRoot;
		Ogre::SceneManager* mSceneMgr;
		Ogre::RenderWindow* mWindow;
		Ogre::Camera* mCamera;
                bool mSceneReady;

		GFXNode          gfxNodes[FastEcslent::MaxEnts];
		std::map<Ogre::SceneNode*, int> sceneNodeToEntIdMap;
		int              nGFXNodes;

		Ogre::Plane oceanSurface;

		void loadGameGFX(void);

		// Ogre::FrameListener
		bool frameRenderingQueued(const Ogre::FrameEvent& evt);
		// Ogre::WindowEventListener
		//Adjust mouse clipping area
		void windowResized(Ogre::RenderWindow* rw);
		//Unattach OIS before window shutdown (very important under Linux)
		void windowClosed(Ogre::RenderWindow* rw);

		void makeNode(FastEcslent::Entity *);

	private:
		void initGFXManagers();
		bool configure(void);
		void chooseSceneManager(void);
		void createCamera(void);
                void createFrameListener(void);

		void createUIMgr(void);
		void createWidgetMgr(void);
                void createSoundMgr(void);

		void createScene(void);// = 0; // Override me!
		void destroyScene(void);
		void createViewports(void);
		void setupResources(void);
		void createResourceListener(void);
		void loadResources(void);

		bool setup();
		bool setupGame();
		void createLobbyMgr(void);
		void loadLobbyGFX(void);

		//Simulation bridge
		void makeInitialEntityNodes();

		void handleDevelopingEnts();
		void handleBornEnts();
		void copySimState();

		/*
		 * Trigger explosion on a certain unit
		 * @param node, entity node where explosion show
		 */
		void explode(Ogre::SceneNode *node);
		/*
		 * Create explosion particle system
		 */
		void createParticleSystem();

		Ogre::String mResourcesCfg;
		Ogre::String mPluginsCfg;

		bool mCursorWasVisible;                    // was cursor visible before dialog appeared
		bool mShutDown;
		//particle system for explosion
		Ogre::ParticleSystem *explosionParticleSystem;
	};

}

#endif /* GRAPHICSINTERACTIONMANAGER_H_ */
