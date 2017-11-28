/*
 * distance.cpp
 *
 *  Created on: Jan 27, 2012
 *      Author: sushil
 */

#include <distanceMgr.h>
//#include <engine.h>
#include <ent.h>
#include <const.h>

#include <cfloat>
#include <OGRE/OgreVector3.h>
#include "DEBUG.h"

//FastEcslent::DistanceMgr::DistanceMgr(){
//}

FastEcslent::DistanceMgr::DistanceMgr(Engine* eng, Options opts): Mgr (eng){
	options = opts;
	//nEnts = 0;
}

void FastEcslent::DistanceMgr::init(){
	//nEnts = engine->entityMgr->nEnts;
	//ents  = engine->entityMgr->ents;
	for(int i = 0; i < MaxEnts; i++){
		collisions[i] = 0;
		closestEnemy[i] = -1;
		closestEnemyDistance[i] = DBL_MAX;
		furthestEnt[i] = -1;
		furthestDistance[i] = 0;

	}
	collisionTotal = 0;
	//DEBUG(std::cout << "Distance Manager: " << nEnts << " number of ents in game" << std::endl;)

}



float FastEcslent::DistanceMgr::cpaTime(const Entity *ent1, const Entity *ent2){
	Ogre::Vector3 dv = ent1->vel - ent2->vel;
	float dvDotdv = dv.dotProduct(dv);
	if (dvDotdv < FastEcslent::EPSILON) {
		return 0.0;
	}
	Ogre::Vector3 w0 = ent1->pos - ent2->pos;
	return w0.dotProduct(dv)/dvDotdv;
}

float FastEcslent::DistanceMgr::cpaSquaredDistance(const Entity *ent1, const Entity *ent2){
	float cpaT = cpaTime(ent1, ent2);
	Ogre::Vector3 p1 = ent1->pos + (ent1->vel * cpaT);
	Ogre::Vector3 p2 = ent2->pos + (ent2->vel * cpaT);
	return p1.squaredDistance(p2);
}

bool FastEcslent::DistanceMgr::isColliding(const Entity* ent, double collisionDistThreshold)
{
    for(int i = 0; i < engine->entityMgr->nEnts; i++)
    {
        if (ent->entityId.id != i && distance[ent->entityId.id][i] < collisionDistThreshold)
        {
            return true;                        
        }
    }
    return false;
    //return isColliding(ent1->pos, ent1->length, ent1->width, ent2->pos, ent2->length, ent2->width);    
}

bool FastEcslent::DistanceMgr::wouldCollide(Ogre::Vector3 pos, double collisionDistThreshold)
{
    Ogre::Vector3 dist;
    for(int i = 0; i < engine->entityMgr->nEnts; i++)
    {
        dist = pos - engine->entityMgr->ents[i]->pos;
        if (dist.length() < collisionDistThreshold)
        {
            return true;                        
        }
    }
    return false;
    //return isColliding(ent1->pos, ent1->length, ent1->width, ent2->pos, ent2->length, ent2->width);    
}
//bool FastEcslent::DistanceMgr::isColliding(Ogre::Vector3 pos1,float len1, float width1, Ogre::Vector3 pos2,float len2, float width2)
//{
//    Ogre::Vector3 dist = pos1 - pos2;
//    if(Ogre::Math::Abs(dist.x) < (len1+len2) && Ogre::Math::Abs(dist.y) < (width1+width2))
//    {
//        return false;
//    }
//    return true;
//}

void FastEcslent::DistanceMgr::tick (){

	for (int i = 0; i < engine->entityMgr->nEnts; i++){
		ent = engine->entityMgr->ents[i];
		distance[i][i] = 0.0;
		distanceVec[i][i] = Ogre::Vector3(0.0, 0.0, 0.0);

		//need calculate closestEnemyDistance each frame
		closestEnemyDistance[i] = DBL_MAX;

		for (int j = 0; j < engine->entityMgr->nEnts; j++){
			other = engine->entityMgr->ents[j];

			//ignore self
			if(other->entityId.id == ent->entityId.id){
				continue;
			}
			//if the saved entity is dead, reset the distance information.
			if(other->entityState != FastEcslent::ALIVE && closestEnemy[i] == j){
				closestEnemy[i] = -1;
				closestEnemyDistance[i] = DBL_MAX;
				furthestEnt[i] = -1;
				furthestDistance[i] = 0;
				return;
			}
			distanceVec[i][j] = other->pos - ent->pos;
			normalizedDistanceVec[i][j] = distanceVec[i][j].normalisedCopy();
			distanceVec[j][i] = ent->pos - other->pos;
			normalizedDistanceVec[j][i] = distanceVec[j][i].normalisedCopy();
			distance[i][j]    = distanceVec[i][j].length();
			distance[j][i]    = distance[i][j];
/*			if(distance[i][j] < (ent->length + other->length)*2){
				collisions[i]++;
				collisions[j]++;
				collisionTotal++;
			}*/
			//if(distance[i][j] < closestEnemyDistance[i] && other->alive && other->entityId.side != ent->entityId.side){
			if(distance[i][j] < closestEnemyDistance[i] && other->entityState == FastEcslent::ALIVE
					&& other->entityId.side != ent->entityId.side && other->entityId.side != NEUTRAL){
				closestEnemyDistance[i] = distance[i][j];
				closestEnemy[i] = j;

			}
			//if(distance[j][i] < closestEnemyDistance[j] && ent->alive && other->entityId.side != ent->entityId.side){
			if(distance[j][i] < closestEnemyDistance[j] && ent->entityState == FastEcslent::ALIVE
					&& other->entityId.side != ent->entityId.side && ent->entityId.side != NEUTRAL){
				closestEnemyDistance[j] = distance[j][i];
				closestEnemy[j] = i;
			}
			if(distance[i][j] > furthestDistance[i]){
				furthestDistance[i] = distance[i][j];
				furthestEnt[i] = j;
			}
			if(distance[j][i] > furthestDistance[j]){
				furthestDistance[j] = distance[j][i];
				furthestEnt[j] = i;
			}
			cpa[i][j].cpaTime = cpaTime(ent, other);
			cpa[j][i].cpaTime = cpa[i][j].cpaTime;
			cpa[i][j].cpaSquaredDistance = cpaSquaredDistance(ent, other);
			cpa[j][i].cpaTime = cpa[i][j].cpaSquaredDistance;

		}
	}
}

void FastEcslent::DistanceMgr::dumpAll (){
	for (int i = 0; i < engine->entityMgr->nEnts; i++){
		dumpOne(i);
	}
}

void FastEcslent::DistanceMgr::dumpOne (int index){
	//Entity *ent = engine->entityMgr->ents[index];
	for (int j = 0; j < engine->entityMgr->nEnts; j++){
		//if (j == index) continue;
//		DEBUG(std::cout << "Distance from ent: " << ent->entityId.id <<  " to " << ents[j]->entityId.id << " is " << distance[index][j] << std::endl;)
		DEBUG(std::cout << distance[index][j] << ", ";)
	}
	DEBUG(std::cout << std::endl;)
}


