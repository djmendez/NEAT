/*
 * const.h
 *
 *  Created on: Nov 21, 2011
 *      Author: sushil
 */

#ifndef CONST_H_
#define CONST_H_

#include<OgreVector3.h>

namespace FastEcslent {
//control parameters
const unsigned int angleVotingFrequency = 15;
const unsigned int numAngleVotes = 360;

//constants
const unsigned int kInvalidHandle = ~0u;
const unsigned int kMaxAABBs = 1024;
const float kInvalidFloat = (float)(~0u);

const Ogre::Vector3 InvalidLocation(0, -1, 0);

typedef unsigned int CEntHandle;
const int kMaxDebugLines = 1024;


//Entity properties

const int MaxEnts    = 1024;
const int MaxGroups  = 1024;
const int MaxAspects = 10;
const int MaxCommands = 256;

const int NCircleSegments = 16;
const int GuiSpeedScaler  = 1.0;

const int healthLineLength = 100;

const float EPSILON = 0.0001;

const int MaxNeededTypes = 5;
const int MaxEntitiesInQueue = 5;
const int MaxEntityTypesPerBuilding = 5;


//Game Specific properties
const int   PerPlayerMaxEnts          = 200;
const float MaxMineralFindingDistance = 500;
const int   MaxPlayers                = 4;

// segments for the NEAT input
const int NEATSegments = 8;


}


#endif /* CONST_H_ */
