/*
 * aspect.h
 *
 *  Created on: Nov 21, 2011
 *      Author: sushil
 */

#ifndef ASPECT_H_
#define ASPECT_H_

//#include <ent.h>
//#include <group.h>
#include <enums.h>

namespace FastEcslent {

	class Entity;
	class Group;

	class Aspect {
	public:
		Aspect(){};
		virtual void init() = 0;
		virtual void tick() = 0; // {std::cout << "Aspect tick: " << dt << std::endl;		}

	};
}

namespace FastEcslent {

	//-------UnitAspect
	class UnitAspect : public Aspect {
	public:
		Entity *entity;
		UnitAspectType aspectType;
		UnitAspect(Entity *ent, UnitAspectType at): Aspect() {
			entity = ent;
			aspectType = at;
		};
		virtual void init(){};
		virtual void tick(){};

	};

	class GroupAspect: public Aspect {
	public:
		Group *group;
		GroupAspectType aspectType;
		GroupAspect(Group* grp, GroupAspectType gat): Aspect() {
			group = grp;
			aspectType = gat;
		};

		virtual void init(){};
		virtual void tick(){};

	};

}

#endif /* ASPECT_H_ */
