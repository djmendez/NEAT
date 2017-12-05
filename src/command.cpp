/*
 * command.cpp
 *
 *  Created on: Jan 9, 2012
 *      Author: sushil
 */

#include <const.h>
#include <ent.h>
#include <group.h>
#include <target.h>
#include <engine.h>
#include <OgreVector3.h>
#include "DEBUG.h"
#include "buildings.h"
#include "command.h"
#include "distanceMgr.h"
#include <neat.h>

//#include <command.h>
inline bool FastEcslent::Move::done() {
	//return (entity->pos.squaredDistance((target->location)) <= entity->turningRadius);
	float dist = entity->pos.distance(target->location);
	if (dist <= entity->length){
		this->entity->desiredSpeed = 0;
		return true;
//	else if(this->repulsor.length() > 400 && dist <= entity->turningRadius/2){
//		this->entity->desiredSpeed = 0;
//		return true;
//	}else if(this->repulsor.length() > 600 && dist <= entity->turningRadius/2){
//		this->entity->desiredSpeed = 0;
//		return true;
	}else {
		return false;
	}
}

float FastEcslent::Move::isCollide(Entity *ent){
	int r1 = 60;
	int r2 = 60;
	float dist = ent->pos.distance(entity->pos);
	return (r1+r2 - dist)/(r1+r2);
}

Ogre::Vector3 FastEcslent::Move::computeRepulsor(){
	int nEnts = this->entity->engine->entityMgr->nEnts;
	this->repulsor = 0;
	for (int i=0;i<nEnts;i++){
		Entity *ent = this->entity->engine->entityMgr->ents[i];
		if(ent == this->entity) continue;
		float collide = this->isCollide(ent);
		if(collide > 0){
			Ogre::Vector3 dr= this->entity->pos - ent->pos;
			dr.normalise();
			Ogre::Vector3 dt = this->target->location - this->entity->pos;
			dr *= collide * dt.length();
			this->repulsor += dr;
		}
	}
}

void FastEcslent::Move::init(){
	Ogre::Vector3 diff = target->location - entity->pos;
	entity->desiredHeading = -atan2(diff.z, diff.x);
	entity->desiredSpeed = entity->maxSpeed;

	this->repulsor = 0;

	return;
}

inline void FastEcslent::Move::tick() {
	if(!done()) {
		this->computeRepulsor();

		relativePos = target->location - entity->pos;
		entity->desiredHeading = -atan2(relativePos.z, relativePos.x);
		entity->desiredSpeed = entity->maxSpeed;

		if(this->repulsor.length() > 0){
			Ogre::Vector3 force = this->repulsor + this->relativePos;
			Ogre::Quaternion angle = force.getRotationTo(relativePos);
			float angleT = - atan2(relativePos.z, relativePos.x);
			float angleF = - atan2(force.z, force.x);
			float angleX = - atan2(angle.y, angle.w);
			if(angleX < 1.3 and angleX > -1.3){
				entity->desiredHeading = angleF;
			}else{
				entity->desiredHeading = angleT;
			}

			float maxForce = 1000;
			float forceScale = force.length();
			if (forceScale < 700) forceScale = 700;
			if (forceScale > 1000) forceScale = 1000;
			float forcerange = forceScale /maxForce;
			entity->desiredSpeed = entity->maxSpeed * forcerange;
		}
	} else {
		entity->desiredSpeed = 0.0f;
		this->repulsor = 0;
	}
}

inline bool FastEcslent::Move3D::done() {
	//return (entity->pos.squaredDistance((target->location)) <= entity->turningRadius);
	float dist = entity->pos.distance(target->location);
	if (dist <= entity->length){
		this->entity->desiredSpeed = 0;
		return true;
//	else if(this->repulsor.length() > 400 && dist <= entity->turningRadius/2){
//		this->entity->desiredSpeed = 0;
//		return true;
//	}else if(this->repulsor.length() > 600 && dist <= entity->turningRadius/2){
//		this->entity->desiredSpeed = 0;
//		return true;
	}else {
		return false;
	}
}

float FastEcslent::Move3D::isCollide(Entity *ent){
	int r1 = 60;
	int r2 = 60;
	float dist = ent->pos.distance(entity->pos);
	return (r1+r2 - dist)/(r1+r2);
}

Ogre::Vector3 FastEcslent::Move3D::computeRepulsor(){
	int nEnts = this->entity->engine->entityMgr->nEnts;
	this->repulsor = 0;
	for (int i=0;i<nEnts;i++){
		Entity *ent = this->entity->engine->entityMgr->ents[i];
		if(ent == this->entity) continue;
		float collide = this->isCollide(ent);
		if(collide > 0){
			Ogre::Vector3 dr= this->entity->pos - ent->pos;
			dr.normalise();
			Ogre::Vector3 dt = this->target->location - this->entity->pos;
			dr *= collide * dt.length();
			this->repulsor += dr;
		}
	}
}

void FastEcslent::Move3D::init(){
	Ogre::Vector3 diff = target->location - entity->pos;
	entity->desiredHeading = -atan2(diff.z, diff.x);
	entity->desiredSpeed = entity->maxSpeed;

	this->repulsor = 0;

	return;
}

//TODO ask Siming about this Y computation
inline void FastEcslent::Move3D::tick() {
	if(!done()) {
		this->computeRepulsor();

		relativePos = target->location - entity->pos;
		entity->desiredHeading = -atan2(relativePos.z, relativePos.x);
		entity->desiredSpeed = entity->maxSpeed;

		if(target->location.y < entity->pos.y)
			entity->potentialVec.y = -1;
		else
			entity->potentialVec.y = 1;

		if(this->repulsor.length() > 0){
			Ogre::Vector3 force = this->repulsor + this->relativePos;
			Ogre::Quaternion angle = force.getRotationTo(relativePos);
			float angleT = - atan2(relativePos.z, relativePos.x);
			float angleF = - atan2(force.z, force.x);
			float angleX = - atan2(angle.y, angle.w);
			if(angleX < 1.3 and angleX > -1.3){
				entity->desiredHeading = angleF;
			}else{
				entity->desiredHeading = angleT;
			}

			float maxForce = 1000;
			float forceScale = force.length();
			if (forceScale < 700) forceScale = 700;
			if (forceScale > 1000) forceScale = 1000;
			float forcerange = forceScale /maxForce;
			entity->desiredSpeed = entity->maxSpeed * forcerange;
		}
	} else {
		entity->desiredSpeed = 0.0f;
		this->repulsor = 0;
	}
}

//Build for SCV building structures
//----------------------------------------------------------------------------
//inline bool FastEcslent::BuildStructure::done() {
//	return (entity->pos.squaredDistance((target->location)) <= entity->turningRadius);
//}
//
//void FastEcslent::BuildStructure::init(){
//	Ogre::Vector3 diff = target->location - entity->pos;
//	entity->desiredHeading = -atan2(diff.z, diff.x);
//	entity->desiredSpeed = entity->maxSpeed;
//	return;
//}
//
//inline void FastEcslent::BuildStructure::tick() {
//	if(!done()) {
//		relativePos = target->location - entity->pos;
//		entity->desiredHeading = -atan2(relativePos.z, relativePos.x);
//		entity->desiredSpeed = entity->maxSpeed;
//	} else {
//		entity->desiredSpeed = 0.0f;
//	}
//	return;
//}
//Build for structures
//------------------------------------------------------------------------------------------
inline void FastEcslent::BuildCommand::init()
{
    if(entity != NULL && entity->getAspect(BUILDER) != NULL)
    {
        //Building* structure = dynamic_cast<Building*>(entity);
        //structure->enqueue(type);
        //entity->getAspect(BUILDER);
        Builder* builder = dynamic_cast<Builder *> (entity->getAspect(BUILDER));
        //if(builder != NULL)
        //{
        builder->enqueue(entType);
        //}
    }
    isDone = true;
	

}
inline bool FastEcslent::BuildCommand::done(){
	return isDone;
}

inline void FastEcslent::BuildCommand::tick(){
//    if(!isDone)
//        this->init();
}
//Gather for SCV
//------------------------------------------------------------------------------------------
inline void FastEcslent::Wait::init(){
	timeLeftToWait = target->waitTime;

}
inline bool FastEcslent::Wait::done(){
	return timeLeftToWait < 0.0;
}

inline void FastEcslent::Wait::tick(){
	if(timeLeftToWait > 0) {
		timeLeftToWait -= 1;
	} else {
		DEBUG(std::cout << entity->uiname  << ": Done waiting" << std::endl;)
	}
}
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
inline void FastEcslent::GasFieldCommand::init(){

}
inline bool FastEcslent::GasFieldCommand::done(){
    return false;
}

inline void FastEcslent::GasFieldCommand::tick(){
    if(((FastEcslent::Gas*)this->entity)->refinary != NULL && ((FastEcslent::Gas*)this->entity)->refinary->entityState != FastEcslent::DEAD)
    {
        this->entity->selectable = false;
        this->entity->isSelected = false;
        ((FastEcslent::Gas*)this->entity)->refinary= NULL;
    }
    else
    {
        this->entity->selectable = true;
    }
}
//------------------------------------------------------------------------------------------


// Potential fields move for groups

inline bool FastEcslent::PotentialMove::done(){
	//preturn false;
	return (entity->pos.squaredDistance(target->location) <= (entity->turningRadius* entity->turningRadius)*10);// && (int) ((entity->attractivePotential)) > -15);
}

void FastEcslent::PotentialMove::init(){
	Ogre::Vector3 diff = target->location - entity->pos;
	entity->desiredSpeed = entity->maxSpeed;
}

inline void FastEcslent::PotentialMove::tick(){
	int nEnts = entity->engine->entityMgr->nEnts;

	if (!done()){
		// compute force
			double repulsivePotential = 0.0f;
			entity->potentialVec = Ogre::Vector3::ZERO;
			Ogre::Vector3 tmp;
			int nInRange = 1; // at least one so that you don't multiply by 0 later
			for (int i = 0; i < nEnts; i++){
				if(i != entity->entityId.id){// repulsed by all other entities
					if (entity->engine->distanceMgr->distance[entity->entityId.id][i] < 0.1f/*RepulsionThresholdDistance*/) { // Don't care about entities too far away
						nInRange += 1;
						tmp = (entity->engine->distanceMgr->normalizedDistanceVec[i][entity->entityId.id]);
						repulsivePotential =  (B * entity->engine->entityMgr->ents[i]->mass) / pow(entity->engine->distanceMgr->distance[entity->entityId.id][i], m);
						if(repulsivePotential  > INT_MAX){   //repulsive potential could be infinite
							repulsivePotential = INT_MAX;
						}
						entity->potentialVec += (tmp * repulsivePotential);
					}
				}
			}
			//attracted by target
			tmp = (entity->pos - target->location);
			//tmp = target->location - entity->pos;
			double targetDistance = tmp.length();
			entity->attractivePotential =  -(A ) / pow(targetDistance, n);// + (B) /pow (targetDistance, m);
			entity->potentialVec += (tmp.normalisedCopy() * entity->attractivePotential * nInRange); // nInRange needs to be at least 1
			//applyPotential(entity, potentialVec);

			entity->desiredHeading = atan2(-entity->potentialVec.z, entity->potentialVec.x);

			double cosDiffFrac = (1.0 - cos(entity->vel.angleBetween(entity->potentialVec).valueRadians()))/2.0;// between 0 and 2 divided by 2.0 gives something between 0 and 1
			entity->desiredSpeed   = (entity->maxSpeed - entity->minSpeed) * (1.0 - cosDiffFrac);



		// apply force
	} else {
		DEBUG(std::cout << "Attractive Potential: " << entity->attractivePotential << std::endl;)
		entity->desiredSpeed = 0.0f;
		entity->desiredHeading = entity->heading;
	}
}

//3D Potential fields for flying entities
inline bool FastEcslent::Potential3DMove::done() {
	if(this->entity->entityType == SC_ZEALOT){
		return false;
	}

	double dist = entity->pos.squaredDistance(target->location);
	double trad = (entity->turningRadius * entity->turningRadius);

	return (dist <= trad);// && (int) ((entity->attractivePotential)) > -15);*/
	//return false;
}

void FastEcslent::Potential3DMove::init(){
	//Ogre::Vector3 diff = target->location - entity->pos;
	//entity->desiredSpeed = entity->maxSpeed;

	if(entity->entityId.side == Side::BLUE) {
		this->A = entity->engine->infoMgr->squadmgr_blue->getPotentialA();
		this->B = entity->engine->infoMgr->squadmgr_blue->getPotentialB();
		this->m = entity->engine->infoMgr->squadmgr_blue->getPotentialM();
		this->n = entity->engine->infoMgr->squadmgr_blue->getPotentialN();
//		this->A = 1000.0;
//		this->B = 1000.0;
//		this->m = 2.0;
//		this->n = 1;
	} else {
		this->A = entity->engine->infoMgr->squadmgr_red->getPotentialA();
		this->B = entity->engine->infoMgr->squadmgr_red->getPotentialB();
		this->m = entity->engine->infoMgr->squadmgr_red->getPotentialM();
		this->n = entity->engine->infoMgr->squadmgr_red->getPotentialN();
//		this->A = 1000.0;
//		this->B = 1000.0;
//		this->m = 2.0;
//		this->n = 1;
		//if (entity->engine->infoMgr->squadmgr_red->getPotentialM() > 3.00)
			//this->m = entity->engine->infoMgr->squadmgr_red->getPotentialM();
//		else
//			this->m = 3.00;
	}
	//std::cout << this->entity->entityId.side << ": " << this->A << ", " << this->B << ", " << this->m << ", " << this->n << std::endl;
}


// INSERT NEAT CODE BELOW
// Logic:
// If AI side, if AI on, do whats is method already
//				else if AIoff do nothing (set speed 0, heading 0)
// Else if NEAT side, then gather info re location, pass on to Neat and use output to set speed and heading

inline void FastEcslent::Potential3DMove::tick() {
	int nEnts = entity->engine->entityMgr->nEnts;

	// just shortcut
	NEAT *NEATNet = entity->engine->engineNEATNet;

	float relevantDistanceThreshold;// = 100.0f;
	if (this->entity->entityType == SC_ZEALOT){
		relevantDistanceThreshold = 50.0f;
	}
	else
		relevantDistanceThreshold = 100.0f;

	if (!done()){
		// If side is BLUE /ENEMY, either use AI or DO NOTHING if AI disconnected
		if (entity->entityId.side == BLUE) {

			// compute force
			double repulsivePotential = 0.0f;
			entity->potentialVec = Ogre::Vector3::ZERO;
			Ogre::Vector3 tmp;
			int nInRange = 1; // at least one so that you don't multiply by 0 later

			//Capture information for all other entities: repulsivePotential
			for (int i = 0; i < nEnts; i++){
				if(i != entity->entityId.id){// repulsed by all other entities
					if (entity->engine->distanceMgr->distance[entity->entityId.id][i] < relevantDistanceThreshold) { // Don't care about entities too far away
						if(entity->entityId.side == entity->engine->entityMgr->ents[i]->entityId.side) {
							nInRange += 1;
							tmp = (entity->engine->distanceMgr->normalizedDistanceVec[i][entity->entityId.id]);

							double val = entity->engine->distanceMgr->distance[entity->entityId.id][i];
							if(val == 0)
								val = 0.1;

							repulsivePotential =  (B * entity->engine->entityMgr->ents[i]->mass) / pow(val, m);
							entity->potentialVec += (tmp * repulsivePotential);
						}
					}
				}
			}
			//attracted by target


			tmp = (entity->pos - target->location);
			//tmp = target->location - entity->pos;
			double targetDistance = tmp.length();
			entity->attractivePotential =  -(A ) / pow(targetDistance, n);// + (B) /pow (targetDistance, m);

			entity->potentialVec += (tmp.normalisedCopy() * entity->attractivePotential * nInRange); // nInRange needs to be at least 1

			//applyPotential(entity, potentialVec);

			entity->desiredHeading = atan2(-entity->potentialVec.z, entity->potentialVec.x);
			double cosDiffFrac = (1.0 - cos(entity->vel.angleBetween(entity->potentialVec).valueRadians()))/2.0;// between 0 and 2 divided by 2.0 gives something between 0 and 1
			entity->desiredSpeed   = (entity->maxSpeed - entity->minSpeed) * (1.0 - cosDiffFrac);
			if (!entity->engine->options.enemyTacticalAI) { // if enemyTacticalAI is DISABLED, move VERY slowly
				entity->desiredHeading = 0.;
				entity->desiredSpeed = 0.00000000000000000000000000001;
			}
		}
		// IF SIDE IS RED (FRIENDLY) USE NEAT
		else if (entity->entityId.side == RED) {
			double repulsivePotential = 0.0f;
			float angleRad = 0;
			int segment = 0;

			entity->potentialVec = Ogre::Vector3::ZERO;
			Ogre::Vector3 tmp;
			int nInRange = 1; // at least one so that you don't multiply by 0 later
			Entity *currEnt;

			// initialize all NEAT inputs to 0
			for (int i = 0; i < NEATSegments*4; i++)
				NEATNet->input[i] = 0;

			//Capture information for all other entities: repulsivePotential
			for (int i = 0; i < nEnts; i++){
				if(i != entity->entityId.id){// repulsed by all other entities
					currEnt = entity->engine->entityMgr->ents[i];

					//here figure out relative angle
					// USE atan2(-entity->potentialVec.z, entity->potentialVec.x) instead????
					angleRad = atan2(entity->pos.z - currEnt->pos.z,entity->pos.x - currEnt->pos.x);

					//classify into segment and side
					angleRad += M_PI; // add pi to make in range 0 - 2pi
					segment = (int) angleRad / ((2 * M_PI) / NEATSegments); // divide 2pi interval by segment size to find proper segment
					if(segment == NEATSegments) // boundary condition -- if angle is EXACTLY pi, would cause improper segment
						segment--;

					segment *= 2; // since segments come in pairs (units, average distance) multiply by 2 to find starting point


					if (currEnt->entityId.side == RED)
						segment += (2 * NEATSegments); // Enemies (BLUE) occupy first half of array, FRiendlies (RED) second half

					NEATNet->input[segment] += 1;
					NEATNet->input[segment+1] += entity->engine->distanceMgr->distance[entity->entityId.id][i];

					//add distance to overall distance (average later)

					if (entity->engine->distanceMgr->distance[entity->entityId.id][i] < relevantDistanceThreshold) { // Don't care about entities too far away
						if(entity->entityId.side == entity->engine->entityMgr->ents[i]->entityId.side) {
							nInRange += 1;
							tmp = (entity->engine->distanceMgr->normalizedDistanceVec[i][entity->entityId.id]);

							double val = entity->engine->distanceMgr->distance[entity->entityId.id][i];
							if(val == 0)
								val = 0.1;

							repulsivePotential =  (B * entity->engine->entityMgr->ents[i]->mass) / pow(val, m);
							entity->potentialVec += (tmp * repulsivePotential);
						}
					}
				}
			}
			// convert all distances to average distance
			for (int i = 0; i < NEATSegments*4; i += 2)
				if (NEATNet->input[i])
					NEATNet->input[i+1] /= NEATNet->input[i];

			//Apply Neural Net
			NEATNet->NEATProcess();

			//CODE BELOW NEEDS TO BE HOOKED UP ONCE WE ARE READY TO USE NEAT

			// ONCE NEAT IS CONNECTED USE THE TWO LINES BELOW
			//entity->desiredHeading = NEATNet->output[0];
			//entity->desiredSpeed = NEATNet->output[1];

			// ELIMINATE BELOW ONCE NEAT IS CONNECTED
			tmp = (entity->pos - target->location);
			//tmp = target->location - entity->pos;
			double targetDistance = tmp.length();
			entity->attractivePotential =  -(A ) / pow(targetDistance, n);// + (B) /pow (targetDistance, m);

			entity->potentialVec += (tmp.normalisedCopy() * entity->attractivePotential * nInRange); // nInRange needs to be at least 1

			//applyPotential(entity, potentialVec);

			entity->desiredHeading = atan2(-entity->potentialVec.z, entity->potentialVec.x);
			double cosDiffFrac = (1.0 - cos(entity->vel.angleBetween(entity->potentialVec).valueRadians()))/2.0;// between 0 and 2 divided by 2.0 gives something between 0 and 1
			entity->desiredSpeed   = (entity->maxSpeed - entity->minSpeed) * (1.0 - cosDiffFrac);
			// ELEMINATE TO HERE ONCE NEAT IS CONECTED
		}
	}

			//std::cout << "Moving " << entity->uiname << " to " << target->location.y << " Y" << std::endl;

		/*double repulsivePotential = 0.0f;
					entity->potentialVec = Ogre::Vector3::ZERO;
					Ogre::Vector3 tmp;
					int nInRange = 1; // at least one so that you don't multiply by 0 later
					for (int i = 0; i < nEnts; i++){
						if(i != entity->entityId.id){// repulsed by all other entities
							//if (entity->engine->distanceMgr->distance[entity->entityId.id][i] < RepulsionThresholdDistance) { // Don't care about entities too far away
							if(entity->pos.distance(entity->engine->entityMgr->ents[i]->pos) < RepulsionThresholdDistance) {
								nInRange += 1;
								//tmp = (entity->engine->distanceMgr->normalizedDistanceVec[i][entity->entityId.id]);
								tmp = (entity->pos - entity->engine->entityMgr->ents[i]->pos).normalisedCopy();

								double val = sqrt(pow(entity->pos.x - entity->engine->entityMgr->ents[i]->pos.x, 2) + pow(entity->pos.y - entity->engine->entityMgr->ents[i]->pos.y, 2) + pow(entity->pos.z - entity->engine->entityMgr->ents[i]->pos.z, 2));

								repulsivePotential =  (B * entity->engine->entityMgr->ents[i]->mass) / pow(val, m);
								if(repulsivePotential  > INT_MAX){   //repulsive potential could be infinite
									repulsivePotential = INT_MAX;
								}
								entity->potentialVec += (tmp * repulsivePotential);
							}
						}
					}
					//attracted by target
					tmp = (entity->pos - target->location);
					//tmp = target->location - entity->pos;
					double targetDistance = tmp.length();
					entity->attractivePotential =  -(A ) / pow(targetDistance, n);// + (B) /pow (targetDistance, m);
					entity->potentialVec += (tmp.normalisedCopy() * entity->attractivePotential * nInRange); // nInRange needs to be at least 1
					//applyPotential(entity, potentialVec);

					entity->desiredHeading = atan2(-entity->potentialVec.z, entity->potentialVec.x);

					double cosDiffFrac = (1.0 - cos(entity->vel.angleBetween(entity->potentialVec).valueRadians()))/2.0;// between 0 and 2 divided by 2.0 gives something between 0 and 1
					entity->desiredSpeed   = (entity->maxSpeed - entity->minSpeed) * (1.0 - cosDiffFrac);*/

		// apply force
	else {
		DEBUG(std::cout << "Attractive Potential: " << entity->attractivePotential << std::endl;)
		entity->desiredSpeed = 0.0f;
		entity->desiredHeading = entity->heading;
	}
}


FastEcslent::Maintain::Maintain (Entity *ent, Target *tgt): UnitCommand(ent, MaintainCommand, tgt){
	if(valid(tgt->entity)) {
		DEBUG(std::cout << "Maintaining with respect to: " << tgt->entity->uiname << std::endl;)
		tgt->location = tgt->entity->pos + tgt->offset;
	}
}
bool FastEcslent::Maintain::valid(Entity *entity) {
	return (entity != NULL);
}

inline bool FastEcslent::Maintain::done() {
	return false;
}

void FastEcslent::Maintain::init(){

	if (kInvalidFloat == target->offsetDistance) { // offset must contain absolute position right now. Convert to offsetRot and offsetDistance
		Ogre::Quaternion q = Ogre::Vector3(cos(-target->entity->heading) * 100.0f, 0.0f, sin(-target->entity->heading) * 100.0f).getRotationTo(target->offset);
		target->offsetYaw = q.getYaw().valueRadians();
		target->offsetDistance = (target->entity->pos - target->offset).length();
		DEBUG(std::cout << "OffsetYaw: " << target->offsetYaw << " offsetDistance: " << target->offsetDistance << std::endl;)
	}
	return;
}

inline void FastEcslent::Maintain::tick() { //offsetRotation and offsetDistance set by init()
	target->offset = (target->entity->rot * Ogre::Quaternion(Ogre::Radian(target->offsetYaw), Ogre::Vector3::UNIT_Y)) * (Ogre::Vector3::UNIT_X * target->offsetDistance);
	target->location = target->entity->pos + target->offset;

	relativePos = target->location - entity->pos;
	relativeVel = target->entity->vel - entity->vel;
	relativeSpeed = relativeVel.length();
	predictedTimeToClose = relativePos.length()/relativeSpeed;
	predictedPos =  target->location + (target->entity->vel * predictedTimeToClose);
	interceptPos = predictedPos - entity->pos;

	entity->desiredHeading = -atan2(interceptPos.z, interceptPos.x);
	if ((relativeSpeed * predictedTimeToClose) > 30)
		entity->desiredSpeed = entity->maxSpeed;
	else
		entity->desiredSpeed = target->entity->speed;

	return;
}


void FastEcslent::Tactic::changeLeadership(LeadershipType selector){

	Ogre::Vector3 tpos;
	if(this->target->target.location == InvalidLocation){
		tpos = this->target->target.entity->pos;
	} else {
		tpos = this->target->target.location;
	}
	this->group->leaderIndex = 0;
	if (this->group->nEntitiesInGroup > 0) {
		switch(selector){
		case 0:
			this->group->leaderIndex = closestToTarget(true, tpos);
			break;
		case 2:
			this->group->leaderIndex = closestToTarget(false, tpos); //furthest from target
			break;
		case 3:
			this->group->leaderIndex = mostMassive(true);
			break;
		case 4:
			this->group->leaderIndex = mostMassive(true);//smallest mass
			break;
		case 5:
			this->group->leaderIndex = randInt(0, this->group->nEntitiesInGroup);
			break;
		default:
			this->group->leaderIndex = 0;
			break;
		}
	}

}


int FastEcslent::Tactic::mostMassive(bool massest){
	float minMass = 0;
	int minIndex = INT_MAX;
	float maxMass = 0;
	int maxIndex = 0;
	float mass;
	for (int i = 0; i < this->group->nEntitiesInGroup; i++){
		mass = this->group->members[i]->mass;
		if(mass < minMass){
			minMass = mass;
			minIndex = i;
		}
		if (mass > maxMass){
			maxMass = mass;
			maxIndex = i;
		}
	}
	if(!massest) {
		return minIndex;
	} else {
		return maxIndex;
	}

}


int FastEcslent::Tactic::closestToTarget(bool closest, Ogre::Vector3 tpos){
	//Entity** members = this->group->members;
		//int dist = members[leaderIndex]->pos.distance(this->target->target.location);

	float minDist = 0;
	int minIndex = INT_MAX;
	float maxDist = 0;
	int maxIndex = 0;
	float dist;
	for (int i = 0; i < this->group->nEntitiesInGroup; i++){
		dist = this->group->members[i]->pos.distance(tpos);
		if(dist < minDist) {
			minDist = dist;
			minIndex = i;
		}
		if (dist > maxDist){
			maxDist = dist;
			maxIndex = i;
		}
	}
	if(closest) {
		return minIndex;
	} else {
		return maxIndex;
	}
}

//Stand by
//------------------------------------------------------------------------------------------
inline void FastEcslent::Guard::init(){
	distanceMgr = this->entity->engine->distanceMgr;
	this->isGuarding = false;
	this->move = new Move(this->entity, this->target);
}
inline bool FastEcslent::Guard::done(){
	return false;
}
void FastEcslent::Guard::startGuarding(){
	if(!isGuarding){
		target->location = entity->pos;
		target->offset = entity->pos;
		this->entity->isAttacking = true;
		//this->move->init();
	}
	isGuarding = true;
}

void FastEcslent::Guard::stopGuarding(){
	this->entity->desiredSpeed = 0;
	isGuarding = false;
	this->entity->isAttacking = false;
}
inline void FastEcslent::Guard::tick(){
	if(this->entity->entityId.id==0){
		int pause=0;
		int j = pause;
		j = j+1;
	}
	int index = distanceMgr->closestEnemy[this->entity->entityId.id];
	Entity* enemy = this->entity->engine->entityMgr->ents[index];
	float dist_o = this->target->offset.distance(enemy->pos);
	float dist_e = this->entity->pos.distance(enemy->pos);

	//chase - in case the enemy shows in the seekRange
	//chase - in case the distance between unit and enemy is less than 2/3 of the seekRange.
	if(dist_o < this->entity->seekRange || dist_e < this->entity->seekRange){
		this->target->entity = enemy;
		this->target->location = enemy->pos;
		this->move->tick();
	}else if(dist_o > this->entity->turningRadius){
		this->target->location = target->offset;
		this->move->tick();
	}else {
		stopGuarding();
	}
}
//------------------------------------------------------------------------------------------

//Attack Move
int count;
//------------------------------------------------------------------------------------------
inline void FastEcslent::AttackMove::init(){
	this->entity->weapon->setTarget(this->target->entity);
	this->move = new Move(this->entity, this->target);
	this->entity->isAttacking = true;
}

inline void FastEcslent::AttackMove::postProcess(){
	this->entity->isAttacking = false;
}

inline bool FastEcslent::AttackMove::done(){
	//Attack to a position, if the entity arrived the destination, done
	if(this->target->entity==0 && this->target->location.distance(this->entity->pos) < this->entity->turningRadius){
		this->entity->isAttacking = false;
		return true;
	}
	//If the target is an entity, stop when the entity is dead.
	if(this->target->entity !=0 && this->target->entity->entityState != FastEcslent::ALIVE){
		this->entity->isAttacking = false;
		return true;
	}

	return false;
}


inline void FastEcslent::AttackMove::tick(){
	this->entity->isAttacking = true;
	//Target is an entity
	if(this->target->entity != 0 && this->entity->pos.distance(this->target->entity->pos) > this->entity->weapon->weaponType->maxRange()){
		this->move->tick();
	}

	//Target is a location
	if(this->target->entity == 0 &&
			this->entity->engine->distanceMgr->closestEnemyDistance[this->entity->entityId.id] > this->entity->weapon->weaponType->maxRange()){
		this->move->tick();
	}
}

inline void FastEcslent::AttackMove3D::init(){
	this->entity->weapon->setTarget(this->target->entity);
	this->move = new Potential3DMove(this->entity, this->target);
	this->move->init();
	this->entity->isAttacking = true;
}

inline void FastEcslent::AttackMove3D::postProcess(){
	this->entity->isAttacking = false;
}

inline bool FastEcslent::AttackMove3D::done(){
	if(this->target->entity==0 && this->entity->entityType == SC_ZEALOT && this->target->location.distance(this->entity->pos) < 1){
		this->entity->isAttacking = false;
		return true;
	}
	//Attack to a position, if the entity arrived the destination, done
	if(this->target->entity==0 && this->entity->entityType == DRONE && this->target->location.distance(this->entity->pos) < this->entity->turningRadius){
		this->entity->isAttacking = false;
		return true;
	}
	//If the target is an entity, stop when the entity is dead.
	if(this->target->entity !=0 && this->target->entity->entityState != FastEcslent::ALIVE){
		this->entity->isAttacking = false;
		return true;
	}

	return false;
}


inline void FastEcslent::AttackMove3D::tick(){
	this->entity->isAttacking = true;
	//Target is an entity
	if(this->target->entity != 0 && this->entity->pos.distance(this->target->entity->pos) > this->entity->weapon->weaponType->maxRange()){
		this->move->tick();
	}

	//Target is a location
	if(this->target->entity == 0 &&
			this->entity->engine->distanceMgr->closestEnemyDistance[this->entity->entityId.id] > this->entity->weapon->weaponType->maxRange()){
		this->move->tick();
	}
}

//------------------------------------------------------------------------------------------
