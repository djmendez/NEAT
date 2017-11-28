/*
 * gather.h
 *
 *  Created on: Mar 20, 2013
 *      Author: sushil
 */

#ifndef GATHER_H_
#define GATHER_H_

#include <command.h>
#include <const.h>
#include <enums.h>
#include <ent.h>
#include <buildings.h>
#include <engine.h>
#include <const.h>
#include <cfloat>

namespace FastEcslent {
/*
	Move (Entity *ent, Target *tgt): UnitCommand(ent, MoveCommand, tgt) {
*/
	enum GatherState {
		MOVETOMINERALS    = 1,
		GATHERINGMINERALS = 2,
		RETURNTOCC        = 3,
		STOPPED           = 4,

		NGATHERSTATES
	};

	class Gather: public UnitCommand {
	private:
		GatherState state;
		SCVehicle   *scv;
		bool        isMineral;
	public:
		bool          finished;
		Minerals      *mineralEntity;
		Gas           *gasEntity;
		CommandCenter *commandCenter;
		CommandCenter *findNearestCommandCenter();

		Gather(Entity *ent, Target *target);//: UnitCommand(ent, GatherCommand, tgt);

		virtual void postProcess();
		virtual bool done();
		virtual void init();
		virtual void tick();
                virtual Command* clone() {};

		void gatherTick();
		void moveToEntityTick(Entity *ent);

		void switchState(GatherState newState);
		void switchMoveToState();

	};

}

#endif /* GATHER_H_ */
