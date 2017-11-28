/*
 * commandHelp.h
 *
 *  Created on: Feb 26, 2012
 *      Author: sushil
 */

#ifndef COMMANDHELP_H_
#define COMMANDHELP_H_

#include <OgreVector3.h>

#include <ent.h>

//#include <commandFactory.h>
//#include <ai/gather.h>
#include "buildings.h"

namespace FastEcslent {
    
    class CommandFactory;
    class Gather;
    class Minerals;
Move* createMoveForEnt(Entity* ent, Ogre::Vector3 pos);
Move3D* createMove3DForEnt(Entity* ent, Ogre::Vector3 pos, bool isKite);
AttackMove* createAttackForEnt(Entity* ent, Entity* enemy);
AttackMove* createAttackForEnt(Entity* ent, Ogre::Vector3 pos);
AttackMove3D* createAttack3DForEnt(Entity* ent, Entity* enemy);
AttackMove3D* createAttack3DForEnt(Entity* ent, Ogre::Vector3 pos);
Wait* createWaitForEnt(Entity* ent, double wtime);
Gather* createGatherForEntAndMineral(SCVehicle *scv, Entity *resource);
PotentialMove *createPotentialMoveForEnt(Entity *ent, Ogre::Vector3 pos);
Potential3DMove *createPotential3DMoveForEnt(Entity* ent, Ogre::Vector3 pos);
Potential3DMove *createPotential3DMoveForEnt(Entity* ent, Entity* enemy);

Maintain* createMaintainForEnt(Entity* ent, Entity* enemy);
void setMaintainForEnt(Entity* ent, Entity* enemy);

void  setMoveForEnt(Entity* ent, Ogre::Vector3 pos);
void  addMoveForEnt(Entity* ent, Ogre::Vector3 pos);

void  setDirectMoveForEnt(Entity* ent, Ogre::Vector3 pos, bool append);
void  setPotentialMoveForEnt(Entity* ent, Ogre::Vector3 pos, bool append);
std::deque<Command*> addCommandForEnt(Entity* ent, CommandFactory* commandFactory, std::pair<int, Ogre::Vector3 > mouseTargets);



/*!
 * Gather Command
 */
void setGatherForEnt(Entity* ent, Entity* resource, bool append);
std::string EntityTypeToIconName(EntityType entType);

}

#endif /* COMMANDHELP_H_ */
