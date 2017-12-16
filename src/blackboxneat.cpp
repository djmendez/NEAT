/*
 * neat.cpp
 *
 *  Created on: Dec 2, 2017
 *      Author: daniel
 */


/*
 * neat.c
 *
 *  Created on: Dec 2, 2017
 *      Author: daniel
 */


#include <blackboxneat.h>
#include <ent.h>
#include <engine.h>

bool FastEcslent::BlackBoxNEAT::loadFromFile(const char *filename){
	bool good = importformfile(filename,net);
	if (!good) {
		std::cout << "error loading neat network" <<std::endl;
		return false;
	}
	return true;
}


void FastEcslent::BlackBoxNEAT::NEATProcess() {
	//net->flush();
	if(!activate(net, input, output)){
		std::cout << "error calculating neat network" <<std::endl;
	}
}

void FastEcslent::BlackBoxNEAT::applyNEATProcessToGetSpeedAndHeading(Entity *entity) {

	int nEnts = entity->engine->entityMgr->nEnts;
	BlackBoxNEAT *NEATNet = entity->engine->engineNEATNet;

	double repulsivePotential = 0.0f;
	float angleRad = 0;
	int segment = 0;
	float totFriendlyDist = 0.0f, totEnemyDist = 0.0f; // used to keep track of total distance for NN SCALING

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

			// keep track of the total distance of all friendly and enemy units for use in scaling later
			if (currEnt->entityId.side == RED)
				totFriendlyDist += entity->engine->distanceMgr->distance[entity->entityId.id][i];
			else
				totEnemyDist += entity->engine->distanceMgr->distance[entity->entityId.id][i];
		}
	}

	// SCALE both distances and numbers of units to represent PERCENT of TOTAL per SIDE
	// i.e. For each side, the sum of all unit counts and distances will be 1.0.
	// compute Enemies side first
	for (int i = 0; i < NEATSegments*2; i += 2) {
		if (NEATNet->input[i]) { // ignore segment if there is nothing
			NEATNet->input[i] = NEATNet->input[i] / entity->engine->options.numUnitsB;
			NEATNet->input[i+1] = NEATNet->input[i+1] / totEnemyDist;
		}
	}

	// now compute Friendly Side
	for (int i = NEATSegments*2; i < NEATSegments*4; i += 2) {
		if (NEATNet->input[i]) { // ignore segment if there is nothing
			NEATNet->input[i] = NEATNet->input[i] / (entity->engine->options.numUnitsA - 1); // -1 since *this* Unit is not part of computation
			NEATNet->input[i+1] = NEATNet->input[i+1] / totFriendlyDist;
		}
	}

	//Apply Neural Net
	NEATProcess();

	//Now Process output
	entity->desiredSpeed = (NEATNet->output[0] * (entity->maxSpeed - entity->minSpeed)) + entity->minSpeed;
	entity->desiredHeading = (NEATNet->output[1] - .5) * M_PI;
}

bool FastEcslent::BlackBoxNEAT::isCloseEnough(float distance) {
	return (distance < NEATMinDistance);
}

//double repulsivePotential = 0.0f;
//float angleRad = 0;
//int segment = 0;
//float totFriendlyDist = 0.0f, totEnemyDist = 0.0f; // used to keep track of total distance for NN SCALING
//
//entity->potentialVec = Ogre::Vector3::ZERO;
//Ogre::Vector3 tmp;
//int nInRange = 1; // at least one so that you don't multiply by 0 later
//Entity *currEnt;
//
//// initialize all NEAT inputs to 0
//for (int i = 0; i < NEATSegments*4; i++)
//	NEATNet->input[i] = 0;
//
////Capture information for all other entities: repulsivePotential
//for (int i = 0; i < nEnts; i++){
//	if(i != entity->entityId.id){// repulsed by all other entities
//		currEnt = entity->engine->entityMgr->ents[i];
//
//		//here figure out relative angle
//		// USE atan2(-entity->potentialVec.z, entity->potentialVec.x) instead????
//		angleRad = atan2(entity->pos.z - currEnt->pos.z,entity->pos.x - currEnt->pos.x);
//
//		//classify into segment and side
//		angleRad += M_PI; // add pi to make in range 0 - 2pi
//		segment = (int) angleRad / ((2 * M_PI) / NEATSegments); // divide 2pi interval by segment size to find proper segment
//		if(segment == NEATSegments) // boundary condition -- if angle is EXACTLY pi, would cause improper segment
//			segment--;
//
//		segment *= 2; // since segments come in pairs (units, average distance) multiply by 2 to find starting point
//
//		if (currEnt->entityId.side == RED)
//			segment += (2 * NEATSegments); // Enemies (BLUE) occupy first half of array, FRiendlies (RED) second half
//
//		NEATNet->input[segment] += 1;
//		NEATNet->input[segment+1] += entity->engine->distanceMgr->distance[entity->entityId.id][i];
//
//		// keep track of the total distance of all friendly and enemy units for use in scaling later
//		if (currEnt->entityId.side == RED)
//			totFriendlyDist += entity->engine->distanceMgr->distance[entity->entityId.id][i];
//		else
//			totEnemyDist += entity->engine->distanceMgr->distance[entity->entityId.id][i];
//
//		//add distance to overall distance (average later)
//
//
////					if(entity->entityId.side == entity->engine->entityMgr->ents[i]->entityId.side) {
////						nInRange += 1;
////						tmp = (entity->engine->distanceMgr->normalizedDistanceVec[i][entity->entityId.id]);
////
////						double val = entity->engine->distanceMgr->distance[entity->entityId.id][i];
////						if(val == 0)
////							val = 0.1;
////
////						repulsivePotential =  (B * entity->engine->entityMgr->ents[i]->mass) / pow(val, m);
////						entity->potentialVec += (tmp * repulsivePotential);
////					}
//
//	}
//}
//// SCALE both distances and numbers of units to represent PERCENT of TOTAL per SIDE
//// i.e. For each side, the sum of all unit counts and distances will be 1.0.
//// compute Enemies side first
//for (int i = 0; i < NEATSegments*2; i += 2) {
//	if (NEATNet->input[i]) { // ignore segment if there is nothing
//		NEATNet->input[i] = NEATNet->input[i] / entity->engine->options.numUnitsB;
//		NEATNet->input[i+1] = NEATNet->input[i+1] / totEnemyDist;
//	}
//}
//
//// now compute Friendly Side
//for (int i = NEATSegments*2; i < NEATSegments*4; i += 2) {
//	if (NEATNet->input[i]) { // ignore segment if there is nothing
//		NEATNet->input[i] = NEATNet->input[i] / (entity->engine->options.numUnitsA - 1); // -1 since *this* Unit is not part of computation
//		NEATNet->input[i+1] = NEATNet->input[i+1] / totFriendlyDist;
//	}
//}
//
////Apply Neural Net
//NEATNet->NEATProcess();
//
////CODE BELOW NEEDS TO BE HOOKED UP ONCE WE ARE READY TO USE NEAT
//
//// ONCE NEAT IS CONNECTED USE THE TWO LINES BELOW
//entity->desiredSpeed = (NEATNet->output[0] * (entity->maxSpeed - entity->minSpeed)) + entity->minSpeed;
//entity->desiredHeading = (NEATNet->output[1] - .5) * M_PI;



