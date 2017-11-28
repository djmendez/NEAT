/*
 * controlGroupHandler.h
 *
 *  Created on: Jan 15, 2013
 *      Author: sushil
 */

#ifndef CONTROLGROUPHANDLER_H_
#define CONTROLGROUPHANDLER_H_

#include<list>
#include<map>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <gfxNode.h>



namespace OgreGFX {

	class GraphicsInteractionManager;

	class ControlGroupsHandler {

	private:
		static const int MAX_CONTROL_GROUPS = 10;
		GraphicsInteractionManager *gfx;
		//std::list <OgreGFX::GFXNode*>  group0;
		std::list <OgreGFX::GFXNode*>  groups[MAX_CONTROL_GROUPS]; //0 - 9
		std::map <OIS::KeyCode, int> OISKeyToGroupIndexMap;

	public:
		ControlGroupsHandler(GraphicsInteractionManager *gim);
		~ControlGroupsHandler();
		void checkHandleControlGroupKeys(bool ctrlDown, OIS::KeyCode k);
		void createControlGroup(int k);
		void selectControlGroup(int k);

	};
}


#endif /* CONTROLGROUPHANDLER_H_ */
