/*
 * target.h
 *
 *  Created on: Dec 21, 2011
 *      Author: sushil
 */

#ifndef TARGET_H_
#define TARGET_H_


#include <OgreVector3.h>
//#include <ent.h>

namespace FastEcslent {

	class Entity;
	class Group;

	typedef struct {
		Ogre::Vector3 location;
		//or
		Entity       *entity;
		//or
		double        waitTime; //time to wait

		Ogre::Vector3 offset; //raw offset position (say from UI) must be converted to
		float offsetYaw; // offset relative to heading and
		float offsetDistance; // distance along offsetRotation

	} Target;

	typedef struct {
		Target target;
		Group* group;
	} GroupTarget;


}

#endif /* TARGET_H_ */
