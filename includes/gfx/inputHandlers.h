/*
 * keyHandler.h
 *
 *  Created on: Oct 15, 2012
 *      Author: sushil
 */

#ifndef INPUTHANDLER_H_
#define INPUTHANDLER_H_

#include "DEBUG.h"

#include <set>

#include <OISEvents.h>
#include <OISMouse.h>
#include <OISKeyboard.h>


namespace OgreGFX {

	class GraphicsInteractionManager;

	class KeyHandler {
	protected:
		const std::set<OIS::KeyCode> *modifiers;
		OIS::KeyCode key;
	public:
		GraphicsInteractionManager *gfxMgr;
		KeyHandler(GraphicsInteractionManager *gim, const std::set<OIS::KeyCode> *modifierKeys, const OIS::KeyCode keyCode){
			gfxMgr    = gim;
			modifiers = modifierKeys;
			key       = keyCode;
		}

		virtual void handleKeyEvent() {
			DEBUG(std::cout << "Handling key: " << key << std::endl;)
		}

	};

	class MouseHandler {
	public:
		GraphicsInteractionManager *gfxMgr;
		MouseHandler(GraphicsInteractionManager *gim, const std::set<OIS::KeyCode> *modifierKeys, const OIS::MouseButtonID oisMouseButtonId){
			gfxMgr    = gim;
			modifiers = modifierKeys;
			mouseButton = oisMouseButtonId;
		}
		virtual void handleMouseEvent(const OIS::MouseEvent &arg) {
			DEBUG(std::cout << "Handling mousebutton: " << mouseButton << std::endl;)
		}

	protected:
		const std::set<OIS::KeyCode> *modifiers;
		OIS::MouseButtonID mouseButton;
	};

}


#endif /* KEYHANDLER_H_ */
