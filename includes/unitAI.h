/*
 * unitAI.h
 *
 *  Created on: Dec 21, 2011
 *      Author: sushil
 */

#ifndef UNITAI_H_
#define UNITAI_H_

#include <deque>
#include <aspect.h>
#include <command.h>
//#include <ent.h>

namespace FastEcslent {

	class Entity;
	//class UnitAspect;

	class UnitAI : public UnitAspect {
		private:
			Target* tgt;
			Guard* guard;
		public:
			std::deque<Command*> commands;

			ControlType state;

			UnitAI (Entity* ent, UnitAspectType at): UnitAspect(ent, at) {};
			virtual void tick();
			virtual void init();
			void addCommand(Command *cmd);
			void setCommand(Command *cmd);
			void setCommandList(std::deque<Command*> comds);

		};
}

#endif /* UNITAI_H_ */
