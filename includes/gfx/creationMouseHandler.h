/*
 * creationMouseHandler.h
 *
 *  Created on: Jan 9, 2013
 *      Author: sushil
 */

#ifndef CREATIONMOUSEHANDLER_H_
#define CREATIONMOUSEHANDLER_H_

#include<set>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include<GraphicsInteractionManager.h>
#include <enums.h>
#include <ent.h>

#include <inputHandlers.h>


namespace OgreGFX {

	class GraphicsInteractionManager;

	class CreationMouseHandler : public MouseHandler {


	public:
		CreationMouseHandler(GraphicsInteractionManager *gim, const std::set<OIS::KeyCode> *mods);
		~CreationMouseHandler();
		virtual void handleMouseEvent(const OIS::MouseEvent &args);
		boost::mutex creationLock;

	};


}


#endif /* CREATIONMOUSEHANDLER_H_ */
