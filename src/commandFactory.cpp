/* 
 * File:   commandFactory.cpp
 * Author: chris
 * 
 * Created on September 16, 2013, 10:25 AM
 */

#include "commandFactory.h"
using namespace FastEcslent;

//CommandFactory::CommandFactory() {
//}

CommandFactory::CommandFactory(const CommandFactory& orig) {
}

CommandFactory::~CommandFactory() {
}

        std::deque<Command*>  BuildStructureFactory::MakeCommand(Entity* owner, std::pair<int, Ogre::Vector3 > mouseTargets) {
            std::deque<Command*> newCommands;
            Ogre::Vector3 movePosition;
            if(this->requiredTarget != TargetPosition)
                movePosition = owner->engine->entityMgr->ents[mouseTargets.first]->pos;
            else
                movePosition = mouseTargets.second;
            newCommands.push_back(createMoveForEnt(owner, movePosition - owner->builder->minBuildRadius));
            newCommands.push_back(new BuildCommand(owner, spawnEnt, FastEcslent::ConstructCommand));
            newCommands.push_back(createWaitForEnt(owner, 10.0));
            //createMoveForEnt(owner, mouseTargets.second); // move
            //new BuildCommand(owner, spawnEnt, FastEcslent::ConstructCommand); // Build
            //createWaitForEnt(owner, mouseTargets.second); //Wait
            //return new BuildCommand(owner, spawnEnt, FastEcslent::ConstructCommand);
            return newCommands;
        }


        std::deque<Command*> AttackFactory::MakeCommand(Entity* owner, std::pair<int, Ogre::Vector3 > mouseTargets) {
            std::deque<Command*> newCommands;
            if(mouseTargets.first > -1 && owner->engine->entityMgr->ents[mouseTargets.first]->entityId.side != owner->entityId.side ){
           		newCommands.push_back(createAttackForEnt(owner, owner->engine->entityMgr->ents[mouseTargets.first]));
            }else{
            	newCommands.push_back(createAttackForEnt(owner, mouseTargets.second));
            }
            return newCommands;
        }
