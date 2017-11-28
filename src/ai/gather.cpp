/*
 * gather.cpp
 *
 *  Created on: Mar 20, 2013
 *      Author: sushil
 */

#include <ai/gather.h>
#include "DEBUG.h"

namespace FastEcslent {

	FastEcslent::Gather::Gather(Entity* ent, Target *target): UnitCommand(ent, GatherCommand, target) {
		this->scv = dynamic_cast<SCVehicle *> (ent);
		if (target->entity->entityType == MINERALS){
			this->mineralEntity = dynamic_cast<Minerals *> (target->entity);
			this->gasEntity = 0;
		} else {
			this->mineralEntity = 0;
			this->gasEntity = dynamic_cast<Gas *> (target->entity);
		}
		this->commandCenter = findNearestCommandCenter();
		this->finished = false;
		this->state    = MOVETOMINERALS;
	}

	inline bool Gather::done(){
		return finished;
	}

	inline void Gather::init(){
		this->commandCenter = findNearestCommandCenter();
		this->state = MOVETOMINERALS;
		this->finished = false;
		if(mineralEntity){
			this->mineralEntity->miners++;
		}
	}

	/*!
	 * Inherited from base class Command
	 * This SCV was issued another command instead of Gather.
	 * 1. The number of miners of the mineral need decrease by 1
	 * 2. If the SCV is currently mining the mineral, the miner should be removed from the mineral.
	 * @see command.h
	 */
	inline void Gather::postProcess(){
		if(target->entity->entityType == MINERALS){
			Minerals* min = dynamic_cast<Minerals *> (target->entity);
			min ->miners--;
			if(min->miner == this->entity){
				min->miner = 0;
			}
		}
	}

	inline void Gather::switchState(GatherState newState){
		this->state = newState;
		switch(newState){
		case RETURNTOCC:
			this->commandCenter = findNearestCommandCenter();
			break;
		case GATHERINGMINERALS:
			this->scv->mineralCount = 0; //what happens if you run out of minerals while gathering?
			this->scv->gasCount = 0;
			//if time to find a less utilized mineral ent, find and set your mineralEntity
			break;
		case MOVETOMINERALS:
			this->entity->engine->gameMgr->resources[this->scv->entityId.player].minerals += this->scv->mineralCount; // should be 0 or maxMineralCount
			this->entity->engine->gameMgr->resources[this->scv->entityId.player].gas += this->scv->gasCount; // should be 0 or maxMineralCount
			break;
		default:
			break;
		}
	}

	inline void Gather::tick(){
		switch(this->state){
		case RETURNTOCC:
			//this->commandCenter = findNearestCommandCenter();
			moveToEntityTick(this->commandCenter);

			break;

		case GATHERINGMINERALS:
			gatherTick();
			//checks for switching in gatherTick
			break;

		case MOVETOMINERALS:
			moveToEntityTick(this->target->entity);

			break;
		default:
			break;
		}
	}

	void Gather::switchMoveToState(){
		if (this->state == MOVETOMINERALS) {
			switchState(GATHERINGMINERALS);
		} else if (this->state == RETURNTOCC){
			//increment player minerals by 5 == mineralCount == maxMineralCount here
			switchState(MOVETOMINERALS);
		}
	}

	/*!
	 * The SCV moves to mining a mineral.
	 * When the SCV comes in front of the mineral, it will check if the target mineral is being mined by another SCV.
	 * If it is occupied, the SCV will check if there is optimal choice to mine (minerals with fewer miners in the same patch).
	 * If there is no optimal choice, wait until the target mineral is available. Otherwise, go to the optimal one.
	 */
	inline void Gather::moveToEntityTick(Entity *targetEnt){
		if(!targetEnt){
			DEBUG(std::cout << "No target ent: state: " << this->state << std::endl;)
			return;
		}
		relativePos = targetEnt->pos - this->entity->pos;
		if (relativePos.length() < this->scv->length * 2) {  //in front of mineral
			if(this->mineralEntity && this->mineralEntity->miner){   //check if it is mining by another SCV
				Minerals* mineral = this->mineralEntity->engine->gameMgr->getMineralPatch(this->mineralEntity->mineralPatchId)->getNextMineral(this->mineralEntity); //check if there is optimal one
				if(mineral){ //if there is optimal, give up the original mineral and switch to the new mineral.
					this->mineralEntity->miners--;
					this->mineralEntity = mineral;
					this->mineralEntity->miners++;
					return;
				}
			}
			this->entity->desiredHeading = this->entity->heading;
			this->entity->desiredSpeed = 0;
			this->entity->speed = 0;
			switchMoveToState();//and count additional resources if scv was at CC

		} else {
			this->entity->desiredHeading = -atan2(relativePos.z, relativePos.x);
			this->entity->desiredSpeed = this->entity->maxSpeed;
		}
	}


	/*!
	 * Wait/Gather minerals
	 * If the SCV switched into gathering state, there are two possibilities:
	 * 1. Do nothing but wait, in case of another SCV is mining this mineral.
	 * 2. Mining, in case of nobody else is mining this mineral.
	 */
	inline void Gather::gatherTick(){
		//Mining Minerals
		if(this->mineralEntity && (!this->mineralEntity->miner || this->mineralEntity->miner == this->entity )){ //nobody mining, start to mine
			float mineralsGathered = this->scv->mineralGatherRate; // how does this change with number of scvs?
			float fractionOverMaxMineralsPerTrip = 0.0f;
			this->scv->mineralCount += mineralsGathered;
			this->mineralEntity->mineralAmount -= mineralsGathered;
			this->mineralEntity->miner = this->entity;
			if(this->scv->mineralCount >= this->scv->maxMineralCount){
				fractionOverMaxMineralsPerTrip = this->scv->maxMineralCount - this->scv->mineralCount;
				this->mineralEntity->mineralAmount += fractionOverMaxMineralsPerTrip; // give back the fraction
				this->scv->gasCount = 0;
				this->scv->mineralCount = this->scv->maxMineralCount;
				this->mineralEntity->miner = 0;
				switchState(RETURNTOCC);
			}
			// If you run out of minerals
			if(this->mineralEntity->mineralAmount <= 0.0f){
				Entity *ent = this->entity->engine->gameMgr->findClosestEntityOfTypeWithinDistance(MINERALS, this->entity->pos, MaxMineralFindingDistance, this->entity->entityId.side, this->entity->entityId.player);
				if(ent){
					this->mineralEntity = dynamic_cast<Minerals *> (ent);
				} else {
					switchState(STOPPED);
				}
			}
		}
		//Mining Gas
		else if(this->gasEntity && (!this->gasEntity->miner || this->gasEntity->miner == this->entity )){
			float gasGathered = this->scv->gasGatherRate; // how does this change with number of scvs?
			float fractionOverMaxMineralsPerTrip = 0.0f;
			this->scv->gasCount += gasGathered;
			this->gasEntity->gasAmount -= gasGathered;
			this->gasEntity->miner = this->entity;
			if(this->scv->gasCount >= this->scv->maxGasCount){
				fractionOverMaxMineralsPerTrip = this->scv->maxGasCount - this->scv->gasCount;
				this->gasEntity->gasAmount += fractionOverMaxMineralsPerTrip; // give back the fraction
				this->scv->mineralCount = 0;
				this->scv->gasCount = this->scv->maxGasCount;
				this->gasEntity->miner = 0;
				switchState(RETURNTOCC);
			}
			// If you run out of minerals
			if(this->gasEntity->gasAmount <= 0.0f){
				switchState(STOPPED);
			}
		}
	}


	FastEcslent::CommandCenter * FastEcslent::Gather::findNearestCommandCenter(){
		DEBUG(std::cout << "***************************Finding closest Command Center: " << std::endl;)
		Entity *ent = this->entity->engine->gameMgr->findClosestEntityOfTypeWithinDistance(COMMANDCENTER, this->entity->pos, FLT_MAX, this->entity->entityId.side, this->entity->entityId.player);
		if (ent){
			DEBUG(std::cout << "-----------------Found command center: -------------" << ent->uiname  << std::endl;)
			return dynamic_cast<CommandCenter*>(ent);
		} else {
			DEBUG(std::cout << "********************************************No command center found" << std::endl;)
			return 0;
		}
	}

}
