/*
 * physics2.cpp
 *
 *  Created on: Jan 5, 2012
 *      Author: sushil
 */

#include <assert.h>
//#include <stdio.h>

#include <OgreVector3.h>
#include <OgreQuaternion.h>

#include <physics.h>
#include <ent.h>
#include <utils.h>
#include "DEBUG.h"

void FastEcslent::Physics2D2::init(){
	entity->desiredSpeed = entity->speed;//kInvalidFloat;
	entity->desiredHeading = entity->heading;//kInvalidFloat;

}

/*void FastEcslent::Physics2D2::printVector(const Ogre::Vector3 &v){
	std::cout << "[" << v.x << ", "  << v.y << ", " << v.z << "]" << std::endl;
}*/

void FastEcslent::Physics2D2::doHelmsman(){// You have a desired speed and heading, get me new position and orientation

	if (kInvalidFloat == entity->desiredSpeed || kInvalidFloat == entity->desiredHeading)
		return;

	DEBUG(std::cout << "Speed: " << entity->vel.length() << ", Heading: " << -atan2(entity->vel.z, entity->vel.x ) << std::endl;)
	//FastEcslent::Physics2D2::printVector(entity->vel);
//	Ogre::Vector3 desiredVelocity = Ogre::Vector3(cos(-entity->desiredHeading) * entity->desiredSpeed, 0.0f, sin(-entity->desiredHeading) * entity->desiredSpeed);


	if (entity->speed < entity->desiredSpeed) {
		entity->speed += (entity->maxAcceleration);
	} else if (entity->speed > entity->desiredSpeed) {
		entity->speed -= (entity->maxAcceleration);
	}

	Ogre::Quaternion desiredRot(Ogre::Radian(entity->desiredHeading), Ogre::Vector3::UNIT_Y);

	//update it all
	entity->rot = Ogre::Quaternion::Slerp(entity->maxRotationalSpeed, entity->rot, desiredRot, true);
	entity->heading = entity->rot.getYaw().valueRadians();
	entity->yaw     = entity->heading;

	entity->speed = clamp(entity->speed, entity->minSpeed, entity->maxSpeed);

	entity->vel = Ogre::Vector3(cos(-entity->heading) * entity->speed, 0.0f, sin(-entity->heading) * entity->speed);
	//finally update pos
	entity->pos = entity->pos + (entity->vel);
	entity->pos.x = clamp (entity->pos.x, 0, 10000);
	entity->pos.z = clamp (entity->pos.z, 0, 10000);

	DEBUG(std::cout << "Speed: " << entity->speed << ", Heading: " << entity->heading << std::endl <<std::endl;)

}


void FastEcslent::Physics2D2::tick(){
	doHelmsman();
}
