/*
 * unutAI.cpp
 *
 *  Created on: Dec 21, 2011
 *      Author: sushil
 */

#include <unitAI.h>
#include "DEBUG.h"
#include <enums.h>
#include <ent.h>
#include <engine.h>

inline void FastEcslent::UnitAI::tick() {
	if( state == NETSLAVE &&
	    entity->entityId.side != entity->engine->options.side &&
	    entity->entityId.player != entity->engine->options.player ){ //controlled by remote
		return;
	}

	if (!commands.empty()) {
//		guard->stopGuarding();
		//do the command
		commands.front()->tick();
		if (commands.front()->done()){ // if done
			commands.pop_front();      // pop top
			if(!commands.empty()){     // if more commands
				commands.front()->init(); // init next command
			}
		}
		state = MANUAL;
	}
//	else{
//		guard->startGuarding();
//		guard->tick(dt);
//	}

}

void FastEcslent::UnitAI::init(){
//	DEBUG(std::cout << "Starting unitAi for Entity: " << entity->entityId.id << std::endl;)
	if(!commands.empty()){
		commands.front()->init();
	}
	tgt = new Target();
	tgt->entity = 0;
	tgt->location = this->entity->pos;
	tgt->offsetDistance  = this->entity->seekRange;
	guard = new Guard(this->entity, this->tgt);
	guard->init();
}

void FastEcslent::UnitAI::addCommand(Command* command){
	commands.push_back(command);
        if(commands.size() == 1) //first command we've pushed, needs to be init
        {
            command->init();
        }
}

void FastEcslent::UnitAI::setCommand(Command* command){
	for(std::deque<Command *>::iterator i= commands.begin();i != commands.end();i++){
		(*i)->postProcess();
	}
	commands.clear();
	commands.push_back(command);
        command->init();
}

void FastEcslent::UnitAI::setCommandList(std::deque<Command*> cmds){
    for(std::deque<Command *>::iterator i= commands.begin();i != commands.end();i++){
		(*i)->postProcess();
	}
	commands.clear();
	commands = cmds;
        commands.front()->init();
}
