/*
 * unitBuilder.cpp
 *
 *  Created on: Mar 2, 2013
 *      Author: sushil
 */
#include <ent.h>
#include <buildings.h>
#include <unitBuilder.h>
#include <engine.h>

namespace FastEcslent {
    const int Builder::MAX_QUEUE = 5;

    //Builder::Builder(Building *building, UnitAspectType aspectType, EntityType entType) : UnitAspect(dynamic_cast<Entity *> (building), aspectType) {
    Builder::Builder(Entity *ent, UnitAspectType aspectType,Ogre::Vector3 minBuildRadius,Ogre::Vector3 maxBuildRadius) : UnitAspect(ent, aspectType), entityBeingBuilt(NULL),minBuildRadius(minBuildRadius),maxBuildRadius(maxBuildRadius), waitNetworkResponse(false) {
//        this->timeNeeded = 600.0;
//        this->timeLeft = -1.0;
//        this->building = building;
//        this->entityType = entType;

//        for (int i = 0; i < NENTITYTYPES; i++) {
//            buildableEntities[i] = false;
//        }

    }

    Builder::~Builder() {

    }

    void Builder::init() {


    }
    
    Ogre::Vector3 Builder::selectRandPosInRadius(int attempts)
    {
        Ogre::Vector3 newPos = Ogre::Vector3(0,0,0);
        for(int i = 0; i < attempts; i++)
            {
                        newPos = this->entity->pos;
                        newPos.x += (Ogre::Math::RangeRandom(minBuildRadius.x, maxBuildRadius.x));
                        newPos.z += (Ogre::Math::RangeRandom(minBuildRadius.z, maxBuildRadius.z));
                                                    
                        if(!(entity->engine->distanceMgr->wouldCollide(newPos, 200.0)))
                        {
                            //this->entityBeingBuilt->pos = newPos;
                            break;
                        }
            }
        return newPos;
        
    }

    void Builder::startBuildingNextEntity() 
    {
        if (this->entity->engine->gameMgr->notExceedPopCap(this->entity->entityId)) {
            Ogre::Vector3 newPos = selectRandPosInRadius(1000);
            if(this->entity->engine->options.isServer)
            {
                this->entityBeingBuilt = this->entity->engine->entityMgr->createEntityForPlayerAndSide(this->entityQueue.front(), newPos, 0.0f, this->entity->entityId.side,this->entity->entityId.player);
            }
            else
            {
                this->entity->engine->net->createEntFromClientReq(this->entity->entityId.id, this->entityQueue.front(), newPos);
                this->waitNetworkResponse = true;
            }
         }
    }

    void Builder::tick() {
//    	if(this->waitNetworkResponse){
//    		return;
//    	}
        if (!this->entityQueue.empty() && !this->waitNetworkResponse ) {
            //this->timeleft -= dt;
            //if (this->timeLeft < 0) {
            if(this->entityBeingBuilt == NULL)
            {
                this->startBuildingNextEntity();
            }

            else if (this->entityBeingBuilt->timeLeftToBirth <=0) {
                //this->building->engine->entityMgr->createEntityForPlayerAndSide(this->building->pos + offset, this->building->heading, this->building->entityId.side, this->building->entityId.player);
                this->entityQueue.pop_front();
                this->entityBeingBuilt = NULL;
                
                //startBuildingNextEntity();
                //create entity
                //popfront
                //start building new front ent

            }
        }




    }

    bool Builder::enqueue(EntityType entType) {
        if (this->entity->engine->gameMgr->entTypeData[this->entity->entityType].buildableEntities[entType] && this->entityQueue.size() < MAX_QUEUE) {
            if (this->entity->engine->gameMgr->preReqExists(this->entity->entityId.player, entType)) {
                this->entity->engine->gameMgr->consumeResources(this->entity->entityId.player, entType);
                this->entityQueue.push_back(entType);
            }
            return true;
        } else {
            return false;
        }
    }
    

    bool Builder::cancelEntityBeingBuilt() {
        return this->entity->engine->entityMgr->cancelGestatingEntity(this->entityBeingBuilt);
    }

    bool Builder::removeAtIndex(unsigned int pos) {
        if (pos < this->entityQueue.size()) {
            this->entityQueue.erase(this->entityQueue.begin() + pos);
            return true;
        } else {
            return false;
        }
    }

    bool Builder::removeTail(void) {
        if (this->entityQueue.empty()) {
            return false;
        } else {
            this->entityQueue.pop_back();
            return true;
        }
    }




}
