/*
 * physics.cpp
 *
 *  Created on: Dec 19, 2011
 *      Author: sushil
 */
#include <assert.h>

#include <OgreVector3.h>

#include <physics.h>
#include <ent.h>
#include <utils.h>
#include "DEBUG.h"

void FastEcslent::Physics2D1::init(){
	entity->desiredSpeed = entity->speed;
	entity->desiredHeading = entity->heading;
}

void FastEcslent::Physics2D1::doHelmsman(){

	if (kInvalidFloat == entity->desiredSpeed || kInvalidFloat == entity->desiredHeading)
		return;
	//speed
	timeScaledAcceleration = entity->maxAcceleration ;
	entity->speed += clamp(entity->desiredSpeed - entity->speed, -timeScaledAcceleration, +timeScaledAcceleration);
	cosYaw = cos(-entity->heading);
	sinYaw = sin(-entity->heading);
	entity->vel = Ogre::Vector3(entity->speed * cosYaw, 0.0f, entity->speed * sinYaw);
	entity->pos = entity->pos + (entity->vel);

	//heading
	this->timeScaledRotationalSpeed = entity->maxRotationalSpeed;
	//rotate only if you are moving at > 0.5
	if(entity->speed > 0.5f){
		angleDiff = differenceBetweenAngles(entity->desiredHeading, entity->heading);
		dHeading = clamp(angleDiff, -timeScaledRotationalSpeed, +timeScaledRotationalSpeed);
		entity->heading += dHeading;
	}

	DEBUG(std::cout << "Speed: " << entity->speed << ", Heading: " << entity->heading << std::endl;)
}


void FastEcslent::Physics2D1::tick(){
	doHelmsman();
}
