/*
 * gfxThread.cpp
 *
 *  Created on: Dec 20, 2011
 *      Author: sushil
 */

#include <boost/thread.hpp>
#include "DEBUG.h"
#include <engine.h>
#include <gfxThread.h>

void FastEcslent::GfxThread::init() {
	DEBUG(std::cout << "Initialized Gfx before starting thread. Engine instance Id: " << engine->instanceId << std::endl;)
}

void FastEcslent::GfxThread::run() {

	gfx = boost::thread(&FastEcslent::GfxThread::runThread, this);
	DEBUG(std::cout << "Started Gfx thread's runThread method" << gfx.get_id() << std::endl;)

}

void FastEcslent::GfxThread::runThread(){

	gimPtr = new OgreGFX::GraphicsInteractionManager(engine); //&gim;
	DEBUG(std::cout << "Ogre: created TA, getting ready to run:" << gfx.get_id() << std::endl;)

	gimPtr->go();
	DEBUG(std::cout << "Ogre finishing: shutting down" << gfx.get_id() << std::endl;)
}

void FastEcslent::GfxThread::stop(){

}

void FastEcslent::GfxThread::join(){
	DEBUG(std::cout << "Ogre: Joining getting ready to stop:" << gfx.get_id() << std::endl;)
	gfx.join();
}


void FastEcslent::GfxThread::kill(){
	gfx.interrupt();
}
