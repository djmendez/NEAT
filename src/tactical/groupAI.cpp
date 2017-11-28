/*
 * groupAI.cpp
 *
 *  Created on: Feb 2, 2012
 *      Author: sushil
 */
#include <groupAI.h>
#include "DEBUG.h"
#include <boost/thread/mutex.hpp>

inline void FastEcslent::GroupAI::tick() {
	if (!commands.empty()) {
		//do the command
		commands.front()->tick();
		if (commands.front()->done()){ // if done
			commands.pop_front();      // pop top
			if(!commands.empty()){     // if more commands
				commands.front()->init(); // init next command
			}
		}
	}
}

void FastEcslent::GroupAI::init(){
//	DEBUG(std::cout << "Starting GroupAI for Entity: " << group->gid.id  << std::endl;)
	if(!commands.empty()){
		commands.front()->init();
	}
}

void FastEcslent::GroupAI::addCommand(Command* command){
	boost::mutex::scoped_lock scoped_lock(groupLock);
	commands.push_back(command);
	//command->init();
}

void FastEcslent::GroupAI::setCommand(Command* command){
	boost::mutex::scoped_lock scoped_lock(groupLock);
	commands.clear();
	commands.push_back(command);
}

void FastEcslent::GroupAI::setCommandList(std::deque<Command*> cmds){
	boost::mutex::scoped_lock scoped_lock(groupLock);

	commands.clear();
	commands = cmds;
}
