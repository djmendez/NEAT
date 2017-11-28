/*
 * groupMgr.h
 *
 *  Created on: Jan 28, 2012
 *      Author: sushil
 */

#ifndef GROUPMGR_H_
#define GROUPMGR_H_

#include <group.h>
#include <options.h>
#include <mgr.h>

namespace FastEcslent {

	class Engine;

	class GroupMgr : public Mgr {

	private:
		void reset();

	public:
		Group*  groups[MaxGroups];     // MaxGroups in const.h
		Options options;
		int     nGroups;

		GroupMgr(Engine *eng, Options opts);
		GroupMgr();

		void init();
		void tick();


		void dumpOne(int i);
		void dumpAll();

		Group* createGroup(Entity** ents, int n);
		//Group* createGroup();
		Group* findIfExists(Group* g);

	};


}



#endif /* GROUPMGR_H_ */
