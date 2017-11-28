/*
 * gfxNode.h
 *
 *  Created on: Feb 20, 2012
 *      Author: sushil
 */

#ifndef GFXNODE_H_
#define GFXNODE_H_

#include <OgreSceneNode.h>
#include "enums.h"

namespace OgreGFX {

	typedef struct {
		Ogre::SceneNode* node;
		bool  selectable;
		int   id;
                FastEcslent::EntityType entType;
	} GFXNode;

}


#endif /* GFXNODE_H_ */
