/*
 * group.h
 *
 *  Created on: Jan 28, 2012
 *      Author: sushil
 */

#ifndef GROUP_H_
#define GROUP_H_


#include <boost/lexical_cast.hpp>

#include <identity.h>
#include <ent.h>
#include <string>
//#include <aspect.h>
#include <enums.h>

namespace FastEcslent {

	class GroupAspect;
	class Engine;
//	typedef struct EntId Identity;

	class Group {

	private:
		std::string createGroupName(Identity gid);

	public:
		Engine *engine;
		Entity* members[MaxEnts];
		std::string   groupName;
		//Entity *leader;
		int     leaderIndex;
		int nEntitiesInGroup;
		Identity gid; //a group id (inspired by 'nix)
		std::deque<GroupAspect*> aspects;


		//Constructor

		Group(Engine*eng, Identity id);
		Group(Engine*eng, Entity** ents, int nEntsInGroup, Identity id);
		Group(Engine*eng, Entity** ents, int nEntsInGroup, std::string name, Identity id);
		void reset(Engine*eng);//set instance vars to 0

		void setName(std::string name);
		void setMembers(Entity** ents, int nEntsInGroup);
		void addMember(Entity *ent);
		bool isMember(Entity *ent);

		void setLeaderByIndex(int i) { assert(i >= 0 && i < nEntitiesInGroup); leaderIndex = i; };
		Entity*   getLeaderByIndex() { return members[leaderIndex];};

		void print();

		void resetAspect(GroupAspectType gat, GroupAspect *gasp);
		void addAspect(GroupAspect *gasp);
		int  findAspect(GroupAspectType gat);
		GroupAspect* getAspect(GroupAspectType index);

		void init();
		void tick();


	};


}


#endif /* GROUP_H_ */
