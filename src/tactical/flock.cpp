/*
 * flock.cpp
 *
 *  Created on: Feb 4, 2012
 *      Author: sushil
 */

#include<climits>

#include <OgreVector3.h>

#include <engine.h>

#include <flock.h>
#include <ent.h>
#include <command.h>
#include <unitAI.h>
#include "DEBUG.h"

//#include <params.h>

FastEcslent::Flock::Flock(Group *grp, GroupTarget* trgt): Tactic(grp, FLOCK, trgt){
	//group = grp;       done in Tactic class
	//target = trgt;

}

inline bool FastEcslent::Flock::done() {
	return false;
}

void FastEcslent::Flock::init(){
	DEBUG(std::cout << "Initializing Flocking " << std::endl;)
	/*
	CohesionParameter = this->group->engine->flockParameters.cohesionParameter;
	AlignmentParameter = this->group->engine->flockParameters.alignmentParameter;
	SeparationParameter = this->group->engine->flockParameters.separationParameter;
	SeparationThreshold = this->group->engine->flockParameters.separationThreshold;

	*/
	CohesionParameter = 1.0;//this->group->engine->flockParameters.cohesionParameter;
	AlignmentParameter = 1.0;//this->group->engine->flockParameters.alignmentParameter;
	SeparationParameter = 1.0;//this->group->engine->flockParameters.separationParameter;
	SeparationThreshold = 2000;//this->group->engine->flockParameters.separationThreshold;


	members = group->members;
	nMembers = group->nEntitiesInGroup;
	distanceMgr = this->group->engine->distanceMgr;
	entityMgr   = this->group->engine->entityMgr;
	nEnts       = entityMgr->nEnts;

	for (int i = 0; i < nMembers; i++){
		maxSpeedByAngle[i] = (members[i]->maxSpeed - members[i]->minSpeed)/1.57; //pi
		//members[i]->aspects.clear();
	}

	assert(group->leaderIndex >=0 && group->leaderIndex < group->nEntitiesInGroup);

	DEBUG(std::cout << "Leader: " << group->members[group->leaderIndex]->uiname << " Number in group: " << group->nEntitiesInGroup << std::endl;)

}


inline Ogre::Vector3 FastEcslent::Flock::cohesion(int i) {
	//steer towards avgPos
	Ogre::Vector3 force = avgPos - members[i]->pos;
	force.normalise();

	//steering force proportional to angleInRadians
	Ogre::Radian angleInRadians = avgPos.angleBetween(members[i]->pos);
	return force * angleInRadians.valueRadians() * CohesionParameter / Ogre::Math::PI;
}

inline Ogre::Vector3 FastEcslent::Flock::alignment(int i) {
	//Steer in the average direction
	Ogre::Vector3 force = avgVel - members[i]->vel;
	force.normalise();

	//force proportional to difference in direction
	Ogre::Radian angleInRadians = avgVel.angleBetween(members[i]->vel);
	return force * angleInRadians.valueRadians() * AlignmentParameter / Ogre::Math::PI;
}

inline void FastEcslent::Flock::computeSeparation(int leaderIndex) {
	float dist;
	int iId, jId;
	float param = 0;
	int   largePrime = 499;//7411;
	float minMass = 500;
	//for(int i = 0; i < nMembers; i++){
	for(int i = 0; i < nEnts; i++){//everyone is to be avoided
		//if(i == leaderIndex) continue;
		// if distance i, j < threshold push away
		iId = i;//members[i]->entityId.id;

		for(int j = 0; j < nMembers; j++){
			if (i == j) continue;
			jId = members[j]->entityId.id;
			if (distanceMgr->distance[iId][jId] < SeparationThreshold){
				dist = distanceMgr->distance[iId][jId];

				param = minMass + (int) (entityMgr->ents[iId]->mass*entityMgr->ents[iId]->speed) % largePrime ;
				//forces[j] = forces[j] + (distanceMgr->normalizedDistanceVec[iId][jId] * ((((int) entityMgr->ents[i]->mass)%(int) SeparationParameter)/(0.0001f+(dist * dist)))) ;
				//forces[j] = forces[j] + (distanceMgr->normalizedDistanceVec[iId][jId] * ((((int) members[i]->mass)%(int) SeparationParameter)/(0.0001f+(dist * dist)))) ;
				//forces[j] = forces[j] + (distanceMgr->normalizedDistanceVec[iId][jId] * (members[i]->mass * SeparationParameter/(0.000001+(dist * dist)))) ;
				forces[j] = forces[j] + (distanceMgr->normalizedDistanceVec[iId][jId] * (param * SeparationParameter/(0.000001+  (dist*dist)))) ;
				//forces[j] = forces[j] + (distanceMgr->normalizedDistanceVec[iId][jId] * (SeparationParameter/(0.000001+(dist * dist)))) ;
			}
		}
	}
	return;
}

inline void FastEcslent::Flock::applyForces(int leaderIndex) {

	for (int i = 0; i < nMembers; i++){
		if (i == leaderIndex) continue;
		members[i]->desiredHeading = -atan2(forces[i].z, forces[i].x);
		//members[i]->desiredSpeed = members[i]->minSpeed + ( maxSpeedByAngle[i]/ (0.0001f + fabs(members[i]->vel.angleBetween(forces[i]).valueRadians())));
		members[i]->desiredSpeed = members[i]->minSpeed + ( (1.57 - (members[i]->vel.angleBetween(forces[i]).valueRadians())) * maxSpeedByAngle[i]);
		//members[i]->desiredSpeed = members[i]->maxSpeed;
	}

}

inline void FastEcslent::Flock::tick() {
	//update leader - later
	//update flock
	avgPos.x = 0.0; avgPos.y = 0.0; avgPos.z = 0.0;
	avgVel.x = 0.0; avgVel.y = 0.0; avgVel.z = 0.0;
	int leaderIndex = this->group->leaderIndex;
	float weight = 0.0f;
	float mass = 1.0f;
	//Average position and average velocity
	for (int i = 0; i < nMembers; i++) {
		//if (i == this->group->leaderIndex) continue;
		//mass = ((int) members[i]->mass);
		avgPos += (members[i]->pos * mass);
		avgVel += (members[i]->vel* mass);
		weight += mass;
		forces[i].x = 0.0; forces[i].y = 0.0; forces[i].z = 0.0;
	}
	avgPos = members[leaderIndex]->pos;// avgPos/weight;
	avgVel = members[leaderIndex]->vel;//avgVel/weight;
	//alignment and cohesion
	for (int i = 0; i < nMembers; i++) {
		if(i != leaderIndex){
			forces[i] = forces[i] + alignment(i);
			forces[i] = forces[i] + cohesion(i);
		}
	}
	//spread
	computeSeparation(leaderIndex);
	//convert to dh, ds and apply
	applyForces(leaderIndex);
	return;
}
