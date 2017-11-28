/*
 * selectionMgr.cpp
 *
 *  Created on: Jan 24, 2012
 *      Author: sushil
 */

#include <selectionMgr.h>
#include <engine.h>
#include <ent.h>
#include <map>
#include "DEBUG.h"

void FastEcslent::SelectionMgr::reset() {
	for (std::map<Identity, Entity *>::iterator it = selectedEnts.begin(); it != selectedEnts.end(); it++){
		it->second->isSelected = false;
	}
	this->selectedEnts.clear();
	primarySelection = NULL;
}

void FastEcslent::SelectionMgr::resetAll() {
	for (int i = 0; i < this->engine->entityMgr->nEnts; i++){
		this->engine->entityMgr->ents[i]->isSelected = false;
	}
	this->selectedEnts.clear();
	primarySelection = NULL;
}

//FastEcslent::SelectionMgr::SelectionMgr() {
//	resetAll();
//}

FastEcslent::SelectionMgr::SelectionMgr(Engine* engine, Options opts): Mgr(engine) {
	resetAll();
	options = opts;
}

void FastEcslent::SelectionMgr::init() {
	DEBUG(std::cout << "SelectionMgr Initialized" << std::endl;)

	resetAll();
}



void FastEcslent::SelectionMgr::tick(){


	return;
}

void FastEcslent::SelectionMgr::updatePrimarySelection(){
	if(!selectedEnts.empty()){
		primarySelection = selectedEnts.begin()->second;
	}
}

//-----------------------------
void FastEcslent::SelectionMgr::select(int i){
	Entity *ent;
	ent = engine->entityMgr->ents[i];
	if (!ent->isSelected) {
		ent->isSelected = true;
		selectedEnts[ent->entityId] = ent;
	}
	updatePrimarySelection();
}
void FastEcslent::SelectionMgr::select(Entity *ent){
	if (!ent->isSelected) {
		ent->isSelected = true;
		selectedEnts[ent->entityId] = ent;
	}
	updatePrimarySelection();
}
//-----------------------------
void FastEcslent::SelectionMgr::unselect(int i){
	Entity *ent;
	ent = engine->entityMgr->ents[i];

	if (ent->isSelected){
		ent->isSelected = false;
		selectedEnts.erase(ent->entityId);
	}
	updatePrimarySelection();
}

void FastEcslent::SelectionMgr::unselect(Entity *ent){
	if (ent->isSelected) {
		ent->isSelected = false;
		selectedEnts.erase(ent->entityId);
	}
	updatePrimarySelection();
}

// Management
FastEcslent::Entity** FastEcslent::SelectionMgr::getSelectedEnts(){
	nSelectedEnts = selectedEnts.size();
	int index = 0;
	for (std::map<Identity, Entity*>::iterator it = selectedEnts.begin(); it != selectedEnts.end(); it++){
		selectedEntsArray[index++] = it->second;
	}
	return selectedEntsArray;
}

FastEcslent::Entity* FastEcslent::SelectionMgr::getPrimarySelection(){
	return this->primarySelection;
}

int FastEcslent::SelectionMgr::getNSelectedEnts(){
	this->nSelectedEnts = this->selectedEnts.size();
	return this->nSelectedEnts;
}

void FastEcslent::SelectionMgr::dump(){
	DEBUG(std::cout << "Selected: (";)
	for (std::map<Identity, Entity*>::iterator it = selectedEnts.begin(); it != selectedEnts.end(); it++){
		DEBUG(std::cout << it->second->entityId.id << ", ";)
	}
	DEBUG(std::cout << ")" << std::endl;)
}
