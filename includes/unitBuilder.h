/*
 * unitBuilder.h
 *
 *  Created on: Mar 2, 2013
 *      Author: sushil
 */

#ifndef UNITBUILDER_H_
#define UNITBUILDER_H_

#include <aspect.h>
#include <enums.h>

namespace FastEcslent {

	class Building;

	class Builder: public UnitAspect {
	private:
            static const int MAX_QUEUE;

	public:

//		double   timeLeft; /*! Time left to finish constructing the current unit */
//		double   timeNeeded;
//		Building *building;
//		EntityType entityType;
            Ogre::Vector3 minBuildRadius;
            Ogre::Vector3 maxBuildRadius;
            
                std::deque<EntityType> entityQueue; /*! Queue of entities to build */
		//bool buildableEntities[NENTITYTYPES]; /*! The type of entities the Builder aspect is allowed to construct */
		//double buildTimeForEntity[NENTITYTYPES];
		Entity *entityBeingBuilt; /*! Pointer to the current entity being build */
		bool waitNetworkResponse;

		//Builder(Building *ent, UnitAspectType ast, EntityType entType);
                Builder(Entity *ent, UnitAspectType ast,Ogre::Vector3 minBuildRadius= Ogre::Vector3(0,0,0),Ogre::Vector3 maxBuildRadius= Ogre::Vector3(0,0,0));
		~Builder();

		virtual void init();
		virtual void tick();

		void startBuildingNextEntity();
                Ogre::Vector3 selectRandPosInRadius(int attempts);
                bool enqueue(EntityType entityType);
		bool removeAtIndex(unsigned int pos);
		bool removeTail(void);
		bool cancelEntityBeingBuilt();

	};



}

#endif /* UNITBUILDER_H_ */
