/*
 * flock.h
 *
 *  Created on: Jan 24, 2012
 *      Author: sushil
 */

#ifndef FLOCK_H_
#define FLOCK_H_

#include <OgreVector3.h>

#include <distanceMgr.h>
#include <entityMgr.h>
#include <utils.h>
#include <const.h>

#include<command.h>
#include<group.h>


namespace FastEcslent {

	class Flock: public Tactic {
	private:
		bool valid (Group *grp){return true;};
		Ogre::Vector3 avgPos;
		Ogre::Vector3 avgVel;
		Ogre::Vector3 force;
		Ogre::Vector3 cohesion(int i);
		float CohesionParameter;
		Ogre::Vector3 alignment(int i);
		float AlignmentParameter;
		void computeSeparation( int leaderIndex);
		float SeparationParameter;
		float SeparationThreshold;
		void applyForces(int leaderIndex);
		float maxSpeedByAngle[MaxEnts];

		void bringWithinNeighborhood();
		Entity *nonNeighbors[MaxEnts];
		int     nNonNeighbors;


		Ogre::Vector3 forces[MaxEnts];
		Entity **members;
		int    nMembers;
		int    nEnts;
		DistanceMgr* distanceMgr;
		EntityMgr*   entityMgr;

	public:
		Flock (Group *grp, GroupTarget *tgt);
		bool done();
		void init();
		void tick();
		void postProcess(){};
                virtual Command* clone() {};

		int closestToTarget(bool closest);       // closest (closest==true) or furthest (closest = false)
		int mostMassive(bool massivest); // most (massivest == true) and least massive (massivest == false)

	};

}



#endif /* FLOCK_H_ */
