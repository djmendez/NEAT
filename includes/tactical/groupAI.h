/*
 * groupAI.h
 *
 *  Created on: Feb 2, 2012
 *      Author: sushil
 */

#ifndef GROUPAI_H_
#define GROUPAI_H_

#include <deque>
#include <aspect.h>
#include <command.h>
#include <boost/thread/mutex.hpp>

namespace FastEcslent {

class GroupAI : public GroupAspect {
	public:
		std::deque<Command*> commands;
		boost::mutex groupLock;



		GroupAI (Group* grp, GroupAspectType gat): GroupAspect(grp, gat) {};
		virtual void tick();
		virtual void init();
		void addCommand(Command *cmd);
		void setCommand(Command *cmd);
		void setCommandList(std::deque<Command*> cmds);
	};
}

#endif /* GROUPAI_H_ */
