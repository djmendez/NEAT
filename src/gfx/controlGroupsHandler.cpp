/*
 * controlGroupHandler.cpp
 *
 *  Created on: Jan 15, 2013
 *      Author: sushil
 */


#include <controlGroupsHandler.h>
#include <GraphicsInteractionManager.h>
#include "DEBUG.h"

OgreGFX::ControlGroupsHandler::ControlGroupsHandler(GraphicsInteractionManager *gim){
	this->gfx = gim;
	DEBUG(std::cout << "Handling control group handler initialization" << std::endl;)
	for (int i = 0; i < MAX_CONTROL_GROUPS; i++){
		groups[i] = std::list<OgreGFX::GFXNode *>();
		groups[i].clear();
	}
	OISKeyToGroupIndexMap.clear();
	OISKeyToGroupIndexMap[OIS::KC_0] = 0;
	OISKeyToGroupIndexMap[OIS::KC_1] = 1;
	OISKeyToGroupIndexMap[OIS::KC_2] = 2;
	OISKeyToGroupIndexMap[OIS::KC_3] = 3;
	OISKeyToGroupIndexMap[OIS::KC_4] = 4;
	OISKeyToGroupIndexMap[OIS::KC_5] = 5;
	OISKeyToGroupIndexMap[OIS::KC_6] = 6;
	OISKeyToGroupIndexMap[OIS::KC_7] = 7;
	OISKeyToGroupIndexMap[OIS::KC_8] = 8;
	OISKeyToGroupIndexMap[OIS::KC_9] = 9;
	return;
}


OgreGFX::ControlGroupsHandler::~ControlGroupsHandler(){
}

void OgreGFX::ControlGroupsHandler::checkHandleControlGroupKeys(bool ctrlDown, OIS::KeyCode key){

	switch (key) {
	case OIS::KC_0:
	case OIS::KC_1:
	case OIS::KC_2:
	case OIS::KC_3:
	case OIS::KC_4:
	case OIS::KC_5:
	case OIS::KC_6:
	case OIS::KC_7:
	case OIS::KC_8:
	case OIS::KC_9:
		DEBUG(std::cout << "Handling control key : " << key << ", " << this->OISKeyToGroupIndexMap[key] << std::endl;)
		if (ctrlDown){
			createControlGroup(this->OISKeyToGroupIndexMap[key]);
		} else {
			selectControlGroup(this->OISKeyToGroupIndexMap[key]);
		}
		break;
	default:
		//do nothing
		break;
	}


}

void OgreGFX::ControlGroupsHandler::createControlGroup(int mkey){
	groups[mkey].clear();
	for(std::list<OgreGFX::GFXNode *>::iterator it = this->gfx->uiMgr->currentSelections.begin();
				it != this->gfx->uiMgr->currentSelections.end(); ++it){
		groups[mkey].push_back(*it);
	}
	gfx->uiMgr->printCurrentSelections();
}


void OgreGFX::ControlGroupsHandler::selectControlGroup(int mkey){

	this->gfx->uiMgr->clearSelectionsAndUpdateFEEngine();
	for(std::list<OgreGFX::GFXNode *>::iterator it = this->groups[mkey].begin();
				it != this->groups[mkey].end(); ++it){
		this->gfx->uiMgr->currentSelections.push_back(*it);
	}
	gfx->uiMgr->printCurrentSelections();
	this->gfx->uiMgr->updateFEEngineWithSelections();



}

