/*
 * buildings.cpp
 *
 *  Created on: Jan 9, 2013
 *      Author: sushil
 */
#include <engine.h>

#include <buildings.h>
#include <unitWeapons.h>

#include <const.h>
#include "DEBUG.h"

FastEcslent::Building::Building(Engine *engine, EntityType etype) :
Entity(engine, etype) {
    this->meshName = "Barracks.mesh";
    this->uiname = "Barracks";
    //this->uiname   = uiname.append(boost::lexical_cast<std::string>(id));

    entityClass = STATIC;
    //entityType    = etype;
    //		for(int i = 0; i < NENTITYTYPES; i++){
    //			buildableEntities[i] = false;
    //		}

    // and so on
}

FastEcslent::Building::~Building() {
}

void FastEcslent::Building::init() {
    // do not add any aspects
    weapon = new Weapon(this, WEAPON, &FastEcslent::WeaponTypes::Rifle);
    DEBUG(std::cout << "Created weapon aspect" << std::endl;)
    //weapon->init();
    DEBUG(std::cout << "Initialized weapon aspect" << std::endl;)
            this->addAspect(weapon);
    DEBUG(std::cout << "Added weapon aspect" << std::endl;)
    ai = new UnitAI(this, UNITAI);
    ai->init();
    this->addAspect(ai);

}

//bool FastEcslent::Building::enqueue(EntityType entType){
//	if(buildableEntities[entType]){
//		if(this->engine->gameMgr->preReqExists(entType)){
//			this->entityQueue.push_back(entType);
//			if (this->entityQueue.size() == 1){// if this is first push, start entity building timer
//				if (this->engine->gameMgr->notExceedPopCap(this->entityId)) {
//					this->entityBeingBuilt = this->engine->entityMgr->createEntityAfterTime(entType, this->pos, this->heading);
//				}
//			}
//		}
//		return true;
//	} else {
//		return false;
//	}
//}
//
//bool FastEcslent::Building::cancelEntityBeingBuilt(){
//	return this->engine->entityMgr->cancelGestatingEntity(this->entityBeingBuilt);
//}
//
//
//bool FastEcslent::Building::removeAtIndex(unsigned int pos){
//	if(pos < this->entityQueue.size()) {
//		this->entityQueue.erase(this->entityQueue.begin() + pos);
//		return true;
//	} else {
//		return false;
//	}
//}
//
//bool FastEcslent::Building::removeTail(void){
//	if (this->entityQueue.empty()) {
//		return false;
//	} else {
//		this->entityQueue.pop_back();
//		return true;
//	}
//}

FastEcslent::Minerals* FastEcslent::MineralPatch::getNextMineral(Minerals *m){
	int minMiners = m->miners;
	Minerals * mineral = 0;
	int minDist = 10000;
	for(std::vector<Minerals*>::iterator i= minerals.begin();i!=minerals.end();i++){
		if((*i)->miners < minMiners){   //highest priority is less miners
			minMiners =(*i)->miners;
			mineral = *i;
			minDist = (*i)->pos.distance(m->pos);
		}else if((*i)->miners == minMiners && (*i)->miners < m->miners){ //second priority is shorter distance
			int distance = (*i)->pos.distance(m->pos);
			if(distance < minDist){
				minDist = distance;
				mineral = *i;
			}
		}
	}
	return mineral;
}

void FastEcslent::Refinery::setAssociatedOilField(Ogre::Vector3 pos){
	Entity * gasEnt = 0;
	int minDist = 10000;
	for (int i = 0; i < engine->entityMgr->nEnts; i++){
		Entity* ent = engine->entityMgr->ents[i];
		if(ent->entityType != GAS)
			continue;
		else{
			int distance = ent->pos.distance(pos);
			if(distance < minDist){
				minDist = distance;
				gasEnt = ent;
			}
		}
	}

	if(gasEnt){
		Gas* gas = dynamic_cast<Gas*>(gasEnt);
		this->associatedOilField = gas;
		gas->refinary = this;
	}
}
