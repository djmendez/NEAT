/*
 * buildings.h
 *
 *  Created on: Jan 9, 2013
 *      Author: sushil
 */

#ifndef BUILDINGS_H_
#define BUILDINGS_H_
#include <boost/lexical_cast.hpp>
#include <deque>
#include <ent.h>
#include <enums.h>
#include "unitBuilder.h"

namespace FastEcslent {
	class Engine;
	class Building : public Entity {
	public:
		//Entity queue

//		std::deque<EntityType> entityQueue;
//		bool buildableEntities[NENTITYTYPES];
//		//double buildTimeForEntity[NENTITYTYPES];
//		Entity *entityBeingBuilt;

		Building(Engine *engine, EntityType etype);
		~Building();

		virtual void init();

//		bool enqueue(EntityType entityType);
//		bool removeAtIndex(unsigned int pos);
//		bool removeTail(void);
//		bool cancelEntityBeingBuilt();

	};

	class Barracks : public Building {
	public:
		//static int count = 0;
		Barracks(Engine *engine) : Building(engine, BARRACKS){
			meshName = "Barracks.mesh";
			uiname =    "Barracks";
			uiname.append(boost::lexical_cast<std::string>(count++));

			// properties
			length = feet(550.0f);
			width  = feet(200.0f);
			height = feet(20.0f);
			maxSpeed = knots(0.0f);
			minSpeed = knots(0.0f);
			speedRange = maxSpeed - minSpeed;
			maxAcceleration = feetPerSecondSquared(0.0f);
			maxRotationalSpeed = degreesPerSecond(0.0f);
			turningRadius = 0;
			mass = tons(50);
			selectable = true;
			entityId.side = BLUE;

                        //this->addAspect(new Builder())
                        builder = new Builder(this, BUILDER,Ogre::Vector3(-400,0,-400), Ogre::Vector3(400,0,400));
                        builder->init();
                        this->addAspect(builder);
			
//                        builder->buildableEntities[MARINE]   = true;
//			builder->buildableEntities[MARAUDER] = true;
//			builder->buildableEntities[REAPER]   = true;
			//buildableEntities[GHOST] = true;

		};
		~Barracks(){};
		//virtual void init(){};
	};

	class CommandCenter : public Building {
	public:
		CommandCenter(Engine *engine) : Building(engine, COMMANDCENTER){
			// properties
			meshName = "CommandCenter.mesh";
			uiname = "CommandCenter";
			uiname.append(boost::lexical_cast<std::string>(count++));

			length = feet(550.0f);
			width  = feet(200.0f);
			height = feet(200.0f);
			maxSpeed = knots(0.0f);
			minSpeed = knots(0.0f);
			speedRange = maxSpeed - minSpeed;
			maxAcceleration = feetPerSecondSquared(0.0f);
			maxRotationalSpeed = degreesPerSecond(0.0f);
			turningRadius = 0;
			mass = tons(60);
			selectable = true;
			entityId.side = BLUE;
			//entityType    = COMMANDCENTER;
                        builder = new Builder(this, BUILDER, Ogre::Vector3(-400,0,-400), Ogre::Vector3(400,0,400));
                        builder->init();
                        this->addAspect(builder);


//			builder->buildableEntities[SCV] = true;

		};
		~CommandCenter(){};
		//virtual void init(){};
	};

	class Factory : public Building {
	public:
		Factory(Engine *engine) : Building(engine, FACTORY){
			// properties
			meshName = "Factory.mesh";
			uiname = "Factory";
			uiname.append(boost::lexical_cast<std::string>(count++));

			length = feet(550.0f);
			width  = feet(200.0f);
			height = feet(15.0f);
			maxSpeed = knots(0.0f);
			minSpeed = knots(0.0f);
			speedRange = maxSpeed - minSpeed;
			maxAcceleration = feetPerSecondSquared(0.0f);
			maxRotationalSpeed = degreesPerSecond(0.0f);
			turningRadius = 0;
			mass = tons(50);
			selectable = true;
			entityId.side = BLUE;
                        
                        builder = new Builder(this, BUILDER,Ogre::Vector3(-400,0,-400), Ogre::Vector3(400,0,400));
                        builder->init();
                        this->addAspect(builder);

//			builder->buildableEntities[TANK] = true;
//			builder->buildableEntities[THOR] = true;
//			builder->buildableEntities[HELLION] = true;

		};
		~Factory(){};
		//virtual void init(){};
	};

	class Refinery : public Building {
	private:


	public:
		Entity* associatedOilField;
		Refinery(Engine *engine) : Building(engine, REFINERY){
			// properties
			meshName = "Refinery.mesh";
			uiname = "GasRefinery";
			uiname.append(boost::lexical_cast<std::string>(count++));

			length = feet(550.0f);
			width  = feet(200.0f);
			height = feet(200.0f);
			maxSpeed = knots(0.0f);
			minSpeed = knots(0.0f);
			speedRange = maxSpeed - minSpeed;
			maxAcceleration = feetPerSecondSquared(0.0f);
			maxRotationalSpeed = degreesPerSecond(0.0f);
			turningRadius = 0;
			mass = tons(50);
			selectable = true;
			entityId.side = BLUE;
			associatedOilField = 0;

		};
		void setAssociatedOilField(Ogre::Vector3 pos);
		~Refinery(){};
		//virtual void init(){};
	};


	class SupplyDepot : public Building {
	public:
		SupplyDepot(Engine *engine) : Building(engine, SUPPLYDEPOT){
			// properties
			meshName = "SupplyDepot.mesh";
			uiname = "SupplyDepot";
			uiname.append(boost::lexical_cast<std::string>(count++));

			length = feet(550.0f);
			width  = feet(200.0f);
			height = feet(200.0f);
			maxSpeed = knots(0.0f);
			minSpeed = knots(0.0f);
			speedRange = maxSpeed - minSpeed;
			maxAcceleration = feetPerSecondSquared(0.0f);
			maxRotationalSpeed = degreesPerSecond(0.0f);
			turningRadius = 0;
			mass = tons(40);
			selectable = true;
			entityId.side = BLUE;


		};
		~SupplyDepot(){};
		//virtual void init(){};
	};


	class EngineeringBay : public Building {
	public:
		EngineeringBay(Engine *engine) : Building(engine, ENGINEERINGBAY){
			// properties
			meshName = "EngineeringBay.mesh";
			uiname = "EngineeringBay";
			uiname.append(boost::lexical_cast<std::string>(count++));

			length = feet(550.0f);
			width  = feet(200.0f);
			height = feet(200.0f);
			maxSpeed = knots(0.0f);
			minSpeed = knots(0.0f);
			speedRange = maxSpeed - minSpeed;
			maxAcceleration = feetPerSecondSquared(0.0f);
			maxRotationalSpeed = degreesPerSecond(0.0f);
			turningRadius = 0;
			mass = tons(40);
			selectable = true;
			entityId.side = BLUE;


		};
		~EngineeringBay(){};
		//virtual void init(){};
	};
	class Armory : public Building {
	public:
		Armory(Engine *engine) : Building(engine, ARMORY){
			// properties
			meshName = "Armory.mesh";
			uiname = "Armory";
			uiname.append(boost::lexical_cast<std::string>(count++));

			length = feet(550.0f);
			width  = feet(200.0f);
			height = feet(200.0f);
			maxSpeed = knots(0.0f);
			minSpeed = knots(0.0f);
			speedRange = maxSpeed - minSpeed;
			maxAcceleration = feetPerSecondSquared(0.0f);
			maxRotationalSpeed = degreesPerSecond(0.0f);
			turningRadius = 0;
			mass = tons(40);
			selectable = true;
			entityId.side = BLUE;


		};
		~Armory(){};
		//virtual void init(){};
	};

	class Minerals;
	/*!
	 * Class MineralPatch
	 * GameMgr has to create a patch of minerals
	 * Every mineral belongs to a MineralPatch
	 * @see Minerals
	 */
	class MineralPatch {
	private:
		std::vector<Minerals*> minerals;
		int patchID;
	public:
		MineralPatch(int id):patchID(id){};
		/*!
		 * Add mineral to a this patch
		 * @param m the minerals to add
		 */
		void addMineral(Minerals *m){
			minerals.push_back(m);
		}
		/*!
		 * Get another optimal mineral within the same patch
		 * @return the optimal mineral which has fewer miners mining. return 0 if there is no better choice.
		 */
		Minerals* getNextMineral(Minerals *m);

		/*!
		 * Get ID of this mineral patch
		 */
		int getID(){return patchID;};
	};

	class Minerals: public Building {

	public:
		float mineralAmount;
		//mineral patch the mineral belongs to
		int mineralPatchId;
		//The number of miners of this Mineral
		int miners;
		//The entity(SCV) which is currently mining
		Entity * miner;
		Minerals(Engine *eng) : Building(eng, MINERALS) {
			meshName = "Mineral.mesh";
			uiname = "DilithiumCrystals";
			uiname.append(boost::lexical_cast<std::string>(count++));

			// properties
			length = meters(50.0f);
			width  = meters(50.0f);
			height = meters(50.0f);
			maxSpeed = knots(0.0f);
			minSpeed = knots(0.0f);
			speedRange = maxSpeed - minSpeed + EPSILON;

			maxAcceleration = feetPerSecondSquared(0.0f);
			maxRotationalSpeed = degreesPerSecond(0.0f);
			turningRadius = 0;
			mass = tons(0);

			selectable = true;

			entityId.side = YELLOW;

			entityState   = ALIVE;
			mineralAmount = 1500;
			miners = 0;
			miner = 0;
			mineralPatchId = 0;
		}
	};

	class Land: public Building {

	public:
		Land(Engine *eng) : Building(eng, LAND) {
			meshName = "land.mesh";
			uiname = "DilithiumCrystals";
			uiname.append(boost::lexical_cast<std::string>(count++));

			// properties
			length = meters(300.0f);
			width  = meters(300.0f);
			height = meters(5.0f);
			maxSpeed = knots(0.0f);
			minSpeed = knots(0.0f);
			speedRange = maxSpeed - minSpeed + EPSILON;

			maxAcceleration = feetPerSecondSquared(0.0f);
			maxRotationalSpeed = degreesPerSecond(0.0f);
			turningRadius = 10000;
			mass = tons(100000);

			selectable = true;

			entityId.side = YELLOW;

			entityState   = ALIVE;
		}
	};

	class Gas: public Building {

	public:
		float gasAmount;
		Entity * miner;
		Entity * refinary;
		Gas(Engine *eng) : Building(eng, GAS) {
			meshName = "Oil.mesh";
			uiname = "Vespene Oil";
			uiname.append(boost::lexical_cast<std::string>(count++));

			// properties
			length = meters(50.0f);
			width  = meters(50.0f);
			height = meters(50.0f);
			maxSpeed = knots(0.0f);
			minSpeed = knots(0.0f);
			speedRange = maxSpeed - minSpeed + EPSILON;

			maxAcceleration = feetPerSecondSquared(0.0f);
			maxRotationalSpeed = degreesPerSecond(0.0f);
			turningRadius = 0;
			mass = tons(0);

			selectable = true;

			entityId.side = YELLOW;

			entityState   = ALIVE;
			gasAmount = 2500; //sc2 (sc1 == 5000)
			miner = 0;
			refinary = 0;
                        this->ai->addCommand(new GasFieldCommand(MaintainCommand, this));
		}
	};
}


#endif /* BUILDINGS_H_ */
