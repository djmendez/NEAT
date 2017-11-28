/*
 * cameraMgr.h
 *
 *  Created on: Feb 19, 2012
 *      Author: sushil
 */
#ifndef CAMERAMGR_H_
#define CAMERAMGR_H_

#include <OgreVector3.h>
#include <OISEvents.h>
//#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <gfxMgr.h>

namespace OgreGFX {

	class GraphicsInteractionManager;

	class CameraMgr : public GFXMgr {
	public:
		Ogre::Vector3 velocity;
		Ogre::SceneNode* yawNode;

		Ogre::SceneNode* pitchNode;
		Ogre::SceneNode* cameraNode;

		Ogre::SceneNode* mTarget;
		bool orbiting;
		bool zooming;
		float topSpeed;
		float topRotation;

		bool forward;
		bool back;
		bool left;
		bool right;
		bool up;
		bool down;
		bool yawLeft;
		bool yawRight;
		bool pitchUp;
		bool pitchDown;

		Ogre::Vector3 mouseScrollTranslation;
		Ogre::Vector3 mouseScrollTranslationSrc;
		Ogre::Vector3 zeroTranslation;

		float lerpTime;
		float timeToStop;
		float mouseScrollSpeed;
                float maxHeight;
                float minHeight;

		bool fastMove;

		Ogre::Camera* camera;
		Ogre::SceneManager* sceneManager;

		CameraMgr(GraphicsInteractionManager *gim);//Engine* eng, Ogre::SceneManager *sm, Ogre::Camera* cam);
		void injectKeyDown(const OIS::KeyEvent& evt);
		void injectKeyUp(const OIS::KeyEvent& evt);
		bool frameRenderingQueued(const Ogre::FrameEvent& evt);
		void handleMouseScrollCameraZoom(const OIS::MouseEvent &e);


	};
}

#endif
