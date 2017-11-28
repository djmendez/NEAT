/*
 * creationMouseHandler.cpp
 *
 *  Created on: Jan 9, 2013
 *      Author: sushil
 */

#include "DEBUG.h"
#include <creationMouseHandler.h>

OgreGFX::CreationMouseHandler::CreationMouseHandler(GraphicsInteractionManager *gim, const std::set<OIS::KeyCode> *mods) :
	OgreGFX::MouseHandler(gim, mods, OIS::MB_Left) {

	DEBUG(std::cout << "Constructed the creation handler" << std::endl;)
}


OgreGFX::CreationMouseHandler::~CreationMouseHandler() {
}

void OgreGFX::CreationMouseHandler::handleMouseEvent(const OIS::MouseEvent &arg) {
	DEBUG(std::cout << "Called MouseHandler for Ctrl-LeftMouseButton" << std::endl;)
//	boost::mutex::scoped_lock scoped_lock(creationLock);
	FastEcslent::Entity *ent = this->gfxMgr->engine->entityMgr->createEntityForPlayerAndSide(FastEcslent::TANK, this->gfxMgr->uiMgr->posUnderMouse, 0.0, FastEcslent::BLUE, FastEcslent::ONE);
}
