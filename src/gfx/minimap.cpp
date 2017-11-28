/*
 * minimap.cpp
 *
 *  Created on: Jan 20, 2013
 *      Author: sushil
 */



#include <minimap.h>
#include <GraphicsInteractionManager.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreRectangle2D.h>


OgreGFX::Minimap::Minimap(GraphicsInteractionManager *gim): DEFAULT_MINIMAP_UPDATE_PERIOD(0.2) {
	gfx = gim;
	init();

}

OgreGFX::Minimap::~Minimap(void){

}
//I have done something similar. Just use the member function setVisibilityMask and setVisibilityFlag in the respective
//Viewport & ManualObject & Viewport.
//
//By default, all Movableobject (for which ManualObjectpb derives from) are displayed on all viewports because both have the
//flag 0xFFFFFFFF so when they are AND together, it results into true. So to have ManualObject B & C not shown in viewport A,
//simply set ViewportA visibility mask to 0xFFFFFF00, ManualObject A to 0xF00, ManualObject B to 0xF0, ManualObject C to 0xF.
//Viewport B should be set to 0xFFFF0F0, and C is 0xFFFFF00F.
//
//You can actually 'save more mask' by translating to binary number and set the appropriate mask there, but if you don't
//need a lot of visbility category, the hexadecimal example above is much more 'readable'.


void OgreGFX::Minimap::init(void) {

	rttt = Ogre::TextureManager::getSingleton().createManual("MinimapTex",
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Ogre::TEX_TYPE_2D, gfx->mWindow->getWidth(), gfx->mWindow->getHeight(), 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);
	renderTexture = rttt->getBuffer()->getRenderTarget();

	minimapCamera = gfx->mSceneMgr->createCamera("MinimapCamera");
	minimapCamera->setNearClipDistance(5);
	minimapCamera->setAspectRatio((double)gfx->mWindow->getViewport(0)->getActualWidth()/(double)gfx->mWindow->getViewport(0)->getActualHeight());
	Ogre::Vector3 cpos = gfx->mCamera->getPosition();
	minimapCamera->setPosition(5000, 2000, 5000);

	minimapCamera->pitch(Ogre::Radian(-1.5708));;
	renderTexture->addViewport(minimapCamera);
	renderTexture->getViewport(0)->setClearEveryFrame(true);
	renderTexture->getViewport(0)->setBackgroundColour(Ogre::ColourValue::Black);
	renderTexture->getViewport(0)->setOverlaysEnabled(false);
	renderTexture->getViewport(0)->setVisibilityMask(OgreGFX::MINIMAP_CAMERA_MASK);
	renderTexture->setAutoUpdated(false);

	minimap = new Ogre::Rectangle2D(true);
	minimap->setCorners(-1.0, -0.6, -0.5, -1.0);

	minimap->setBoundingBox(Ogre::AxisAlignedBox(-100000.0f * Ogre::Vector3::UNIT_SCALE, 100000.0f * Ogre::Vector3::UNIT_SCALE));
	minimapSceneNode = gfx->mSceneMgr->getRootSceneNode()->createChildSceneNode("MinimapSceneNode");
	minimapSceneNode->attachObject(minimap);

	renderMaterial = Ogre::MaterialManager::getSingleton().create("MinimapMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	renderMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	renderMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("MinimapTex");

	minimap->setMaterial("MinimapMaterial");

	renderTexture->addListener(this);
	gfx->mRoot->addFrameListener(this);
	renderTexture->update();
	updatePeriod = DEFAULT_MINIMAP_UPDATE_PERIOD;

}

bool OgreGFX::Minimap::frameRenderingQueued(const Ogre::FrameEvent& evt){
	updatePeriod -= evt.timeSinceLastFrame;
	if(updatePeriod < 0){
		renderTexture->update();
		updatePeriod = DEFAULT_MINIMAP_UPDATE_PERIOD;
	}
	Ogre::Vector3 cpos = gfx->mCamera->getPosition();
	minimapCamera->setPosition(cpos.x+500, 5000, cpos.z+500);


	return true;
}



//make minimap invisible
void OgreGFX::Minimap::preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt){
	minimap->setVisible(false);


}
// snap view from minimapCamera without the minimap being visible!

void OgreGFX::Minimap::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt){
	minimap->setVisible(true);
}
//Then make minimap visible
