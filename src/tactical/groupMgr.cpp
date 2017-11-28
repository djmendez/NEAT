/*
 * groupMgr.cpp
 *
 *  Created on: Jan 28, 2012
 *      Author: sushil
 */
#include <groupMgr.h>
#include <mgr.h>
#include <engine.h>
#include "DEBUG.h"

void FastEcslent::GroupMgr::reset() {
	nGroups = 0;
}


//FastEcslent::GroupMgr::GroupMgr() {
//	reset();
//}

FastEcslent::GroupMgr::GroupMgr(Engine* engine, Options opts): Mgr(engine) {
	reset();
	options = opts;
}

void FastEcslent::GroupMgr::init() {
	DEBUG(std::cout << "GroupMgr Initialized" << std::endl;)
	for (int i = 0; i < nGroups;i++){
		groups[i]->init();
	}

}

void FastEcslent::GroupMgr::tick(){
	for (int i = 0; i < nGroups; i++){
		groups[i]->tick();
	}
}

FastEcslent::Group* FastEcslent::GroupMgr::findIfExists(FastEcslent::Group* group){

	for (int i = 0; i < nGroups; i++){
		Group *existingGroup = this->groups[i];
		int nMatches = 0;
		if (existingGroup->nEntitiesInGroup == group->nEntitiesInGroup) {
			for(int j = 0; j < group->nEntitiesInGroup; j++){
				if(existingGroup->isMember(group->members[j])){
					nMatches++;
				}
			}
		}
		if(nMatches == existingGroup->nEntitiesInGroup)
			return existingGroup;
	}
	return 0;
}

FastEcslent::Group* FastEcslent::GroupMgr::createGroup(FastEcslent::Entity** ents, int nMembers){
	assert(nMembers < MaxEnts - 1);

	Identity id;
	id.id         = nGroups;
	id.instanceId = engine->instanceId;

	Group *newGroup = new Group(this->engine, ents, nMembers, id);
	Group* foundGroup = findIfExists(newGroup);

	if (!foundGroup){
		groups[nGroups++] = newGroup;
		return newGroup;
	} else {
		return foundGroup;
	}
//	DEBUG(std::cout << "GroupMgr: created group with " << nEntitiesInGroup << " members." << std::endl;)
	//return group;
}
/*
FastEcslent::Group* FastEcslent::GroupMgr::createGroup(){

	Identity id;
	id.id         = nGroups;
	id.instanceId = engine->instanceId;

	Group *group = new Group(this->engine, id);
	groups[nGroups++] = group;
	DEBUG(std::cout << "GroupMgr: created an empty group." << std::endl;)
	return group;
}
*/

void FastEcslent::GroupMgr::dumpOne(int i){
	assert(i >= 0 && i < nGroups);
	groups[i]->print();
}
void FastEcslent::GroupMgr::dumpAll(){
	for (int i = 0; i < nGroups; i++){
		groups[i]->print();
	}
}
