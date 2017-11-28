/*
 * group.cpp
 *
 *  Created on: Jan 28, 2012
 *      Author: sushil
 */


//#include <ent.h>

#include <engine.h>
#include <group.h>
#include <aspect.h>

#include <boost/lexical_cast.hpp>
#include "DEBUG.h"

std::string FastEcslent::Group::createGroupName(Identity gid){
	std::string tmp = "Group:";
	tmp.append(boost::lexical_cast<std::string>(gid.id));
	return tmp;
}

void FastEcslent::Group::setName(std::string name){
	groupName = name;
}

void FastEcslent::Group::setMembers(Entity** ents, int nEntsInGroup){
	for (int i = 0; i < nEntsInGroup; i++){
		members[i] = ents[i];
	}
	nEntitiesInGroup = nEntsInGroup;
}


void FastEcslent::Group::addMember(Entity* ent){
	members[nEntitiesInGroup++] = ent;
	//ent->aspects.clear(); maybe only for flock?

	DEBUG(std::cout << "\t\tAdded " << ent->uiname << " to group " << this->gid.id << std::endl;)
}

bool FastEcslent::Group::isMember(Entity *ent){
	for (int i = 0; i < this->nEntitiesInGroup; i++){
		if(ent->entityId.id == this->members[i]->entityId.id){
			return true;
		}
	}
	return false;
}

void FastEcslent::Group::reset(Engine *eng){
	engine = eng;
	nEntitiesInGroup = 0;
	leaderIndex = -1;
	this->aspects.clear();
}

FastEcslent::Group::Group(Engine*eng, FastEcslent::Identity id){
	reset(eng);

	gid = id;
	groupName = createGroupName(id);
}

FastEcslent::Group::Group(Engine*eng, Entity** ents, int nEntsInGroup, Identity id){
	reset(eng);
	setMembers(ents, nEntsInGroup);// sets nEntitiesInGroup
	groupName = createGroupName(id);
	gid = id;
}

FastEcslent::Group::Group(Engine*eng, Entity** ents, int nEntsInGroup, std::string name, Identity id){
	reset(eng);
	setMembers(ents, nEntsInGroup);// sets nEntitiesInGroup
	groupName = name;
	gid = id;
}

void FastEcslent::Group::init(){
	for (std::deque<GroupAspect*>::iterator it = aspects.begin(); it != aspects.end(); it++){
		(*it)->init();
	}
}


void FastEcslent::Group::tick(){
	for (std::deque<GroupAspect*>::iterator it = aspects.begin(); it != aspects.end(); it++){
		(*it)->tick();
	}
}

void FastEcslent::Group::print(){
	DEBUG(std::cout << "Group: " << gid.id << std::endl;)
	for(int i = 0; i < nEntitiesInGroup; i++){
		this->members[i]->print();
	}
}

FastEcslent::GroupAspect* FastEcslent::Group::getAspect(GroupAspectType gaspectType) {
	int n = findAspect(gaspectType);
	if (n >= 0){
		return this->aspects[n];
	}else{
		return NULL;
	}
}

void FastEcslent::Group::resetAspect(GroupAspectType gaspType, FastEcslent::GroupAspect* asp) {
	assert(gaspType < NGROUPASPECTTYPES);
	int n = findAspect(gaspType);
	if(n >= 0){
		this->aspects[gaspType] = asp;

	} else {
		addAspect(asp);
	}
	asp->init();
}

int FastEcslent::Group::findAspect(GroupAspectType gat){
	int index = 0;
	for (std::deque<GroupAspect*>::iterator it = aspects.begin(); it != aspects.end(); it++){
		if(gat == (*it)->aspectType){
			return index;
		}
		index++;
	}
	return -1;
}



void FastEcslent::Group::addAspect(FastEcslent::GroupAspect* asp) {
	this->aspects.push_back(asp);
	asp->init();
}




