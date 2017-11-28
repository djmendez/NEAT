/*
 * selection.h
 *
 *  Created on: Jan 24, 2012
 *      Author: sushil
 */

#ifndef SELECTION_H_
#define SELECTION_H_

#include <const.h>
#include <ent.h>
#include <entHelpers.h>
#include <mgr.h>
#include <options.h>

#include <map>

namespace FastEcslent {

	class Engine;



	class SelectionMgr : public Mgr {

	private:
		void updatePrimarySelection();
		Entity *                 selectedEntsArray[MaxEnts];
		int                      nSelectedEnts;

	public:
		std::map <Identity, Entity *, EntityLessThan> selectedEnts; // MaxEnts in const.h

		Options                  options;
		Entity*                  primarySelection;

		// All mgrs
		SelectionMgr(Engine *eng, Options opts);
		SelectionMgr();
		void init();
		void tick();


		// selection
		void reset(); // set selected ents to unselected, update nSelectedEnts
		void resetAll(); // set all ents to unselected

		void unselect(int i);
		void unselect(Entity *ent);
		void select(int i);
		void select(Entity *ent);
		void dump();

		Entity* getPrimarySelection();
		Entity** getSelectedEnts();
		int  getNSelectedEnts();

	};

}





#endif /* SELECTION_H_ */
