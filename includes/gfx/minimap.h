/*
 * minimap.h
 *
 *  Created on: Jan 20, 2013
 *      Author: sushil
 */

#ifndef MINIMAP_H_
#define MINIMAP_H_

#include <OgreFrameListener.h>
#include <OgreRenderTargetListener.h>
#include <OgreRectangle2D.h>
#include <OgreTexture.h>
#include <OgreRenderTexture.h>
#include <OgreSceneNode.h>
#include <OgreMaterialManager.h>

namespace OgreGFX {

	class GraphicsInteractionManager;

	class Minimap : public Ogre::FrameListener, public Ogre::RenderTargetListener {

	public:
		Minimap(GraphicsInteractionManager *gim);
		virtual ~Minimap(void);


	protected:
		virtual bool frameRenderingQueued(const Ogre::FrameEvent& e);
		virtual void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
		virtual void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);

		void init(void);

		GraphicsInteractionManager *gfx;
	    Ogre::Camera* minimapCamera;


	    //Ogre::MovablePlane *mPlane;
		//Ogre::Entity       *mPlaneEntity;
		//Ogre::SceneNode    *mPlaneNode;

		Ogre::TexturePtr rttt;
		Ogre::RenderTexture *renderTexture;
		Ogre::MaterialPtr   renderMaterial;
		Ogre::Rectangle2D  *minimap;
		Ogre::SceneNode    *minimapSceneNode;

	private:
		Ogre::Camera *createMinimapCamera(void);
		Ogre::Real   updatePeriod;
		const Ogre::Real DEFAULT_MINIMAP_UPDATE_PERIOD;// = 2.0;

	};

}

#endif /* MINIMAP_H_ */
